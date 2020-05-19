#pragma once

#include "../Object.h"

class Enemy : public Object
{
public:
	Enemy();
	~Enemy();

	virtual void InitDetail() override;

};

