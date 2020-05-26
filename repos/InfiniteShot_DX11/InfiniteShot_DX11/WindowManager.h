#pragma once

#include <Windows.h>
#include "ErrorLogger.h"

#ifndef null
	#define null (NULL)
#endif

class WindowManager
{
public:
	WindowManager();
	HRESULT InitializeWindow(HINSTANCE hInstance, int showWnd, bool bIswindowed, const TCHAR* titleName, const TCHAR* className, INT width, INT height);
	bool ProcessMessage();
	double GetFps() const { return mFps; }

protected:
	void StartTime();
	double GetTime();
	double GetFrameTime();

protected:
	LPCTSTR mWndClassName;
	HWND mHwnd;
	HINSTANCE mInstance;
	int mWidth;
	int mHeight;

	// Time
	double mCountsPerSecond = 0.0;
	__int64 mCounterStart = 0;

	int mFrameCount = 0;
	int mFps = 0;

	__int64 mFrameTimeOld = 0;
	double mFrameTime = 0.0;
};

