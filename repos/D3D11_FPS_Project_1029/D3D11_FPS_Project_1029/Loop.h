#pragma once
#include "D3DSetting.h"

class Bullet;
class Camera;
class Cube;
class Ground;
class Gun;
class Player;

class Loop : public D3DSetting
{
public:
	virtual bool Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	Loop();
	~Loop();

private:
	DWORD mFrame;

	std::unique_ptr<Ground> mGround;
	std::unique_ptr<Camera> mCamera;
	std::unique_ptr<Cube> mCube;
	std::unique_ptr<Player> mPlayer;
	std::unique_ptr<Gun> mGun;
	std::unique_ptr<Bullet> mBullet;
};

