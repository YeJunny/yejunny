#include "Timer.h"

Timer::Timer()
	: mCurrentTick(0)
	, mDiffTick(0)
{
	mOldTick = GetTickCount64();
}

void Timer::Update()
{
	mCurrentTick = GetTickCount64();

	mDiffTick = mCurrentTick - mOldTick;

	mOldTick = mCurrentTick;
}

ULONGLONG Timer::GetDiffTick() const
{
	return mDiffTick;
}