#pragma once

#include "../Object.h"

class Castle : public Object
{
public:
	Castle();
	~Castle();

	virtual void InitDetail() override;

};

