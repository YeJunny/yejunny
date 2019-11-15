#include "Bullet.h"
#include "Input.h"
#include "Player.h"

Player::Player()
{
	mPos = { 0.0f, 0.0f, 0.0f };
	mRot = { 0.0f, 0.0f, 0.0f };
}

bool Player::Init(const ComPtr<ID3D11Device> pD3DDevice, const XMMATRIX projection, std::shared_ptr<Input> input)
{
	mpD3DDevice = pD3DDevice;
	mpD3DDevice->GetImmediateContext(&mpD3DContext);
	mInput = input;

	return true;
}

void Player::Update()
{
	const float delay = 10.0f;
	const float speed = 0.05f;
	static ULONGLONG accumulateTime = 0;
	static ULONGLONG gunDelayTime = 0;
	static ULONGLONG oldTick = GetTickCount64();
	ULONGLONG currentTick = GetTickCount64();
	ULONGLONG diffTick = currentTick - oldTick;
	oldTick = currentTick;

	accumulateTime += diffTick;
	gunDelayTime += diffTick;

	// Shoot
	if (mInput->IsLeftMouseButtonDown() && gunDelayTime >= delay)
	{
		for (int i = 0; i < BULLET_COUNT; ++i)
		{
			if (!mbBullet[i])
			{
				mbBullet[i] = true;
				break;
			}
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
		mRot.y -= 2.0f;
		if (mRot.y < 0.0f)
		{
			mRot.y += 360.0f;
		}

		accumulateTime = 0;
	}

	if (GetAsyncKeyState('E') && accumulateTime >= delay)
	{
		mRot.y += 2.0f;
		if (mRot.y > 360.0f)
		{
			mRot.y -= 360.0f;
		}

		accumulateTime = 0;
	}
}

void Player::Render()
{
}

void Player::Release()
{
}

XMFLOAT3 Player::GetRotation()
{
	return mRot;
}

XMFLOAT3 Player::GetPosition()
{
	return mPos;
}

bool* Player::GetAliveBullet()
{
	return mbBullet;
}

void Player::SetAliveBullet(int index)
{
	mbBullet[index] = false;
}