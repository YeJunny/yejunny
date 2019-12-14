#pragma once

#include <iostream>
#include <string>

class Stage final
{
public:
	Stage(int width, int height, char* stage);
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
	char* mDrawStage;
	int* mO;
	int* mQ;
};