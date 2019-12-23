#pragma once
#include "DxSettings.h"

class Axis;
class Bullet;
class Camera;
class House;
class Skybox;
class Ground;
class Gun;
class Player;

class MainLoop : public DxSettings
{
public:
	MainLoop();
	~MainLoop();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	std::unique_ptr<Axis> mAxis;
	std::unique_ptr<House> mHouse;
	std::unique_ptr<Ground> mGround;
	std::unique_ptr<Camera> mCamera;
	std::unique_ptr<Skybox> mSkybox;
	std::unique_ptr<Player> mPlayer;
};

