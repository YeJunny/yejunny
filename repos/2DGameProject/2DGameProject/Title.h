#pragma once

#include "Sequence.h"

class Image;

class Title
{
public:
	Title(const char* imageName);
	virtual ~Title();
	void Update(Sequence* parent);

private:
	Image* mTitle;
};

