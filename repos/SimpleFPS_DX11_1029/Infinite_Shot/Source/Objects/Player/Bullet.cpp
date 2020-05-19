#include "Bullet.h"

Bullet::Bullet()
{
	mLayoutElementNumber = 3;
	mLayout.reset(new D3D11_INPUT_ELEMENT_DESC[mLayoutElementNumber]);
	mLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

Bullet::~Bullet()
{
}

void Bullet::InitDetail()
{
	mWorldMat = XMMatrixScaling(0.009f, 0.009f, 0.009f);
}

void Bullet::Create(const XMFLOAT3& gunPos)
{
	XMFLOAT3& playerRot = Global::GetPlayerRot();
	mRot =
	{
		XMConvertToRadians(playerRot.x),
		XMConvertToRadians(playerRot.y),
		XMConvertToRadians(playerRot.z)
	};

	XMVECTOR angleVector = { sinf(mRot.y), 1.0f, cosf(mRot.y) };
	XMVECTOR upVector = { 0.0f, 1.0f, 0.0f };
	XMVECTOR crossVector = XMVector3Cross(upVector, angleVector);
	XMFLOAT3 crossFloat;
	XMFLOAT3 angleFloat;
	XMStoreFloat3(&angleFloat, angleVector);
	XMStoreFloat3(&crossFloat, crossVector);

	mPos =
	{
		gunPos.x + crossFloat.x * 0.5f + angleFloat.x * 7.0f,
		gunPos.y + 0.2f,
		gunPos.z + crossFloat.z * 0.5f + angleFloat.z * 7.0f
	};

}

void Bullet::Update()
{
	float distance = 0.8f;

	mPos.x += distance * sinf(mRot.y);
	mPos.z += distance * cosf(mRot.y);

	mWorldMat = XMMatrixScaling(0.009f, 0.009f, 0.009f) *
		XMMatrixRotationRollPitchYaw(mRot.x, mRot.y, mRot.z) *
		XMMatrixTranslation(mPos.x, mPos.y, mPos.z);
}

XMFLOAT3 Bullet::GetPosition() const
{
	return mPos;
}