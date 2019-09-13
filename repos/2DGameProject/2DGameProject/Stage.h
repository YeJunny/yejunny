#pragma once

#include <cstring>

class Image;
struct PreInput;

class Stage
{
public:
	Stage(const char* stage, const char* image);
	virtual ~Stage();
	int Input();
	void Update(int move);
	void Draw();
	bool IsClear() const;

private:
	enum EDraw
	{
		Player,
		Wall,
		Box,
		Target,
		Goal,
		Space
	};
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