#pragma once

#include "Global.h"
#include "Object.h"

class Sound;
class Timer;

class Bullet : public Object
{
public:
	virtual void InitDetail(HWND hWnd);
	virtual void Update(const XMMATRIX& viewMat);
	void Render();

	void Create(const XMFLOAT3& pos, const XMFLOAT3& rot);

	XMFLOAT3 GetPosition() const;
	bool GetLive() const;
	void SetLive(const bool bLive);

	Bullet();
	~Bullet();

private:
	std::shared_ptr<Timer> mTimer;

	XMFLOAT3 mPos;
	XMFLOAT3 mRot;

	bool mLive;

	std::unique_ptr<Sound> mShooting;
};

