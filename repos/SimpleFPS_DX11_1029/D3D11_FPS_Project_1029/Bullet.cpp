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
		mVertices[i].Uv = (fbxLoader.GetUVs())[i];
	}

	mLive = false;

	mLayoutElementNumber = 2;
	mLayout.reset(new D3D11_INPUT_ELEMENT_DESC[mLayoutElementNumber]);
	mLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
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
		gunPos.x + crossFloat.x * 0.5f + angleFloat.x * 0.6f,
		gunPos.y - 0.3f,
		gunPos.z + crossFloat.z * 0.5f + angleFloat.z * 0.6f
	};

	mShooting->PlayWaveFile(gunPos);
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

void Bullet::Render()
{
	UINT stride = sizeof(VertexElements);
	UINT offset = 0;
	mD3DContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	mD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ConstantBuffer cb;
	cb.World = XMMatrixTranspose(mWorldMat);
	cb.View = XMMatrixTranspose(mViewMat);
	cb.Projection = XMMatrixTranspose(mProjectionMat);
	mD3DContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
	mD3DContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mD3DContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());
	mD3DContext->PSSetShaderResources(0, 1, mTextureRV.GetAddressOf());
	mD3DContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());
	mD3DContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

	mD3DContext->Draw(mVertexCount, 0);
}

XMFLOAT3 Bullet::GetPosition() const
{
	return mPos;
}

bool Bullet::GetLive() const
{
	return mLive;
}

void Bullet::SetLive(const bool bLive)
{
	mLive = bLive;
}