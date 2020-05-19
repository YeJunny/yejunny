#pragma once

#include "../../Settings/Include/Framework.h"

class Time
{
public:
	Time();
	void Update();
	ULONGLONG GetDiffTick() const;

private:
	ULONGLONG mOldTick, mCurrentTick, mDiffTick;
};

