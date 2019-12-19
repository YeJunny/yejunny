#pragma once

#include "Global.h"
#include "Object.h"

class Sound;
class Timer;

class Bullet : public Object
{
public:
	Bullet();
	~Bullet();

	virtual void Update(const XMMATRIX& viewMat);

	virtual void InitDetail(HWND hWnd);
	void Create(const XMFLOAT3& pos, const XMFLOAT3& rot);
	XMFLOAT3 GetPosition() const;


private:
	std::shared_ptr<Timer> mTimer;

	XMFLOAT3 mPos;
	XMFLOAT3 mRot;
};

