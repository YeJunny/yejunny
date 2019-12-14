#pragma once
#include "DxSettings.h"

class Bullet;
class Camera;
class Cube;
class Ground;
class Gun;
class Player;

class MainLoop : public DxSettings
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Render();

	MainLoop();
	~MainLoop();

private:
	DWORD mFrame;

	std::unique_ptr<Ground> mGround;
	std::unique_ptr<Camera> mCamera;
	std::unique_ptr<Cube> mCube;
	std::unique_ptr<Player> mPlayer;
	std::unique_ptr<Gun> mGun;
	std::unique_ptr<Bullet> mBullet;
};

