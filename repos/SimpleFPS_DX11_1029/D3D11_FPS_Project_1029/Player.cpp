#include "Bullet.h"
#include "Input.h"
#include "Player.h"

Player::Player()
	: mNumShootBullet(0)
{
	mPos = { 0.0f, 0.0f, 0.0f };
	mRot = { 0.0f, 0.0f, 0.0f };
}

Player::~Player()
{
	mTimer.reset();
	mInput.reset();
}

void Player::Init(const ComPtr<ID3D11Device> d3dDevice, HWND hWnd, const XMMATRIX& viewMat, const XMMATRIX& projectionMat, std::shared_ptr<Input> input, std::shared_ptr<Timer> timer)
{
	mInput = input;
	mTimer = timer;
	mViewMat = viewMat;

	mGun = std::make_unique<Gun>();
	mGun->Init(d3dDevice, hWnd, L"Shader\\Box.fx", L"Fbx\\gun.jpg", projectionMat, mTimer);

	mBullet.reset(new Bullet[BULLET_COUNT]);
	for (int i = 0; i < BULLET_COUNT; i++)
	{
		mBullet[i].Init(d3dDevice, hWnd, L"Shader\\Box.fx", L"Fbx\\Bullet_Shell.jpg", projectionMat, mTimer);
	}
}

void Player::Update(const XMMATRIX& viewMat)
{
	ULONGLONG diffTick = mTimer->GetDiffTick();
	
	float delay = diffTick * 0.01f;
	float speed = diffTick * 0.005f;
	float rotation = diffTick * 0.1f;

	static ULONGLONG accumulateTime = 0;
	static ULONGLONG gunDelayTime = 0;

	accumulateTime += diffTick;
	gunDelayTime += diffTick;


	mGun->Update(mPos, mRot, viewMat);
	for (int i = 0; i < mNumShootBullet; i++)
	{
		mBullet[i].Update(viewMat);
	}

	// Shoot
	mInput->Frame();
	if (mInput->IsLeftMouseButtonDown() && gunDelayTime >= 200.0f)
	{
		mBullet[mNumShootBullet++].Create(mGun->GetPosition(), mRot);
		if (mNumShootBullet >= BULLET_COUNT)
		{
			mNumShootBullet = 0;
		}

		gunDelayTime = 0;
	}

	// Move
	if (GetAsyncKeyState('W') && accumulateTime >= delay)
	{
		float rad = XMConvertToRadians(mRot.y);

		mPos.x += sinf(rad) * speed;
		mPos.z += cosf(rad) * speed;

		accumulateTime = 0;
	}

	if (GetAsyncKeyState('S') && accumulateTime >= delay)
	{
		float rad = XMConvertToRadians(mRot.y);

		mPos.x -= sinf(rad) * speed;
		mPos.z -= cosf(rad) * speed;

		accumulateTime = 0;
	}

	if (GetAsyncKeyState('D') && accumulateTime >= delay)
	{
		float rad = XMConvertToRadians(mRot.y + 90.0f);

		mPos.x += sinf(rad) * speed;
		mPos.z += cosf(rad) * speed;

		accumulateTime = 0;
	}

	if (GetAsyncKeyState('A') && accumulateTime >= delay) 
	{
		float rad = XMConvertToRadians(mRot.y + 90.0f);

		mPos.x -= sinf(rad) * speed;
		mPos.z -= cosf(rad) * speed;

		accumulateTime = 0;
	}

	// Rotation
	if (GetAsyncKeyState('Q') && accumulateTime >= delay)
	{
		mRot.y -= rotation;

		if (mRot.y < 0.0f)
		{
			mRot.y += 360.0f;
		}

		accumulateTime = 0;
	}

	if (GetAsyncKeyState('E') && accumulateTime >= delay)
	{
		mRot.y += rotation;

		if (mRot.y > 360.0f)
		{
			mRot.y -= 360.0f;
		}

		accumulateTime = 0;
	}
}

void Player::Render()
{
	mGun->Render();

	for (int i = 0; i < mNumShootBullet; ++i)
	{
		mBullet[i].Render();
	}
}


XMFLOAT3 Player::GetRotation() const
{
	return mRot;
}

XMFLOAT3 Player::GetPosition() const
{
	return mPos;
}
//
//bool* Player::GetLiveBullet()
//{
//	return mbBullet;
//}
//
//void Player::SetLiveBullet(const int index)
//{
//	mbBullet = false;
//}