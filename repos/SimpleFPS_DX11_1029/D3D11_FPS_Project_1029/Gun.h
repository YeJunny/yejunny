#pragma once

#include "Object.h"

class Gun : public Object
{
public:
	virtual void Update(const XMFLOAT3& pos, const XMFLOAT3& rot, const XMMATRIX& viewMat);
	XMFLOAT3 GetPosition() const;

	Gun();
	~Gun();

private:
	XMFLOAT3 mPos;
};