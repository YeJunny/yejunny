#pragma once

#include "Gun.h"
#include "../Object.h"
using namespace Microsoft::WRL;
using namespace DirectX;

class Bullet;
class Input;
class Sound;
class Time;

class Player
{
public:
	Player();
	~Player();

	virtual void Init();
	virtual void Update();
	virtual void Render();

private:
	size_t mNumShootBullet;

	std::unique_ptr<Gun> mGun;
	std::unique_ptr<Bullet[]> mBullet;
	std::unique_ptr<Sound> mShooting;
};

