#include "Cube.h"
#include "Global.h"

Cube::Cube()
{
	mVertexCount = 8;
	SimpleVertex vertices[] = 
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
	};

	mVertices = new SimpleVertex[mVertexCount];
	memcpy(mVertices, vertices, sizeof(vertices));
}

void Cube::Update(const XMMATRIX view)
{
	static float radians = 0.0f;
	ULONGLONG diffTick = mTimer->GetDiffTick();
	radians += diffTick * 0.001f;

	if (radians >= XM_PI * 2)
	{
		radians -= XM_PI * 2;
	}

	mWorld = XMMatrixRotationY(radians) * XMMatrixTranslation(10.0f, 0.5f, 10.0f);
	mView = view;
}

void Cube::Render()
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	mpD3DContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
	mpD3DContext->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	mpD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ConstantBuffer cb;
	cb.World = XMMatrixTranspose(mWorld);
	cb.View = XMMatrixTranspose(mView);
	cb.Projection = XMMatrixTranspose(mProjection);

	mpD3DContext->UpdateSubresource(mpConstantBuffer, 0, nullptr, &cb, 0, 0);

	mpD3DContext->VSSetShader(mpVertexShader, nullptr, 0);
	mpD3DContext->VSSetConstantBuffers(0, 1, &mpConstantBuffer);
	mpD3DContext->PSSetShader(mpPixelShader, nullptr, 0);

	mpD3DContext->DrawIndexed(mIndexCount, 0, 0);
}

void Cube::Release()
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

	if (mpIndexBuffer)
	{
		mpIndexBuffer->Release();
		mpIndexBuffer = nullptr;
	}

	if (mpConstantBuffer)
	{
		mpConstantBuffer->Release();
		mpConstantBuffer = nullptr;
	}
}

void Cube::InitIndexBuffer()
{
	mIndexCount = 36;
	WORD* indices = new WORD[mIndexCount]
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * mIndexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;
	if (FAILED(mpD3DDevice->CreateBuffer(&bd, &InitData, &mpIndexBuffer)))
	{
		return;
	}
	delete[] indices;
}