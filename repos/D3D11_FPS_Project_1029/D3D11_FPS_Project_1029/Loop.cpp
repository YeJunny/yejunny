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
	if (!mPlayer->Init(mpD3DDevice, mProjection, mInput, mTimer))
	{
		return false;
	}
	
	// Init Camera
	if (!mCamera->Init(mpD3DDevice, mView))
	{
		return false;
	}

	// Init Object
	if (!mCube->Init(mpD3DDevice, L"Shader\\Cube.fx", mProjection, mTimer))
	{
		return false;
	}

	if (!mGround->Init(mpD3DDevice, L"Shader\\Cube.fx", mProjection, mTimer))
	{
		return false;
	}

	mGun = std::make_unique<Gun>();
	if (!mGun->Init(mpD3DDevice, L"Shader\\Cube.fx", mProjection, mTimer))
	{
		return false;
	}

	mBullet = std::make_unique<Bullet>();
	if (!mBullet->Init(mpD3DDevice, L"Shader\\Cube.fx", mProjection, mhWnd, mTimer))
	{
		return false;
	}

	return true;
}

void Loop::Update()
{
	// Calculate FPS
	static ULONGLONG cumulativeTime = 0;
	ULONGLONG diffTick = mTimer->GetDiffTick();
	cumulativeTime += diffTick;
	if (cumulativeTime >= 1000)
	{
		WCHAR str[100];
		wsprintf(str, L"FPS: %d\n", mFrame);
		mFrame = 0;
		cumulativeTime = 0;
	}


	// Player update
	mPlayer->Update();
	XMFLOAT3 playerPos = mPlayer->GetPosition();
	XMFLOAT3 playerRot = mPlayer->GetRotation();


	// Camera update
	mCamera->UpdateLocation(playerPos, playerRot);
	XMMATRIX view = mCamera->GetViewMatrix();


	// Object update
	mGun->Update(playerPos, playerRot, view);
	XMFLOAT3 gunPos = mGun->GetPosition();
	bool* bBullet = mPlayer->GetLiveBullet();
	for (int i = 0; i < BULLET_COUNT; ++i)
	{
		if (mBullet->GetLive(i))
		{
			mBullet->Update(i, gunPos, view);
			XMFLOAT3 bulletPos = mBullet->GetPosition(i);

			if (bulletPos.x >= 30.0f || bulletPos.x <= -30.0f ||
				bulletPos.z >= 30.0f || bulletPos.z <= -30.0f)
			{
				mBullet->SetLive(i, false);

				mPlayer->SetLiveBullet(i);
			}
		}

		for (int j = 0; j < BULLET_COUNT; ++j)
		{
			if (bBullet[i] && !mBullet->GetLive(j))
			{
				mBullet->Create(j, gunPos, playerRot);
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
	float backgroundColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	mpD3DContext->ClearRenderTargetView(mpRenderTargetView, backgroundColor);
	mpD3DContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	{
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