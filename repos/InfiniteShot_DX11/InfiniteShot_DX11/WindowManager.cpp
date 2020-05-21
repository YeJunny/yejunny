#include "WindowManager.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
		{
			if (MessageBox(0, L"Are you sure you want to exit?",
				L"Quit Game", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				DestroyWindow(hwnd);
			}
			exit(EXIT_SUCCESS);
		}
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		exit(EXIT_SUCCESS);
	}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

WindowManager::WindowManager()
	: mWidth(0)
	, mHeight(0)
	, mHwnd(null)
	, mWndClassName(nullptr)
{
	StartTime();
}

HRESULT WindowManager::InitializeWindow(HINSTANCE hInstance, int showWnd, bool bIswindowed, const TCHAR* titleName, const TCHAR* className, INT width, INT height)
{
	mWndClassName = className;
	mWidth = width;
	mHeight = height;

	typedef struct _WNDCLASS
	{
		UINT cbSize;
		UINT style;
		WNDPROC lpfnWndProc;
		int cbClsExtra;
		int cbWndExtra;
		HANDLE hInstance;
		HICON hIcon;
		HCURSOR hCursor;
		HBRUSH hbrBackground;
		LPCTSTR lpszMenuName;
		LPCTSTR lpszClassName;
		HICON hIconSm;
	} WNDCLASS;

	WNDCLASS wc;

	wc.cbSize = sizeof(WNDCLASS);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = null;
	wc.cbWndExtra = null;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(null, IDI_WINLOGO);
	wc.hCursor = LoadCursor(null, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = null;
	wc.lpszClassName = mWndClassName;
	wc.hIconSm = LoadIcon(null, IDI_APPLICATION);

	if (!RegisterClassEx((WNDCLASSEX*)&wc))
	{
		ErrorLogger::Log(E_FAIL, "Error registering class");
		exit(EXIT_FAILURE);
	}

	mHwnd = CreateWindowEx(
		null,
		mWndClassName,
		titleName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		null,
		null,
		hInstance,
		null
	);

	if (!mHwnd)
	{
		ErrorLogger::Log(E_FAIL, "Error creating window");
		exit(EXIT_FAILURE);
	}

	ShowWindow(mHwnd, showWnd);
	UpdateWindow(mHwnd);

	return S_OK;
}

bool WindowManager::ProcessMessage()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (PeekMessage(&msg, null, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		return false;
	}

	++mFrameCount;

	if (GetTime() > 1.0)
	{
		mFps = mFrameCount;
		mFrameCount = 0;
		StartTime();
	}

	mFrameTime = GetFrameTime();

	return true;
}

void WindowManager::StartTime()
{
	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);

	mCountsPerSecond = static_cast<double>(frequencyCount.QuadPart);

	QueryPerformanceCounter(&frequencyCount);
	mCounterStart = frequencyCount.QuadPart;
}

double WindowManager::GetTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return static_cast<double>((currentTime.QuadPart - mCounterStart) / mCountsPerSecond);
}

double WindowManager::GetFrameTime()
{
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart - mFrameTimeOld;
	mFrameTimeOld = currentTime.QuadPart;

	if (tickCount < 0)
	{
		tickCount = 0;
	}

	return static_cast<float>(tickCount) / mCountsPerSecond;
}