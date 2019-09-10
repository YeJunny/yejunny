#pragma once

#include "GameLib/Framework.h"
#include <cstring>

using namespace GameLib;
using namespace std;

class Image;
struct PreInput;

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
	void Draw(unsigned int diffTime);
	void DrawTile(int x, int y, int pos, int tileSize) const;
	bool IsClear() const;

private:
	int mWidth;
	int mHeight;
	int mNumOfBox;
	int mP;
	int mPreP;
	double mCountOfO;
	double mCountOfP;
	char* mStage;
	EDraw* mDrawStage;
	int* mO;
	int* mPreO;
	int* mQ;
	Image* mImage;
	bool mDrawP;
	bool mDrawO;
};