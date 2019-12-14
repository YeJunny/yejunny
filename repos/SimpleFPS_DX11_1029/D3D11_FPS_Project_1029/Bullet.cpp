#include "Bullet.h"
#include "FBXLoader.h"
#include "Global.h"
#include "Sound.h"

Bullet::Bullet()
{
	FBXLoader fbxLoader;
	fbxLoader.LoadFbx("Fbx\\bullet.fbx");

	mVertexCount = fbxLoader.GetVertexCount();
	assert(mVertexCount, L"Bullet, mVertexCount == 0");
	mVertices.reset(new VertexElements[mVertexCount]);
	for (UINT i = 0; i < mVertexCount; ++i)
	{
		mVertices[i].Pos = (fbxLoader.GetVertices())[i];
	}

	for (int i = 0; i < BULLET_COUNT; ++i)
	{
		mLive[i] = false;
	}

	mLayoutElementNumber = 2;
	mLayout.reset(new D3D11_INPUT_ELEMENT_DESC[mLayoutElementNumber]);
	mLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

Bullet::~Bullet()
{
	mShooting->Shutdown();
	mShooting.reset();
	mVertices.reset();
}

void Bullet::InitDetail(HWND hWnd)
{
	mShooting = std::make_unique<Sound>();
	bool bSuccess = mShooting->Initialize(hWnd, "Sound\\Gun_Silencer.wav");
	
	assert(bSuccess, "mShooting->Initiailize() error");
}

void Bullet::Create(const int index, const XMFLOAT3& gunPos, const XMFLOAT3& rot)
{
	mRot[index].x = XMConvertToRadians(rot.x);
	mRot[index].y = XMConvertToRadians(rot.y);
	mRot[index].z = XMConvertToRadians(rot.z);

	XMVECTOR angleVector = { sinf(mRot[index].y), 1.0f, cosf(mRot[index].y) };
	XMVECTOR upVector = { 0.0f, 1.0f, 0.0f };
	XMVECTOR crossVector = XMVector3Cross(upVector, angleVector);
	XMFLOAT3 crossFloat;
	XMFLOAT3 angleFloat;
	XMStoreFloat3(&angleFloat, angleVector);
	XMStoreFloat3(&crossFloat, crossVector);

	mPos[index] =
	{
		gunPos.x + crossFloat.x * 0.5f + angleFloat.x * 0.6f,
		gunPos.y - 0.3f,
		gunPos.z + crossFloat.z * 0.5f + angleFloat.z * 0.6f
	};

	mShooting->PlayWaveFile(gunPos);
}

void Bullet::Update(const int index, const XMFLOAT3 gunPos, const XMMATRIX view)
{
	float distance = 0.05f;

	mPos[index].x += distance * sinf(mRot[index].y);
	mPos[index].z += distance * cosf(mRot[index].y);

	mWorld[index] = XMMatrixScaling(0.01f, 0.01f, 0.01f) *
		XMMatrixRotationRollPitchYaw(mRot[index].x, mRot[index].y, mRot[index].z) *
		XMMatrixTranslation(mPos[index].x, mPos[index].y, mPos[index].z);

	mView[index] = view;
}

void Bullet::Render(const int index)
{
	UINT stride = sizeof(VertexElements);
	UINT offset = 0;
	mD3DContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	mD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ConstantBuffer cb;
	cb.World = XMMatrixTranspose(mWorld[index]);
	cb.View = XMMatrixTranspose(mView[index]);
	cb.Projection = XMMatrixTranspose(mProjection);

	mD3DContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);

	mD3DContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mD3DContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());
	mD3DContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

	mD3DContext->Draw(mVertexCount, 0);
}

XMFLOAT3 Bullet::GetPosition(const int index) const
{
	return mPos[index];
}

bool Bullet::GetLive(const int index) const
{
	return mLive[index];
}

void Bullet::SetLive(const int index, const bool bLive)
{
	mLive[index] = bLive;
}