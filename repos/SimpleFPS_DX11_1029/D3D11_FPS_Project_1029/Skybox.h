#pragma once

#include "Object.h"

class Timer;

class Skybox : public Object
{
public:
	virtual void InitDetail(HWND hWnd);

	Skybox();
	~Skybox();
};

