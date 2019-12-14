#include "FBXLoader.h"
#include "Global.h"
#include "Gun.h"

Gun::Gun()
{
	FBXLoader fbxLoader;
	fbxLoader.LoadFbx("Fbx\\UZI.fbx");

	mVertexCount = fbxLoader.GetVertexCount();
	assert(mVertexCount, L"Gun, mVertexCount == 0");
	mVertices.reset(new SimpleVertex[mVertexCount]);

	for (UINT i = 0; i < mVertexCount; ++i)
	{
		mVertices[i].Pos = (fbxLoader.GetVertices())[i];
		mVertices[i].Color = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	}

	mLayoutElementNumber = 2;
	mLayout.reset(new D3D11_INPUT_ELEMENT_DESC[mLayoutElementNumber]);
	mLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

Gun::~Gun()
{
	mVertices.reset();
}

void Gun::Update(const XMFLOAT3& playerPos, const XMFLOAT3& playerRot, const XMMATRIX& view)
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

	mPos = 
	{ 
		playerPos.x + crossFloat.x * 0.2f + angleFloat.x * 0.1f,
		playerPos.y - 0.3f,
		playerPos.z + crossFloat.z * 0.2f + angleFloat.z * 0.1f
	};

	mWorld = XMMatrixScaling(0.02f, 0.02f, 0.02f) *
		XMMatrixRotationRollPitchYaw(rotX, rotY + XM_PI, rotZ) *
		XMMatrixTranslation(mPos.x, mPos.y, mPos.z);

	mView = view;
}

XMFLOAT3 Gun::GetPosition() const
{
	return mPos;
}