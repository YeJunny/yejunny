#include "Axis.h"
#include "Global.h"

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct ConstantBuffer
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;
};

bool Axis::Init(const ComPtr<ID3D11Device> pD3DDevice, const XMMATRIX projection)
{
	m_projection = projection;
	m_pD3DDevice = pD3DDevice;
	m_pD3DDevice->GetImmediateContext(&m_pD3DContext);

	// SET BUFFER
	SimpleVertex vertices[] =
	{
		{ XMFLOAT3(-15.0f,0.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(15.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.0f, -15.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.0f, 15.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.0f, 0.0f, -15.0f), XMFLOAT4(1.0f, 0.5f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.0f, 0.0f, 15.0f), XMFLOAT4(1.0f, 0.5f, 1.0f, 1.0f) },
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	if (FAILED(m_pD3DDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return false;
	}


	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	if (FAILED(m_pD3DDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer)))
	{
		return false;
	}


	// SET VS
	ID3DBlob* pVSBlob = nullptr;
	if (FAILED(Global::CompileShaderFromFile((WCHAR*)L"Shader\\Axis.fx", "VS", "vs_4_0", &pVSBlob)))
	{
		return false;
	}
	if (FAILED(m_pD3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader)))
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

	if (FAILED(m_pD3DDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &m_pVertexLayout)))
	{
		return false;
	}
	pVSBlob->Release();
	m_pD3DContext->IASetInputLayout(m_pVertexLayout);


	// SET PS
	ID3DBlob* pPSBlob = nullptr;
	if (FAILED(Global::CompileShaderFromFile((WCHAR*)L"Shader\\Axis.fx", "PS", "ps_4_0", &pPSBlob)))
	{
		return false;
	}
	if (FAILED(m_pD3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader)))
	{
		pPSBlob->Release();
		return false;
	}
	pPSBlob->Release();

	return true;
}

void Axis::Update(const XMMATRIX view)
{
	m_view = view;
}

void Axis::Render()
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pD3DContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	ConstantBuffer cb;
	m_world = XMMatrixIdentity();
	cb.World = XMMatrixTranspose(m_world);
	cb.View = XMMatrixTranspose(m_view);
	cb.Projection = XMMatrixTranspose(m_projection);

	m_pD3DContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	m_pD3DContext->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pD3DContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pD3DContext->PSSetShader(m_pPixelShader, nullptr, 0);

	m_pD3DContext->Draw(6, 0);
}

void Axis::Release()
{
	if (m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = nullptr;
	}

	if (m_pVertexLayout)
	{
		m_pVertexLayout->Release();
		m_pVertexLayout = nullptr;
	}

	if (m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = nullptr;
	}

	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}

	if (m_pConstantBuffer)
	{
		m_pConstantBuffer->Release();
		m_pConstantBuffer = nullptr;
	}
}