#pragma once

#include "Object.h"

class Timer;

class Cube : public Object
{
public:
	virtual void Update(const XMMATRIX view);

	Cube();
	~Cube();
};

