#include "Global.h"
#include "Triangle.h"
using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

bool Triangle::Init(ID3D11Device* pD3DDevice, ID3D11DeviceContext* pD3DContext)
{
	m_pD3DDevice = pD3DDevice;
	m_pD3DContext = pD3DContext;

	SimpleVertex simpleVertex[] =

	{
		{ XMFLOAT3(0.0f, 0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	};

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = 3 * sizeof(SimpleVertex);
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
	subData.pSysMem = simpleVertex;

	if (FAILED(m_pD3DDevice->CreateBuffer(&desc, &subData, &m_pVertexBuffer)))
	{
		return false;
	}
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pD3DContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);


	ID3DBlob* pVSBlob;
	if (FAILED(Global::CompileShaderFromFile((WCHAR*)L"Shader\\Tutorial05.fx", "VS", "vs_4_0", &pVSBlob)))
	{
		return false;
	}
	if (FAILED(m_pD3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader)))
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC inputElement[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	if (FAILED(m_pD3DDevice->CreateInputLayout(inputElement, ARRAYSIZE(inputElement),
		pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pInputLayout)))
	{
		return false;
	}
	pVSBlob->Release();


	ID3DBlob* pPSBlob;
	if (FAILED(Global::CompileShaderFromFile((WCHAR*)L"Shader\\Tutorial05.fx", "PS", "ps_4_0", &pPSBlob)))
	{
		return false;
	}
	if (FAILED(m_pD3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(),
		nullptr, &m_pPixelShader)))
	{
		return false;
	}

	m_pD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

void Triangle::Update()
{


}

void Triangle::Render()
{
	m_pD3DContext->IASetInputLayout(m_pInputLayout);

	m_pD3DContext->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pD3DContext->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pD3DContext->Draw(3, 0);
}

void Triangle::Release()
{
	if (m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = nullptr;
	}

	if (m_pInputLayout)
	{
		m_pInputLayout->Release();
		m_pInputLayout = nullptr;
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
}