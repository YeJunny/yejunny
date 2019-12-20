#include "Axis.h"
#include "Global.h"

void Axis::Init(const ComPtr<ID3D11Device> d3dDevice,
	const WCHAR* shaderFile, const XMMATRIX& projectionMat, std::shared_ptr<Timer> timer)
{
	HRESULT hr;

	mProjectionMat = projectionMat;
	mTimer = timer;

	mD3DDevice = d3dDevice;
	mD3DDevice->GetImmediateContext(&mD3DContext);

	VertexElements vertices[6];
	vertices[0].Pos = XMFLOAT3(-15.0f, 0.0f, 0.0f);
	vertices[1].Pos = XMFLOAT3(15.0f, 0.0f, 0.0f);
	vertices[2].Pos = XMFLOAT3(0.0f, -15.0f, 0.0f);
	vertices[3].Pos = XMFLOAT3(0.0f, 15.0f, 0.0f);
	vertices[4].Pos = XMFLOAT3(0.0f, 0.0f, -15.0f);
	vertices[5].Pos = XMFLOAT3(0.0f, 0.0f, 15.0f);
	mVertexCount = 6;

	D3D11_INPUT_ELEMENT_DESC layout = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };

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

	// Create Constant Buffer
	bd.ByteWidth = sizeof(CBufferMatrix);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = mD3DDevice->CreateBuffer(&bd, nullptr, mCBufferMatrix.GetAddressOf());
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
	hr = mD3DDevice->CreateInputLayout(&layout, 1, pVSBlob->GetBufferPointer(),
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
}

void Axis::Render()
{
	UINT stride = sizeof(VertexElements);
	UINT offset = 0;
	mD3DContext->IASetInputLayout(mVertexLayout.Get());
	mD3DContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	mD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	CBufferMatrix cBufferMatrix;
	cBufferMatrix.WorldMat = XMMatrixTranspose(mWorldMat);
	cBufferMatrix.ViewMat = XMMatrixTranspose(mViewMat);
	cBufferMatrix.ProjectionMat = XMMatrixTranspose(mProjectionMat);

	mD3DContext->UpdateSubresource(mCBufferMatrix.Get(), 0, nullptr, &cBufferMatrix, 0, 0);
	mD3DContext->VSSetConstantBuffers(0, 1, mCBufferMatrix.GetAddressOf());

	mD3DContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mD3DContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

	mD3DContext->Draw((UINT)mVertexCount, 0);
}
