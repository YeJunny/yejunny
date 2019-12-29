#pragma once

#include "Object.h"

class House : public Object
{
public:
	House();
	~House();

	virtual void InitDetail(HWND hWnd) override;

};

