#include "Global.h"
#include "Ground.h"

Ground::Ground()
{
	mLayoutElementNumber = 3;
	mLayout.reset(new D3D11_INPUT_ELEMENT_DESC[mLayoutElementNumber]);
	mLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

Ground::~Ground()
{
}

void Ground::Update(const XMMATRIX& viewMat)
{
	static float radians = 0.0f;
	ULONGLONG diffTick = mTimer->GetDiffTick();
	radians += diffTick * 0.001f;
	if (radians >= XM_PI * 2)
	{
		radians = 0;
	}

	mWorldMat = XMMatrixScaling(0.4f, 0.4f, 0.4f);// *
		//XMMatrixRotationRollPitchYaw(0, radians, 0.0f) *
		//XMMatrixTranslation(10.0f, 0.0f, 10.0f);
	
	mViewMat = viewMat;
}