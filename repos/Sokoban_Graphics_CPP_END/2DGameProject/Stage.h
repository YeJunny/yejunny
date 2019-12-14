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
	enum EFlag
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
	int mBoxCount;
	int mP;
	int mPreP; 
	double mNumO;
	double mNumP;
	char* mStage;
	EFlag* mFlagStage;
	int* mO;
	int* mPreO;
	int* mQ;
	Image* mImage;
	bool mbP;
	bool mbO;
};