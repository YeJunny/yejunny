#pragma once

#include "Object.h"

class House : public Object
{
public:
	House();
	~House();

	void InitDetail(HWND hWnd) override;

};

