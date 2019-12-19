#include "Bullet.h"
#include "FBXLoader.h"
#include "Global.h"

Bullet::Bullet()
{
	FBXLoader fbxLoader;
	fbxLoader.LoadFbx("Fbx\\Bullet.fbx");

	mVertexCount = fbxLoader.GetVertexCount();
	Assert(mVertexCount);
	mVertices.reset(new VertexElements[mVertexCount]);
	for (UINT i = 0; i < mVertexCount; ++i)
	{
		mVertices[i].Pos = (fbxLoader.GetVertices())[i];
		mVertices[i].Uv = (fbxLoader.GetUVs())[i];
	}

	mLayoutElementNumber = 2;
	mLayout.reset(new D3D11_INPUT_ELEMENT_DESC[mLayoutElementNumber]);
	mLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

Bullet::~Bullet()
{
	mVertices.reset();
}

void Bullet::InitDetail(HWND hWnd)
{
}

void Bullet::Create(const XMFLOAT3& gunPos, const XMFLOAT3& rot)
{
	mRot.x = XMConvertToRadians(rot.x);
	mRot.y = XMConvertToRadians(rot.y);
	mRot.z = XMConvertToRadians(rot.z);

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
		gunPos.y - 0.2f,
		gunPos.z + crossFloat.z * 0.5f + angleFloat.z * 7.0f
	};

}

void Bullet::Update(const XMMATRIX& viewMat)
{
	float distance = 0.8f;

	mPos.x += distance * sinf(mRot.y);
	mPos.z += distance * cosf(mRot.y);

	mWorldMat = XMMatrixScaling(0.009f, 0.009f, 0.009f) *
		XMMatrixRotationRollPitchYaw(mRot.x, mRot.y, mRot.z) *
		XMMatrixTranslation(mPos.x, mPos.y, mPos.z);

	mViewMat = viewMat;
}

XMFLOAT3 Bullet::GetPosition() const
{
	return mPos;
}