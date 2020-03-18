#include "Skybox.h"

#include <fstream>

Skybox::Skybox()
{
	mLayoutElementNumber = 2;
	mLayout.reset(new D3D11_INPUT_ELEMENT_DESC[mLayoutElementNumber]);
	mLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

Skybox::~Skybox()
{
}

void Skybox::InitDetail()
{
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.CullMode = D3D11_CULL_NONE;
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.FrontCounterClockwise = true;
	mRasterizerState.Reset();
	HRESULT hr = Global::GetDevice()->CreateRasterizerState(&cmdesc, mRasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		assert(hr == S_OK);
	}

	mWorldMat = XMMatrixScaling(40.0f, 40.0f, 40.0f);
}
