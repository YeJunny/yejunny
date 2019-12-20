#pragma once

#include "framework.h"
#include "Gun.h"
#include "Object.h"
using namespace Microsoft::WRL;
using namespace DirectX;

class Bullet;
class Input;
class Sound;
class Timer;

class Player
{
public:
	Player();
	~Player();

	virtual void Init(const ComPtr<ID3D11Device> pD3DDevice, HWND hWnd, 
		const XMMATRIX& projectionMat,
		std::shared_ptr<Input> input, std::shared_ptr<Timer> timer);
	virtual void Update(const XMMATRIX& viewMat);
	virtual void Render();

	XMFLOAT3 GetRotation() const;
	XMFLOAT3 GetPosition() const;

private:
	std::shared_ptr<Input> mInput;
	std::shared_ptr<Timer> mTimer;

	XMFLOAT3 mPos;
	XMFLOAT3 mRot;

	size_t mNumShootBullet;

	std::unique_ptr<Gun> mGun;
	std::unique_ptr<Bullet[]> mBullet;
	std::unique_ptr<Sound> mShooting;
};

