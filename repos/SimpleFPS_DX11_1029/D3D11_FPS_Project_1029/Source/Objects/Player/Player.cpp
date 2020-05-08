#include "../../Hardware/Input.h"
#include "../../Hardware/Sound.h"
#include "Bullet.h"
#include "Player.h"

Player::Player()
	: mNumShootBullet(0)
{
}

Player::~Player()
{
	mShooting->Shutdown();
	mShooting.reset();
	mGun.reset();
	mBullet.reset();
}

void Player::Init()
{
	{
		mGun = std::make_unique<Gun>();
		WCHAR textureFiles[][TEXTURE_LEN] = { L"Fbx\\gun.jpg" };
		WCHAR shaderFiles[][TEXTURE_LEN] = 
		{ 
			L"Shader\\Ground.fx", 
			L"Shader\\Ground.fx" 
		};
		mGun->Init(shaderFiles, "Fbx\\Gun.fbx", textureFiles);
	}
	{
		mBullet.reset(new Bullet[BULLET_COUNT]);
		WCHAR textureFiles[][TEXTURE_LEN] = 
		{ 
			L"Fbx\\Bullet_Shell1.jpg", 
			L"Fbx\\Bullet_Shell2.jpg" 
		};
		WCHAR shaderFiles[][TEXTURE_LEN] = 
		{ 
			L"Shader\\Ground.fx", 
			L"Shader\\Ground.fx" 
		};
		for (int i = 0; i < BULLET_COUNT; ++i)
		{
			mBullet[i].Init(shaderFiles, "Fbx\\Bullet.fbx", textureFiles);
		}
	}

	mShooting = std::make_unique<Sound>();
	bool bSuccess = mShooting->Initialize("Sound\\Gun_Silencer.wav");
	assert(bSuccess);
}

void Player::Update()
{
	ULONGLONG diffTick = Global::GetTime()->GetDiffTick();

	float delay = diffTick * 0.01f;
	float speed = diffTick * 0.005f;
	float rotation = diffTick * 0.1f;

	static ULONGLONG accumulateTime = 0;
	static ULONGLONG gunDelayTime = 0;

	accumulateTime += diffTick;
	gunDelayTime += diffTick;


	mGun->Update();
	for (int i = 0; i < mNumShootBullet; i++)
	{
		mBullet[i].Update();
	}

	// Shoot
	std::unique_ptr<Input>& InputManager = Global::GetInput();
	InputManager->Frame();

	if (InputManager->IsLeftMouseButtonDown() && gunDelayTime >= 200.0f)
	{
		mBullet[mNumShootBullet++].Create(mGun->GetPosition());
		if (mNumShootBullet >= BULLET_COUNT)
		{
			mNumShootBullet = 0;
		}

		mShooting->PlayWaveFile();

		gunDelayTime = 0;
	}

	// Move
	float& playerRotY = Global::GetPlayerRot().y;
	float rad = XMConvertToRadians(playerRotY);
	XMFLOAT3& playerPos = Global::GetPlayerPos();

	if (GetAsyncKeyState('W') && accumulateTime >= delay)
	{
		playerPos.x += sinf(rad) * speed;
		playerPos.z += cosf(rad) * speed;

		accumulateTime = 0;
	}

	if (GetAsyncKeyState('S') && accumulateTime >= delay)
	{
		playerPos.x -= sinf(rad) * speed;
		playerPos.z -= cosf(rad) * speed;

		accumulateTime = 0;
	}

	rad = XMConvertToRadians(playerRotY + 90.0f);
	if (GetAsyncKeyState('D') && accumulateTime >= delay)
	{
		playerPos.x += sinf(rad) * speed;
		playerPos.z += cosf(rad) * speed;

		accumulateTime = 0;
	}

	if (GetAsyncKeyState('A') && accumulateTime >= delay)
	{
		playerPos.x -= sinf(rad) * speed;
		playerPos.z -= cosf(rad) * speed;

		accumulateTime = 0;
	}

	// Rotation
	if (GetAsyncKeyState('Q') && accumulateTime >= delay)
	{
		playerRotY -= rotation;

		if (playerRotY < 0.0f)
		{
			playerRotY += 360.0f;
		}

		accumulateTime = 0;
	}

	if (GetAsyncKeyState('E') && accumulateTime >= delay)
	{
		playerRotY += rotation;

		if (playerRotY > 360.0f)
		{
			playerRotY -= 360.0f;
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
