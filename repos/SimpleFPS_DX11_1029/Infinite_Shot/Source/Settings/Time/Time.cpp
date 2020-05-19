#include "../../Settings/Time/Time.h"

Time::Time()
	: mCurrentTick(0)
	, mDiffTick(0)
{
	mOldTick = GetTickCount64();
}

void Time::Update()
{
	mCurrentTick = GetTickCount64();
	mDiffTick = mCurrentTick - mOldTick;
	WCHAR str[20];
	wsprintf(str, L"%d\n", mDiffTick);
	OutputDebugStringW(str);
	mOldTick = mCurrentTick;
}

ULONGLONG Time::GetDiffTick() const
{
	return mDiffTick;
}