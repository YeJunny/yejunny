#pragma once

#include "../Object.h"

class Skybox : public Object
{
public:
	Skybox();
	~Skybox();

	virtual void InitDetail() override;
};

