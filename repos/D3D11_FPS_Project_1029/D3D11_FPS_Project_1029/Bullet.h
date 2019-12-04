#pragma once

#include "Global.h"
#include "Object.h"

class Sound;
class Timer;

class Bullet : public Object
{
public:
	virtual bool Init(const ComPtr<ID3D11Device> pD3DDevice, const WCHAR* shaderFile, 
		const XMMATRIX projection, HWND hWnd, std::shared_ptr<Timer> timer);
	virtual void Update(const int index, const XMFLOAT3 gunPos, const XMMATRIX view);
	void Render(const int index);

	void Create(const int index, const XMFLOAT3& pos, const XMFLOAT3& rot);

	XMFLOAT3 GetPosition(const int index) const;
	bool GetLive(const int index) const;
	void SetLive(const int index, const bool bLive);

	Bullet();
	~Bullet();

private:
	std::shared_ptr<Timer> mTimer;

	XMFLOAT3 mPos[BULLET_COUNT];
	XMFLOAT3 mRot[BULLET_COUNT];

	bool mLive[BULLET_COUNT];

	XMMATRIX mWorld[BULLET_COUNT];
	XMMATRIX mView[BULLET_COUNT];

	std::unique_ptr<Sound> mShooting;
};

