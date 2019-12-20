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

//#define FBX_WRITE

Object::Object()
	: mVertexCount(0)
{
}

Object::~Object()
{
	mVertexShader.Reset();
	mVertexLayout.Reset();
	mPixelShader.Reset();
	mVertexBuffer.Reset();
	mCBufferMatrix.Reset();
	mCBufferLight.Reset();
	mTextureRV.Reset();
	mRasterizerState.Reset();

	mLayout.reset();
	mTimer.reset();
}

void Object::Init(const ComPtr<ID3D11Device> d3dDevice, HWND hWnd, 
	const WCHAR* shaderFile, const char* fbxFile, const WCHAR* textureFile,
	const XMMATRIX& projectionMat, std::shared_ptr<Timer> timer)
{
	HRESULT hr;

	mProjectionMat = projectionMat;
	mTimer = timer;

	mD3DDevice = d3dDevice;
	mD3DDevice->GetImmediateContext(&mD3DContext);

	// Fbx load
	FBXLoader* fbxLoader = new FBXLoader();
	fbxLoader->LoadFbx(fbxFile);

	mVertexCount = fbxLoader->GetVertexCount();
	Assert(mVertexCount);
	VertexElements* vertices = new VertexElements[mVertexCount];

#ifdef FBX_WRITE
	std::string fileName = std::string(fbxFile) + "Vertices.txt";
	std::ofstream writeFile(fileName.data());
	if (writeFile.is_open())
	{
		writeFile << std::setw(15 * 2) << "Normal" << std::setw(15 * 3) << "UV" << std::setw(15 * 3) << "Pos\n";
	}
#endif
	for (size_t i = 0; i < mVertexCount; ++i)
	{
		vertices[i].Pos = (fbxLoader->GetVertices())[i];
		vertices[i].UV = (fbxLoader->GetUVs())[i];
		vertices[i].Normal = (fbxLoader->GetNormals())[i];
#ifdef FBX_WRITE
		if (writeFile.is_open())
		{
			writeFile << "(" << std::setw(15) << vertices[i].Normal.x << ", " << std::setw(15) << vertices[i].Normal.y << ", " << std::setw(15) << vertices[i].Normal.z << ")" << " | "
				<< "(" << std::setw(15) << vertices[i].UV.x << ", " << std::setw(15) << vertices[i].UV.y << ")" << " | "
				<< "(" << std::setw(15) << vertices[i].Pos.x << ", " << std::setw(15) << vertices[i].Pos.y << ", " << std::setw(15) << vertices[i].Pos.z << ")\n";
		}
#endif
	}
#ifdef FBX_WRITE
	writeFile << mVertexCount << "\n";
	writeFile.close();
#endif
	delete fbxLoader;

	// Create Vertex Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexElements) * (UINT)mVertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;
	hr = mD3DDevice->CreateBuffer(&bd, &initData, mVertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		Assert(hr == S_OK);
	}
	delete[] vertices;

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
	hr = CreateWICTextureFromFile(mD3DDevice.Get(), mD3DContext.Get(), textureFile, mTexture2D.GetAddressOf(), mTextureRV.GetAddressOf(), 0);
	if (FAILED(hr))
	{
		Assert(hr == S_OK);
	}
//#ifdef _DEBUG
//	SaveWICTextureToFile(mD3DContext.Get(), mTexture2D.Get(),
//		GUID_ContainerFormatBmp, L"SCREENSHOT.BMP");
//#endif

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
	mD3DContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	mD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CBufferMatrix cBufferMatrix;
	cBufferMatrix.WorldMat = XMMatrixTranspose(mWorldMat);
	cBufferMatrix.ViewMat = XMMatrixTranspose(mViewMat);
	cBufferMatrix.ProjectionMat = XMMatrixTranspose(mProjectionMat);

	CBufferLight cBufferLight;
	cBufferLight.WorldLightPos = XMFLOAT4(20.0f, 20.0f, 0.0f, 0.0f);
	cBufferLight.WorldCameraPos = XMFLOAT4(playerPos.x, playerPos.y, playerPos.z, 0.0f);

	mD3DContext->UpdateSubresource(mCBufferMatrix.Get(), 0, nullptr, &cBufferMatrix, 0, 0);
	mD3DContext->UpdateSubresource(mCBufferLight.Get(), 0, nullptr, &cBufferLight, 0, 0);
	mD3DContext->VSSetConstantBuffers(0, 1, mCBufferMatrix.GetAddressOf());
	mD3DContext->VSSetConstantBuffers(1, 1, mCBufferLight.GetAddressOf());

	mD3DContext->VSSetShader(mVertexShader.Get(), nullptr, 0);

	mD3DContext->PSSetShaderResources(0, 1, mTextureRV.GetAddressOf());
	mD3DContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());
	mD3DContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

	mD3DContext->RSSetState(mRasterizerState.Get());

	mD3DContext->Draw((UINT)mVertexCount, 0);
}
