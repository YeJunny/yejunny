#include "FBXLoader.h"
#include "Global.h"
#include "Object.h"
#include <fstream>
#include <iomanip>
#include <WICTextureLoader.h>
#include <wincodec.h>
#include <wrl.h>
#include <ScreenGrab.h>
using namespace DirectX;
using namespace std;

#define FBX_WRITE

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
	mRasterizerState.Reset();

	mLayout.reset();
	mTimer.reset();
}

void Object::Init(const ComPtr<ID3D11Device> d3dDevice, HWND hWnd,
	const WCHAR* shaderFile, const char* fbxFile, const WCHAR textureFiles[][TEXTURE_LEN],
	const XMMATRIX& projectionMat, std::shared_ptr<Timer> timer)
{
	HRESULT hr;

	mWorldMat = XMMatrixIdentity();
	mProjectionMat = projectionMat;
	mTimer = timer;

	mD3DDevice = d3dDevice;
	mD3DDevice->GetImmediateContext(&mD3DContext);

	vector<vector<VertexElements>> verticesElementsVector;
	verticesElementsVector.reserve(10);

	FBXLoader* fbxLoader = new FBXLoader();
	fbxLoader->LoadFbx(&verticesElementsVector, fbxFile);
	delete fbxLoader;

	mMeshCount = verticesElementsVector.size();
	mVertexBuffer = new ComPtr<ID3D11Buffer>[mMeshCount]();
	mVertexCount = new size_t[mMeshCount];

	// Create Vertex Buffer

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	for (int i = 0; i < mMeshCount; ++i)
	{
		mVertexCount[i] = verticesElementsVector[i].size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(verticesElementsVector[i]) * mVertexCount[i];
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = verticesElementsVector[i].data();
		hr = mD3DDevice->CreateBuffer(&bd, &initData, mVertexBuffer[i].GetAddressOf());
		if (FAILED(hr))
		{
			Assert(hr == S_OK);
		}
		verticesElementsVector[i].clear();
	}
	verticesElementsVector.clear();

	// Create Constant Buffer
	bd.ByteWidth = sizeof(CBufferMatrix);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = mD3DDevice->CreateBuffer(&bd, nullptr, mCBufferMatrix.GetAddressOf());
	if (FAILED(hr))
	{
		Assert(hr == S_OK);
	}
	bd.ByteWidth = sizeof(CBufferLight);
	hr = mD3DDevice->CreateBuffer(&bd, nullptr, mCBufferLight.GetAddressOf());
	if (FAILED(hr))
	{
		Assert(hr == S_OK);
	}


	// Create Vertex Shader
	ID3DBlob* pVSBlob = nullptr;
	hr = Global::CompileShaderFromFile(shaderFile, "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		Assert(hr == S_OK);
	}
	hr = mD3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, mVertexShader.GetAddressOf());
	if (FAILED(hr))
	{
		pVSBlob->Release();
		Assert(hr == S_OK);
	}


	// Create Layout
	hr = mD3DDevice->CreateInputLayout(mLayout.get(), mLayoutElementNumber, pVSBlob->GetBufferPointer(),
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
	hr = mD3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, mPixelShader.GetAddressOf());
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
		hr = CreateWICTextureFromFile(mD3DDevice.Get(), mD3DContext.Get(), textureFiles[i], mTexture2D.GetAddressOf(), mTextureResourceView[i].GetAddressOf(), 0);
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
	hr = mD3DDevice->CreateSamplerState(&sampDesc, mSamplerState.GetAddressOf());
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
	hr = mD3DDevice->CreateRasterizerState(&cmdesc, mRasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		Assert(hr == S_OK);
	}

	InitDetail(hWnd);
}

void Object::InitDetail(HWND hWnd)
{
}

void Object::Update(const XMMATRIX& viewMat)
{
	mViewMat = viewMat;
}

void Object::Render(const XMFLOAT3& playerPos)
{
	UINT stride = sizeof(VertexElements);
	UINT offset = 0;
	mD3DContext->IASetInputLayout(mVertexLayout.Get());
	mD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CBufferMatrix cBufferMatrix;
	cBufferMatrix.WorldMat = XMMatrixTranspose(mWorldMat);
	cBufferMatrix.ViewMat = XMMatrixTranspose(mViewMat);
	cBufferMatrix.ProjectionMat = XMMatrixTranspose(mProjectionMat);

	CBufferLight cBufferLight;
	cBufferLight.WorldLightPos = XMFLOAT4(500.0f, 500.0f, -500.0f, 0.0f);
	cBufferLight.WorldCameraPos = XMFLOAT4(playerPos.x, playerPos.y, playerPos.z, 0.0f);

	mD3DContext->UpdateSubresource(mCBufferMatrix.Get(), 0, nullptr, &cBufferMatrix, 0, 0);
	mD3DContext->UpdateSubresource(mCBufferLight.Get(), 0, nullptr, &cBufferLight, 0, 0);
	mD3DContext->VSSetConstantBuffers(0, 1, mCBufferMatrix.GetAddressOf());
	mD3DContext->VSSetConstantBuffers(1, 1, mCBufferLight.GetAddressOf());

	mD3DContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mD3DContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
	mD3DContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());

	mD3DContext->RSSetState(mRasterizerState.Get());
	
	for (int i = 0; i < mMeshCount; ++i)
	{
		mD3DContext->IASetVertexBuffers(0, 1, mVertexBuffer[i].GetAddressOf(), &stride, &offset);
		mD3DContext->PSSetShaderResources(0, 1, mTextureResourceView[i].GetAddressOf());
		mD3DContext->Draw(static_cast<UINT>(mVertexCount[i]), 0);
	}
}
