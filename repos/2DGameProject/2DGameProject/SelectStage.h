#pragma once

#include "Sequence.h"

class Image;

class SelectStage
{
public:
	SelectStage(const char* imageName);
	virtual ~SelectStage();
	void Update(Sequence* parent);

private:
	Image* mSelect;
};

