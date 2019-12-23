#include "FBXLoader.h"
#include "Global.h"
#include "Gun.h"

Gun::Gun()
{
	mLayoutElementNumber = 3;
	mLayout.reset(new D3D11_INPUT_ELEMENT_DESC[mLayoutElementNumber]);
	mLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

Gun::~Gun()
{
}

void Gun::Update(const XMFLOAT3& playerPos, const XMFLOAT3& playerRot, const XMMATRIX& viewMat)
{
	float rotX = XMConvertToRadians(playerRot.x);
	float rotY = XMConvertToRadians(playerRot.y);
	float rotZ = XMConvertToRadians(playerRot.z);

	XMVECTOR angleVector = {  sinf(rotY), 1.0f, cosf(rotY) };
	XMVECTOR upVector = { 0.0f, 1.0f, 0.0f };
	XMVECTOR crossVector = XMVector3Cross(upVector, angleVector);
	XMFLOAT3 crossFloat, angleFloat;
	XMStoreFloat3(&angleFloat, angleVector);
	XMStoreFloat3(&crossFloat, crossVector);

	static float xMoveRot = 0;
	xMoveRot += 0.01f;
	if (xMoveRot >= 2 * XM_PI)
	{
		xMoveRot = 0;
	}

	const float posCrossFactor = 0.4f;
	const float posAngleFactor = -4.3f;

	mPos = 
	{ 
		playerPos.x + crossFloat.x * posCrossFactor + angleFloat.x * posAngleFactor + 0.1f * sinf(xMoveRot),
		playerPos.y - 0.35f,
		playerPos.z + crossFloat.z * posCrossFactor + angleFloat.z * posAngleFactor + 0.05f * sinf(xMoveRot)
	};

	mWorldMat = XMMatrixScaling(0.05f, 0.05f, 0.05f) *
		XMMatrixRotationRollPitchYaw(rotX - XM_PI / 2, rotY, rotZ) *
		XMMatrixTranslation(mPos.x, mPos.y, mPos.z);

	mViewMat = viewMat;
}

XMFLOAT3 Gun::GetPosition() const
{
	return mPos;
}