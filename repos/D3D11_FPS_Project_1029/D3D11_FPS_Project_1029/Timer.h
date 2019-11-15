#pragma once

#include "framework.h"

class Timer
{
public:
	void Update();

	ULONGLONG GetDiffTick() const;

	Timer();

private:
	ULONGLONG mOldTick, mCurrentTick, mDiffTick;
};

