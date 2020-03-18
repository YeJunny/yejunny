#pragma once

#include "../../Settings/Global.h"
#include "../Object.h"

class Sound;
class Time;

class Bullet : public Object
{
public:
	Bullet();
	~Bullet();

	virtual void Update() override;
	virtual void InitDetail() override;
	void Create(const XMFLOAT3& gunPos);
	XMFLOAT3 GetPosition() const;


private:
	XMFLOAT3 mPos;
	XMFLOAT3 mRot;
};

