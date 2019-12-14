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

void Object::Init(const ComPtr<ID3D11Device> pD3DDevice, HWND hWnd, 
	const WCHAR* shaderFile, const WCHAR* textureFile,
	const XMMATRIX projection, std::shared_ptr<Timer> timer)
{
	HRESULT hr;

	mProjection = projection;
	mTimer = timer;

	mD3DDevice = pD3DDevice;
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
		assert(hr == S_OK, L"mpD3DDevice->CreateBuffer(vertexBuffer) error");
	}


	// Create Constant Buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = mD3DDevice->CreateBuffer(&bd, NULL, mConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		assert(hr == S_OK, L"mpD3DDevice->CreateBuffer(constantBuffer) error");
	}


	// Create Vertex Shader
	ID3DBlob* pVSBlob = nullptr;
	hr = Global::CompileShaderFromFile(shaderFile, "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		assert(hr == S_OK, L"VS CompileShaderFromFile() error");
	}
	hr = mD3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, mVertexShader.GetAddressOf());
	if (FAILED(hr))
	{
		pVSBlob->Release();
		assert(hr == S_OK, L"mpD3DDevice->CreateVertexShader() error");
	}


	// Create Layout
	hr = mD3DDevice->CreateInputLayout(mLayout.get(), mLayoutElementNumber, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &mVertexLayout);
	if (FAILED(hr))
	{
		assert(hr == S_OK, L"mpD3DDevice->CreateInputLayout() error");
	}
	pVSBlob->Release();
	mD3DContext->IASetInputLayout(mVertexLayout.Get());


	// Create Pixel Shader
	ID3DBlob* pPSBlob = nullptr;
	hr = Global::CompileShaderFromFile(shaderFile, "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		assert(hr == S_OK, L"PS CompileShaderFromFile() error");
	}
	hr = mD3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, mPixelShader.GetAddressOf());
	if (FAILED(hr))
	{
		pPSBlob->Release();
		assert(hr == S_OK, L"mpD3DDevice->CreatePixelShader() error");
	}
	pPSBlob->Release();


	// Create Texture
	hr = CreateWICTextureFromFile(mD3DDevice.Get(), mD3DContext.Get(), textureFile, mTexture2D.GetAddressOf(), mTextureRV.GetAddressOf(), 0);
	if (FAILED(hr))
	{
		assert(hr == S_OK, L"CreateWICTextureFromFile() error");
	}
#ifdef _DEBUG
	SaveWICTextureToFile(mD3DContext.Get(), mTexture2D.Get(),
		GUID_ContainerFormatBmp, L"SCREENSHOT.BMP");
#endif

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
		assert(hr == S_OK, L"mpD3DDevice->CreateSamplerState() error");
	}

	// Create Rasterizer State
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
	cmdesc.FrontCounterClockwise = true;
	//hr = mpD3DDevice->CreateRasterizerState(&cmdesc, &CCWcullMode);
	cmdesc.FrontCounterClockwise = false;
	//hr = mpD3DDevice->CreateRasterizerState(&cmdesc, &CWcullMode);
	cmdesc.CullMode = D3D11_CULL_NONE;
	hr = mD3DDevice->CreateRasterizerState(&cmdesc, mRasterizerState.GetAddressOf());


	mVertices.reset();

	InitDetail(hWnd);
}

void Object::Update(const XMMATRIX view)
{
	mView = view;
}

void Object::Render()
{
	UINT stride = sizeof(VertexElements);
	UINT offset = 0;
	mD3DContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	mD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ConstantBuffer cb;
	cb.World = XMMatrixTranspose(mWorld);
	cb.View = XMMatrixTranspose(mView);
	cb.Projection = XMMatrixTranspose(mProjection);
	mD3DContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
	mD3DContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mD3DContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());
	mD3DContext->PSSetShaderResources(0, 1, mTextureRV.GetAddressOf());
	mD3DContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());
	mD3DContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
	mD3DContext->RSSetState(mRasterizerState.Get());
	mD3DContext->Draw(mVertexCount, 0);
}

void Object::InitDetail(HWND hWnd)
{
}