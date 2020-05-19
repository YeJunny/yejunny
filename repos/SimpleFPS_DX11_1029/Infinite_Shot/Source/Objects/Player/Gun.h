#pragma once

#include "../Object.h"

class Gun : public Object
{
public:
	Gun();
	~Gun();

	virtual void Update();
	XMFLOAT3 GetPosition() const;

private:
	XMFLOAT3 mPos;
};