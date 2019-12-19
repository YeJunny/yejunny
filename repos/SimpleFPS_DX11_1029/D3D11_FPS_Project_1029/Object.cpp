#include "Global.h"
#include "Object.h"
#include <WICTextureLoader.h>
#include <wincodec.h>
#include <wrl.h>
#include <ScreenGrab.h>
using namespace DirectX;

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
	mConstantBuffer.Reset();
	mTextureRV.Reset();
	mRasterizerState.Reset();

	mLayout.reset();
	mTimer.reset();
}

void Object::Init(const ComPtr<ID3D11Device> d3dDevice, HWND hWnd, 
	const WCHAR* shaderFile, const WCHAR* textureFile,
	const XMMATRIX& projectionMat, std::shared_ptr<Timer> timer)
{
	HRESULT hr;

	mProjectionMat = projectionMat;
	mTimer = timer;

	mD3DDevice = d3dDevice;
	mD3DDevice->GetImmediateContext(&mD3DContext);

	// Create Vertex Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexElements) * mVertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = mVertices.get();
	hr = mD3DDevice->CreateBuffer(&bd, &InitData, mVertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		Assert(hr == S_OK);
	}
	mVertices.reset();


	// Create Constant Buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = mD3DDevice->CreateBuffer(&bd, NULL, mConstantBuffer.GetAddressOf());
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
	mD3DContext->IASetInputLayout(mVertexLayout.Get());
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

void Object::Render()
{
	UINT stride = sizeof(VertexElements);
	UINT offset = 0;
	mD3DContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	mD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ConstantBuffer constBuffer;
	constBuffer.WorldMat = XMMatrixTranspose(mWorldMat);
	constBuffer.ViewMat = XMMatrixTranspose(mViewMat);
	constBuffer.ProjectionMat = XMMatrixTranspose(mProjectionMat);
	constBuffer.LightingPosition = XMFLOAT3(500.0f, 500.0f, -500.0f);
	mD3DContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &constBuffer, 0, 0);
	mD3DContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());
	mD3DContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mD3DContext->PSSetShaderResources(0, 1, mTextureRV.GetAddressOf());
	mD3DContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());
	mD3DContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
	mD3DContext->RSSetState(mRasterizerState.Get());
	mD3DContext->Draw(mVertexCount, 0);
}
