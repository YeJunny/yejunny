#pragma once

#include "Object.h"

class Ground : public Object
{
public:
	Ground();
	~Ground();

	void Update(const XMMATRIX& viewMat) override;
};

