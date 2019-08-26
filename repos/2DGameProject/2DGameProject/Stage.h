#pragma once

#include "File.h"
#include "GameLib/Framework.h"
#include "Image.h"
#include <cstring>

using namespace GameLib;
using namespace std;

class Stage
{
public:
	enum EDraw
	{
		Player,
		Wall,
		Box,
		Target,
		Goal,
		Space
	};

	Stage(const char* stage, const char* image);
	virtual ~Stage();
	int Input();
	void Update(int move);
	void Draw() const;
	bool IsClear() const;

private:
	int mWidth;
	int mHeight;
	int mNumOfBox;
	int mP;
	char* mStage;
	EDraw* mDrawStage;
	int* mO;
	int* mQ;
	Image* mImage;
};