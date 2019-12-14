#include "Global.h"
#include "Object.h"
#include <WICTextureLoader.h>
using namespace DirectX;

Object::Object()
	: mVertexCount(0)
{
}

Object::~Object()
{
	mpVertexShader.Reset();
	mpVertexLayout.Reset();
	mpPixelShader.Reset();
	mpVertexBuffer.Reset();
	mpConstantBuffer.Reset();
	mpTextureRV.Reset();

	mLayout.reset();
	mTimer.reset();
}

void Object::Init(const ComPtr<ID3D11Device> pD3DDevice, HWND hWnd, const WCHAR* shaderFile, 
	const XMMATRIX projection, std::shared_ptr<Timer> timer)
{
	HRESULT hr;

	mProjection = projection;
	mTimer = timer;

	mpD3DDevice = pD3DDevice;
	mpD3DDevice->GetImmediateContext(&mpD3DContext);


	// Create Vertex Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * mVertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = mVertices.get();
	hr = mpD3DDevice->CreateBuffer(&bd, &InitData, mpVertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		assert(hr == S_OK, L"mpD3DDevice->CreateBuffer(vertexBuffer) error");
	}

	// Create Constant Buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = mpD3DDevice->CreateBuffer(&bd, NULL, mpConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		assert(hr == S_OK, L"mpD3DDevice->CreateBuffer(constantBuffer) error");
	}


	// Create Vertex Shader
	ID3DBlob* pVSBlob = nullptr;
	hr = Global::CompileShaderFromFile(shaderFile, "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		assert(hr == S_OK, L"VS CompileShaderFromFile error");
	}
	hr = mpD3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, mpVertexShader.GetAddressOf());
	if (FAILED(hr))
	{
		pVSBlob->Release();
		assert(hr == S_OK, L"mpD3DDevice->CreateVertexShader() error");
	}


	// Create Layout
	hr = mpD3DDevice->CreateInputLayout(mLayout.get(), mLayoutElementNumber, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &mpVertexLayout);
	if (FAILED(hr))
	{
		assert(hr == S_OK, L"mpD3DDevice->CreateInputLayout() error");
	}
	pVSBlob->Release();
	mpD3DContext->IASetInputLayout(mpVertexLayout.Get());


	// Create Pixel Shader
	ID3DBlob* pPSBlob = nullptr;
	hr = Global::CompileShaderFromFile(shaderFile, "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		assert(hr == S_OK, L"PS CompileShaderFromFile error");
	}
	hr = mpD3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, mpPixelShader.GetAddressOf());
	if (FAILED(hr))
	{
		pPSBlob->Release();
		assert(hr == S_OK, L"mpD3DDevice->CreatePixelShader() error");
	}
	pPSBlob->Release();


	// Create Texture
	hr = CreateWICTextureFromFile(mpD3DDevice.Get(), mpD3DContext.Get(), L"Fbx\\wood.png", nullptr, mpTextureRV.GetAddressOf(), 0);
	if (FAILED(hr))
	{
		assert(hr == S_OK, L"CreateWICTextureFromFile() error");
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
	hr = mpD3DDevice->CreateSamplerState(&sampDesc, mpSamplerState.GetAddressOf());
	if (FAILED(hr))
	{
		assert(hr == S_OK, L"mpD3DDevice->CreateSamplerState() error");
	}

	mVertices.reset();

	InitDetail(hWnd);
}

void Object::Update(const XMMATRIX view)
{
	mView = view;
}

void Object::Render()
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	mpD3DContext->IASetVertexBuffers(0, 1, mpVertexBuffer.GetAddressOf(), &stride, &offset);
	mpD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ConstantBuffer cb;
	cb.World = XMMatrixTranspose(mWorld);
	cb.View = XMMatrixTranspose(mView);
	cb.Projection = XMMatrixTranspose(mProjection);
	mpD3DContext->UpdateSubresource(mpConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
	mpD3DContext->VSSetShader(mpVertexShader.Get(), nullptr, 0);
	mpD3DContext->VSSetConstantBuffers(0, 1, mpConstantBuffer.GetAddressOf());
	mpD3DContext->PSSetShader(mpPixelShader.Get(), nullptr, 0);
	mpD3DContext->PSSetShaderResources(0, 1, mpTextureRV.GetAddressOf());
	mpD3DContext->PSSetSamplers(0, 1, mpSamplerState.GetAddressOf());
	mpD3DContext->Draw(mVertexCount, 0);
}

void Object::InitDetail(HWND hWnd)
{
}