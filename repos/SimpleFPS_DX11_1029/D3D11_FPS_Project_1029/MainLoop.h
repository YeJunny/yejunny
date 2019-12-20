#pragma once
#include "DxSettings.h"

class Axis;
class Bullet;
class Camera;
class Skybox;
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
	std::unique_ptr<Axis> mAxis;
	std::unique_ptr<Ground> mGround;
	std::unique_ptr<Camera> mCamera;
	std::unique_ptr<Skybox> mSkybox;
	std::unique_ptr<Player> mPlayer;
};

