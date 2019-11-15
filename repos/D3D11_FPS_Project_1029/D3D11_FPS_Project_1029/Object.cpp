#include "Global.h"
#include "Object.h"
using namespace DirectX;

Object::Object()
	: mWorld()
	, mView()
	, mProjection()
	, mpVertexBuffer(nullptr)
	, mpVertexShader(nullptr)
	, mpVertexLayout(nullptr)
	, mpConstantBuffer(nullptr)
	, mpPixelShader(nullptr)
	, mpWireFrame(nullptr)
{
}

bool Object::Init(const ComPtr<ID3D11Device> pD3DDevice, const WCHAR* shaderFile, const XMMATRIX projection)
{
	mProjection = projection;
	mpD3DDevice = pD3DDevice;
	mpD3DDevice->GetImmediateContext(&mpD3DContext);

	// SET BUFFER
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * mVertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = mVertices;
	if (FAILED(mpD3DDevice->CreateBuffer(&bd, &InitData, &mpVertexBuffer)))
	{
		return false;
	}

	InitIndexBuffer();

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	if (FAILED(mpD3DDevice->CreateBuffer(&bd, NULL, &mpConstantBuffer)))
	{
		return false;
	}


	// SET VS
	ID3DBlob* pVSBlob = nullptr;
	if (FAILED(Global::CompileShaderFromFile(shaderFile, "VS", "vs_4_0", &pVSBlob)))
	{
		return false;
	}
	if (FAILED(mpD3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &mpVertexShader)))
	{
		pVSBlob->Release();
		return false;
	}


	// SET LAYOUT
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	if (FAILED(mpD3DDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &mpVertexLayout)))
	{
		return false;
	}
	pVSBlob->Release();
	mpD3DContext->IASetInputLayout(mpVertexLayout);


	// SET PS
	ID3DBlob* pPSBlob = nullptr;
	if (FAILED(Global::CompileShaderFromFile(shaderFile, "PS", "ps_4_0", &pPSBlob)))
	{
		return false;
	}
	if (FAILED(mpD3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mpPixelShader)))
	{
		pPSBlob->Release();
		return false;
	}
	pPSBlob->Release();


	// other
	/*D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;
	m_pD3DDevice->CreateRasterizerState(&wfdesc, &m_pWireFrame);*/
	
	delete[] mVertices;

	InitDetail();

	return true;
}

void Object::Update(const XMMATRIX view)
{
	mView = view;
}

void Object::Render()
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	mpD3DContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
	mpD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ConstantBuffer cb;
	cb.World = XMMatrixTranspose(mWorld);
	cb.View = XMMatrixTranspose(mView);
	cb.Projection = XMMatrixTranspose(mProjection);
	mpD3DContext->UpdateSubresource(mpConstantBuffer, 0, nullptr, &cb, 0, 0);
	mpD3DContext->VSSetShader(mpVertexShader, nullptr, 0);
	mpD3DContext->VSSetConstantBuffers(0, 1, &mpConstantBuffer);
	mpD3DContext->PSSetShader(mpPixelShader, nullptr, 0);
	mpD3DContext->Draw(mVertexCount, 0);
}

void Object::Release()
{
	if (mpVertexShader)
	{
		mpVertexShader->Release();
		mpVertexShader = nullptr;
	}

	if (mpVertexLayout)
	{
		mpVertexLayout->Release();
		mpVertexLayout = nullptr;
	}

	if (mpPixelShader)
	{
		mpPixelShader->Release();
		mpPixelShader = nullptr;
	}

	if (mpVertexBuffer)
	{
		mpVertexBuffer->Release();
		mpVertexBuffer = nullptr;
	}

	if (mpConstantBuffer)
	{
		mpConstantBuffer->Release();
		mpConstantBuffer = nullptr;
	}
}

void Object::InitIndexBuffer()
{
}

void Object::InitDetail()
{
}