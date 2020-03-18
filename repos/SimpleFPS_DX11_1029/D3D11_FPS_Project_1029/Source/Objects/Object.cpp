#include "../FBXLoader/FBXLoader.h"
#include "../Settings/Global.h"
#include "Object.h"
#include <fstream>
#include <iomanip>
#include <WICTextureLoader.h>
#include <wincodec.h>
#include <wrl.h>
#include <ScreenGrab.h>

using namespace DirectX;
using namespace std;

Object::Object()
	: mVertexCount(0)
{
}

Object::~Object()
{
	mVertexShader.Reset();
	mVertexLayout.Reset();
	mPixelShader.Reset();
	for (int i = 0; i < mMeshCount; ++i)
	{
		mVertexBuffer[i].Reset();
		mTextureResourceView[i].Reset();
	}
	mCBufferMatrix.Reset();
	mCBufferLight.Reset();
	mCBufferAnimMatrix.Reset();
	mRasterizerState.Reset();

	mLayout.reset();

	mCharFbxData->PureVertexData.clear();
	mCharFbxData->InverseMatrixAnim.clear();
	mCharFbxData->KeyframeMatrixPerTime.clear();
}

void Object::Init(const WCHAR* shaderFile, const char* fbxFile, const WCHAR textureFiles[][TEXTURE_LEN])
{
	HRESULT hr;

	mWorldMat = XMMatrixIdentity();


	// Create Character Fbx Data.
	mCharFbxData = new CharacterFbxData();


	// Load Fbx to Character Fbx Data.
	FBXLoader* fbxLoader = new FBXLoader();
	fbxLoader->LoadFbx(mCharFbxData, fbxFile);
	delete fbxLoader;

	mMeshCount = mCharFbxData->PureVertexData.size();
	mVertexBuffer = new ComPtr<ID3D11Buffer>[mMeshCount]();
	mVertexCount = new size_t[mMeshCount];


	// Create Vertex Buffer
	ComPtr<ID3D11Device> Device = Global::GetDevice();

	D3D11_BUFFER_DESC bd;

	for (int i = 0; i < mMeshCount; ++i)
	{
		mVertexCount[i] = mCharFbxData->PureVertexData[i].size();
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.ByteWidth = sizeof(PureVertexData) * mVertexCount[i];
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = mCharFbxData->PureVertexData[i].data();
		hr = Device->CreateBuffer(&bd, &initData, mVertexBuffer[i].GetAddressOf());
		if (FAILED(hr))
		{
			Assert(hr == S_OK);
		}
		mCharFbxData->PureVertexData[i].clear();
	}
	mCharFbxData->PureVertexData.clear();


	// Create Constant Buffer
	bd.ByteWidth = sizeof(CBufferMatrix);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = Device->CreateBuffer(&bd, nullptr, mCBufferMatrix.GetAddressOf());
	if (FAILED(hr))
	{
		Assert(hr == S_OK);
	}
	bd.ByteWidth = sizeof(CBufferLight);
	hr = Device->CreateBuffer(&bd, nullptr, mCBufferLight.GetAddressOf());
	if (FAILED(hr))
	{
		Assert(hr == S_OK);
	}
	if (mCharFbxData->bHasAnimation)
	{
		bd.ByteWidth = sizeof(CBufferAnimMatrix);
		hr = Device->CreateBuffer(&bd, nullptr, mCBufferAnimMatrix.GetAddressOf());
		if (FAILED(hr))
		{
			Assert(hr == S_OK);
		}
	}


	// Create Vertex Shader
	ID3DBlob* pVSBlob = nullptr;
	hr = Global::CompileShaderFromFile(shaderFile, "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		Assert(hr == S_OK);
	}
	hr = Device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, mVertexShader.GetAddressOf());
	if (FAILED(hr))
	{
		pVSBlob->Release();
		Assert(hr == S_OK);
	}


	// Create Layout
	hr = Device->CreateInputLayout(mLayout.get(), mLayoutElementNumber, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &mVertexLayout);
	if (FAILED(hr))
	{
		Assert(hr == S_OK);
	}
	pVSBlob->Release();


	// Create Pixel Shader
	ID3DBlob* pPSBlob = nullptr;
	hr = Global::CompileShaderFromFile(shaderFile, "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		Assert(hr == S_OK);
	}
	hr = Device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, mPixelShader.GetAddressOf());
	if (FAILED(hr))
	{
		pPSBlob->Release();
		Assert(hr == S_OK);
	}
	pPSBlob->Release();


	// Create Texture
	mTextureResourceView = new ComPtr<ID3D11ShaderResourceView>[mMeshCount];
	for (int i = 0; i < mMeshCount; ++i)
	{
		hr = CreateWICTextureFromFile(Device.Get(), Global::GetContext().Get(), textureFiles[i], mTexture2D.GetAddressOf(), mTextureResourceView[i].GetAddressOf(), 0);
		if (FAILED(hr))
		{
			Assert(hr == S_OK);
		}
		/*#ifdef _DEBUG
			WCHAR fileName[128];
			swprintf_s(fileName, 128, L"%s.BMP", textureFiles[i]);
			SaveWICTextureToFile(mD3DContext.Get(), mTexture2D.Get(),
				GUID_ContainerFormatBmp, fileName);
		#endif*/
	}


	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = Device->CreateSamplerState(&sampDesc, mSamplerState.GetAddressOf());
	if (FAILED(hr))
	{
		Assert(hr == S_OK);
	}


	// Create Rasterizer State
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
	cmdesc.FrontCounterClockwise = false;
	hr = Device->CreateRasterizerState(&cmdesc, mRasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		Assert(hr == S_OK);
	}

	InitDetail();
}

void Object::InitDetail()
{
}

void Object::Update()
{
}

void Object::Render()
{
	ComPtr<ID3D11DeviceContext> Context = Global::GetContext();
	static float time = 0;

	UINT stride = sizeof(PureVertexData);
	/*if (!mCharFbxData->bHasAnimation)
	{
		stride -= (sizeof(XMFLOAT4) + sizeof(XMINT4));
	}*/
	UINT offset = 0;
	Context->IASetInputLayout(mVertexLayout.Get());
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CBufferMatrix cBufferMatrix;
	cBufferMatrix.WorldMat = XMMatrixTranspose(mWorldMat);
	cBufferMatrix.ViewMat = XMMatrixTranspose(Global::GetViewMat());
	cBufferMatrix.ProjectionMat = XMMatrixTranspose(Global::GetProjectionMat());
	Context->UpdateSubresource(mCBufferMatrix.Get(), 0, nullptr, &cBufferMatrix, 0, 0);
	Context->VSSetConstantBuffers(0, 1, mCBufferMatrix.GetAddressOf());

	CBufferLight cBufferLight;
	cBufferLight.WorldLightPos = XMFLOAT4(-100.0f, 3500.0f, 1000.0f, 0.0f);
	XMFLOAT3& playerPos = Global::GetPlayerPos();
	cBufferLight.WorldCameraPos = XMFLOAT4(playerPos.x, playerPos.y, playerPos.z, 0.0f);
	Context->UpdateSubresource(mCBufferLight.Get(), 0, nullptr, &cBufferLight, 0, 0);
	Context->VSSetConstantBuffers(1, 1, mCBufferLight.GetAddressOf());

	if (mCharFbxData->bHasAnimation)
	{
		CBufferAnimMatrix cBufferAnimMatrix;
		for (int i = 0; i < mCharFbxData->KeyframeMatrixPerTime[0].size(); ++i)
		{
			cBufferAnimMatrix.FinalMatrices[i] = mCharFbxData->KeyframeMatrixPerTime[(int)time][i];
		}

		Context->UpdateSubresource(mCBufferAnimMatrix.Get(), 0, nullptr, &cBufferAnimMatrix, 0, 0);
		Context->VSSetConstantBuffers(2, 1, mCBufferAnimMatrix.GetAddressOf());

		time += 0.1f;
		if (time > mCharFbxData->EndTime)
		{
			time = 0;
		}
	}

	Context->VSSetShader(mVertexShader.Get(), nullptr, 0);
	Context->PSSetShader(mPixelShader.Get(), nullptr, 0);
	Context->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());

	Context->RSSetState(mRasterizerState.Get());

	for (int i = 0; i < mMeshCount; ++i)
	{
		Context->IASetVertexBuffers(0, 1, mVertexBuffer[i].GetAddressOf(), &stride, &offset);
		Context->PSSetShaderResources(0, 1, mTextureResourceView[i].GetAddressOf());
		Context->Draw(static_cast<UINT>(mVertexCount[i]), 0);
	}
}
