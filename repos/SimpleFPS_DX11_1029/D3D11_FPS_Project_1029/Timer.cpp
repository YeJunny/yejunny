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
	WCHAR str[20];
	wsprintf(str, L"%d\n", mDiffTick);
	OutputDebugStringW(str);
	mOldTick = mCurrentTick;
}

ULONGLONG Timer::GetDiffTick() const
{
	return mDiffTick;
}