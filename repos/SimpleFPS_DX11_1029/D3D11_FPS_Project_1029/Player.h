#pragma once

#include "framework.h"
#include "Gun.h"
#include "Object.h"
using namespace Microsoft::WRL;
using namespace DirectX;

class Bullet;
class Input;
class Timer;

class Player
{
public:
	virtual void Init(std::shared_ptr<Input> input, std::shared_ptr<Timer> timer);
	virtual void Update();
	virtual void Render();

	Player();
	~Player();

	XMFLOAT3 GetRotation() const;
	XMFLOAT3 GetPosition() const;
	bool* GetLiveBullet();
	void SetLiveBullet(const int index);

private:
	std::shared_ptr<Input> mInput;
	std::shared_ptr<Timer> mTimer;

	XMFLOAT3 mPos;
	XMFLOAT3 mRot;
	XMMATRIX mView;

	bool mbBullet[BULLET_COUNT];
};

