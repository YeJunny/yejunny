#pragma once

#include "framework.h"
#include "Gun.h"
#include "Object.h"
using namespace Microsoft::WRL;
using namespace DirectX;

class Bullet;
class Input;
class Timer;

class Player : public Object
{
public:
	virtual bool Init(const ComPtr<ID3D11Device> pD3DDevice, const XMMATRIX projection, 
		std::shared_ptr<Input> input, std::shared_ptr<Timer> timer);
	virtual void Update();
	virtual void Render();
	virtual void Release();

	Player();

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

