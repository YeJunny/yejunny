#include "Bullet.h"
#include "FBXLoader.h"
#include "Global.h"
#include "Sound.h"

Bullet::Bullet()
{
	FBXLoader fbxLoader;
	fbxLoader.LoadFbx("Fbx\\bullet.fbx");

	mVertexCount = fbxLoader.GetVertexCount();
	mVertices = new SimpleVertex[mVertexCount];
	for (UINT i = 0; i < mVertexCount; ++i)
	{
		mVertices[i].Pos = (fbxLoader.GetVertices())[i];
		mVertices[i].Color = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	}

	for (int i = 0; i < BULLET_COUNT; ++i)
	{
		mLive[i] = false;
	}
}

Bullet::~Bullet()
{
	mShooting->Shutdown();
	mShooting.reset();
}

bool Bullet::Init(const ComPtr<ID3D11Device> pD3DDevice, const WCHAR* shaderFile, const XMMATRIX projection, HWND hWnd)
{
	mProjection = projection;
	mpD3DDevice = pD3DDevice;
	mpD3DDevice->GetImmediateContext(&mpD3DContext);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * mVertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = mVertices;
	if (FAILED(mpD3DDevice->CreateBuffer(&bd, &InitData, &mpVertexBuffer)))
	{
		return false;
	}

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	if (FAILED(mpD3DDevice->CreateBuffer(&bd, NULL, &mpConstantBuffer)))
	{
		return false;
	}

	// SET VS
	ID3DBlob* pVSBlob = nullptr;
	if (FAILED(Global::CompileShaderFromFile(shaderFile, "VS", "vs_4_0", &pVSBlob)))
	{
		return false;
	}
	if (FAILED(mpD3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &mpVertexShader)))
	{
		pVSBlob->Release();
		return false;
	}


	// SET LAYOUT
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	if (FAILED(mpD3DDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &mpVertexLayout)))
	{
		return false;
	}
	pVSBlob->Release();
	mpD3DContext->IASetInputLayout(mpVertexLayout);


	// SET PS
	ID3DBlob* pPSBlob = nullptr;
	if (FAILED(Global::CompileShaderFromFile(shaderFile, "PS", "ps_4_0", &pPSBlob)))
	{
		return false;
	}
	if (FAILED(mpD3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mpPixelShader)))
	{
		pPSBlob->Release();
		return false;
	}
	pPSBlob->Release();


	// SET SOUND
	mShooting = std::make_unique<Sound>();
	if (!mShooting->Initialize(hWnd, "Sound\\Gun_Silencer.wav"))
	{
		return false;
	}

	return true;
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
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	mpD3DContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
	mpD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ConstantBuffer cb;
	cb.World = XMMatrixTranspose(mWorld[index]);
	cb.View = XMMatrixTranspose(mView[index]);
	cb.Projection = XMMatrixTranspose(mProjection);

	mpD3DContext->UpdateSubresource(mpConstantBuffer, 0, nullptr, &cb, 0, 0);

	mpD3DContext->VSSetShader(mpVertexShader, nullptr, 0);
	mpD3DContext->VSSetConstantBuffers(0, 1, &mpConstantBuffer);
	mpD3DContext->PSSetShader(mpPixelShader, nullptr, 0);

	mpD3DContext->Draw(mVertexCount, 0);
}

XMFLOAT3 Bullet::GetPosition(const int index)
{
	return mPos[index];
}

bool Bullet::GetLive(const int index)
{
	return mLive[index];
}

void Bullet::SetLive(const int index, bool bLive)
{
	mLive[index] = bLive;
}