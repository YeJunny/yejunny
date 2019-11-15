#include "Global.h"
#include "Ground.h"

Ground::Ground()
{
	mRow = 10;
	mCol = 10;
	const float gap = 0.4f;
	mVertexCount = (mRow + 1) * (mCol + 1);
	mIndexCount = ((mRow + 1) + (mCol + 1)) * 2;

	// SET BUFFER
	mVertices = new SimpleVertex[mVertexCount];

	int index = 0;
	for (int z = 0; z <= mRow; ++z)
	{
		for (int x = 0; x <= mCol; ++x)
		{
			mVertices[index].Pos.z = (mCol / 2) * gap * z - gap;
			mVertices[index].Pos.y = -0.5f;
			mVertices[index].Pos.x = (mRow / 2) * gap * x - gap;
			mVertices[index].Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			index++;
		}
	}
}

void Ground::Render()
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	mpD3DContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
	mpD3DContext->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	mpD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	ConstantBuffer cb;
	mWorld = XMMatrixIdentity();
	cb.World = XMMatrixTranspose(mWorld);
	cb.View = XMMatrixTranspose(mView);
	cb.Projection = XMMatrixTranspose(mProjection);

	mpD3DContext->UpdateSubresource(mpConstantBuffer, 0, nullptr, &cb, 0, 0);

	mpD3DContext->VSSetShader(mpVertexShader, nullptr, 0);
	mpD3DContext->VSSetConstantBuffers(0, 1, &mpConstantBuffer);
	mpD3DContext->PSSetShader(mpPixelShader, nullptr, 0);

	mpD3DContext->DrawIndexed(mIndexCount, 0, 0);
}

void Ground::Release()
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

void Ground::InitIndexBuffer()
{
	WORD* indices = new WORD[mIndexCount];
	int index = 0;
	for (int z = 0; z <= mCol; ++z)
	{
		indices[index++] = (mCol + 1) * z;
		indices[index++] = (mCol + 1) * z + mCol;
	}
	for (int x = 0; x <= mRow; ++x)
	{
		indices[index++] = x;
		indices[index++] = x + (mRow + 1) * mCol;
	}

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