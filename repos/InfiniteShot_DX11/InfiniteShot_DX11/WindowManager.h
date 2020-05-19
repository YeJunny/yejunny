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
	bool InitializeWindow(HINSTANCE hInstance, int showWnd, bool bIswindowed, const TCHAR* titleName, const TCHAR* className, INT width, INT height);
	bool ProcessMessage();

protected:
	LPCTSTR mWndClassName;
	HWND mHwnd;
	int mWidth;
	int mHeight;
};

