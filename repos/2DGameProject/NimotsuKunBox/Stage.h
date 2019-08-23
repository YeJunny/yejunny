#pragma once

#define STR_LEN 256
#include <cstring>
#include <fstream>
#include "GameLib/Framework.h"

using namespace GameLib;
using namespace std;

class Stage
{
public:
	Stage();
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
	int* mDrawStage;
	int* mO;
	int* mQ;
};