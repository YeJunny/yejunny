#pragma once

#include "Object.h"

class Timer;

class Skybox : public Object
{
public:
	virtual void Update(const XMMATRIX& viewMat);
	virtual void InitDetail(HWND hWnd);

	Skybox();
	~Skybox();
};

