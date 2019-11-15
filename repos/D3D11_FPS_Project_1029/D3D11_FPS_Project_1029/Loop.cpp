#include "Bullet.h"
#include "Camera.h"
#include "Cube.h"
#include "Ground.h"
#include "Gun.h"
#include "Loop.h"
#include "Player.h"

Loop::Loop()
	: mFrame(0)
{
	mCamera = std::make_unique<Camera>();
	mPlayer = std::make_unique<Player>();
	mCube = std::make_unique<Cube>();
	mGround = std::make_unique<Ground>();
}

bool Loop::Init()
{
	// Init Player
	if (!mPlayer->Init(mpD3DDevice, mProjection, mInput))
	{
		return false;
	}
	
	// Init Camera
	if (!mCamera->Init(mpD3DDevice, mView))
	{
		return false;
	}

	// Init Object
	if (!mCube->Init(mpD3DDevice, (WCHAR*)L"Shader\\Cube.fx", mProjection))
	{
		return false;
	}

	if (!mGround->Init(mpD3DDevice, (WCHAR*)L"Shader\\Cube.fx", mProjection))
	{
		return false;
	}

	mGun = std::make_unique<Gun>();
	if (!mGun->Init(mpD3DDevice, (WCHAR*)L"Shader\\Cube.fx", mProjection))
	{
		return false;
	}

	mBullet = std::make_unique<Bullet>();
	if (!mBullet->Init(mpD3DDevice, (WCHAR*)L"Shader\\Cube.fx", mProjection, mhWnd))
	{
		return false;
	}

	return true;
}

void Loop::Update()
{
	// Check FPS
	static ULONGLONG oldTick = GetTickCount64();
	static ULONGLONG fpsAccumulateTime = 0;
	ULONGLONG currentTick = GetTickCount64();

	ULONGLONG diffTick = currentTick - oldTick;
	oldTick = currentTick;
	fpsAccumulateTime += diffTick;

	if (fpsAccumulateTime >= 1000)
	{
		WCHAR str[100];
		wsprintf(str, L"FPS: %d\n", mFrame);
		mFrame = 0;
		fpsAccumulateTime = 0;
	}

	// Camera & Player update
	mPlayer->Update();

	XMFLOAT3 pos = mPlayer->GetPosition(), rot = mPlayer->GetRotation();
	mCamera->UpdateLocation(pos, rot);

	XMMATRIX view = mCamera->GetViewMatrix();

	// Object update
	mGun->Update(pos, rot, view);
	XMFLOAT3 gunPos = mGun->GetPosition();
	bool* bBullet = mPlayer->GetAliveBullet();
	for (int i = 0; i < BULLET_COUNT; ++i)
	{
		if (mBullet->GetLive(i))
		{
			mBullet->Update(i, gunPos, view);
			XMFLOAT3 pos = mBullet->GetPosition(i);
			if (pos.x >= 30.0f || pos.x <= -30.0f ||
				pos.z >= 30.0f || pos.z <= -30.0f)
			{
				mBullet->SetLive(i, false);
				mPlayer->SetAliveBullet(i);
			}
		}

		for (int j = 0; j < BULLET_COUNT; ++j)
		{
			if (bBullet[i] && !mBullet->GetLive(j))
			{
				mBullet->Create(j, gunPos, rot);
				mBullet->SetLive(j, true);
				mBullet->Update(j, gunPos, view);
				break;
			}
		}
	}

	mCube->Update(view);
	mGround->Update(view);
}

void Loop::Render()
{
	float color[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	mpD3DContext->ClearRenderTargetView(mpRenderTargetView, color);
	mpD3DContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	{
		// Object Rendering
		mGround->Render();
		mCube->Render();
		mGun->Render();
		
		for (int i = 0; i < BULLET_COUNT; ++i)
		{
			if (mBullet->GetLive(i))
			{
				mBullet->Render(i);
			}
		}
	}
	mSwapChain->Present(0, 0);

	mFrame++;
}

void Loop::Release()
{
	mCamera->Release();
	mCube->Release();
	mGround->Release();
	mGun->Release();
	mPlayer->Release();
}

Loop::~Loop()
{
	mCube.reset();
	mGround.reset();
	mCamera.reset();
	mPlayer.reset();
	mGun.reset();
	mBullet.reset();
}