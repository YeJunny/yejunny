#pragma once

#include <iostream>

class Bullet;
class Camera;
class Enemy;
class Castle;
class Skybox;
class Gun;
class Player;

class MainLoopManager
{
public:
	MainLoopManager();
	~MainLoopManager();

	void Init();
	void Update();
	void Render();

private:
	std::unique_ptr<Castle> mCastle;
	std::unique_ptr<Camera> mCamera;
	std::unique_ptr<Skybox> mSkybox;
	std::unique_ptr<Player> mPlayer;
	std::unique_ptr<Enemy> mEnemy;
};

