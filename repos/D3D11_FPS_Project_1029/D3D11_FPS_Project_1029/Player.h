#pragma once

#include "framework.h"
#include "Gun.h"
using namespace Microsoft::WRL;
using namespace DirectX;

class Bullet;
class Input;

class Player
{
public:
	virtual bool Init(const ComPtr<ID3D11Device> pD3DDevice, const XMMATRIX projection, std::shared_ptr<Input> input);
	virtual void Update();
	virtual void Render();
	virtual void Release();

	Player();

	XMFLOAT3 GetRotation();
	XMFLOAT3 GetPosition();
	bool* GetAliveBullet();
	void SetAliveBullet(int index);

private:
	ComPtr<ID3D11Device> mpD3DDevice;
	ComPtr<ID3D11DeviceContext> mpD3DContext;
	std::shared_ptr<Input> mInput;

	XMFLOAT3 mPos;
	XMFLOAT3 mRot;
	XMMATRIX mView;

	bool mbBullet[BULLET_COUNT];
};

