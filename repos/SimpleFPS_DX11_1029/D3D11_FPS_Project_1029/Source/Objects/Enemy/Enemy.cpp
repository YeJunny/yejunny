#include "Enemy.h"

Enemy::Enemy()
{
	mLayoutElementNumber = 5;
	mLayout.reset(new D3D11_INPUT_ELEMENT_DESC[mLayoutElementNumber]);
	mLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[3] = { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[4] = { "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

Enemy::~Enemy()
{

}

void Enemy::InitDetail()
{
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.CullMode = D3D11_CULL_FRONT;
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.FrontCounterClockwise = true;
	mRasterizerState.Reset();
	HRESULT hr = Global::GetDevice()->CreateRasterizerState(&cmdesc, mRasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		assert(hr == S_OK);
	}

	mWorldMat = XMMatrixScaling(0.03f, 0.03f, 0.03f) *
		XMMatrixRotationX(-XM_PI / 2) *
		XMMatrixTranslation(0, 3.0f, 0);
}

