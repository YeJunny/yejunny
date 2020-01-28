#include "Enemy.h"

Enemy::Enemy()
{
	mLayoutElementNumber = 3;
	mLayout.reset(new D3D11_INPUT_ELEMENT_DESC[mLayoutElementNumber]);
	mLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

Enemy::~Enemy()
{

}

void Enemy::InitDetail(HWND hWnd)
{
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.CullMode = D3D11_CULL_FRONT;
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.FrontCounterClockwise = true;
	mRasterizerState.Reset();
	HRESULT hr = mD3DDevice->CreateRasterizerState(&cmdesc, mRasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		assert(hr == S_OK);
	}

	mWorldMat = XMMatrixScaling(0.03f, 0.03f, 0.03f) *
		XMMatrixRotationX(- XM_PI / 2) *
		XMMatrixTranslation(0, 3.0f, 0);
}

