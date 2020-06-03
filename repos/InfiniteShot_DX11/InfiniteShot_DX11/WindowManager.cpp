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
		}
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		exit(S_OK);
	}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndProcSettings(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hX, hY, hZ;
	static HWND hScaleX, hScaleY, hScaleZ;
	static HWND hRotX, hRotY, hRotZ;

	switch (msg)
	{
	case WM_CREATE:
		hX = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL, 60, 130, 60, 25, hwnd, (HMENU)1, GetModuleHandle(nullptr), NULL);
		hY = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL, 120, 130, 60, 25, hwnd, (HMENU)2, GetModuleHandle(nullptr), NULL);
		hZ = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL, 180, 130, 60, 25, hwnd, (HMENU)3, GetModuleHandle(nullptr), NULL);
		hScaleX = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL, 60, 160, 60, 25, hwnd, (HMENU)4, GetModuleHandle(nullptr), NULL);
		hScaleY = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL, 120, 160, 60, 25, hwnd, (HMENU)5, GetModuleHandle(nullptr), NULL);
		hScaleZ = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL, 180, 160, 60, 25, hwnd, (HMENU)6, GetModuleHandle(nullptr), NULL);
		hRotX = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL, 60, 190, 60, 25, hwnd, (HMENU)7, GetModuleHandle(nullptr), NULL);
		hRotY = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL, 120, 190, 60, 25, hwnd, (HMENU)8, GetModuleHandle(nullptr), NULL);
		hRotZ = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL, 180, 190, 60, 25, hwnd, (HMENU)9, GetModuleHandle(nullptr), NULL);
		return 0;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		exit(S_OK);
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
	mInstance = hInstance;
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
	AssertIsSuccess(mHwnd, "Error creating window");

	ShowWindow(mHwnd, showWnd);
	UpdateWindow(mHwnd);


	const WCHAR settingsClassName[] = L"SettingClass";

	WNDCLASS wcSettings;

	wcSettings.cbSize = sizeof(WNDCLASS);
	wcSettings.style = CS_HREDRAW | CS_VREDRAW;
	wcSettings.cbClsExtra = null;
	wcSettings.cbWndExtra = null;
	wcSettings.hInstance = hInstance;
	wcSettings.hIcon = LoadIcon(null, IDI_WINLOGO);
	wcSettings.hCursor = LoadCursor(null, IDC_ARROW);
	wcSettings.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wcSettings.lpszMenuName = null;
	wcSettings.hIconSm = LoadIcon(null, IDI_APPLICATION);
	wcSettings.lpszClassName = settingsClassName;
	wcSettings.lpfnWndProc = WndProcSettings;

	if (!RegisterClassEx((WNDCLASSEX*)&wcSettings))
	{
		ErrorLogger::Log(E_FAIL, "Error registering class");
		exit(EXIT_FAILURE);
	}

	RECT mainRect;
	if (!GetWindowRect(mHwnd, &mainRect))
	{
		ErrorLogger::Log(E_FAIL, "Error Get Window Rect");
		exit(EXIT_FAILURE);
	}

	mHwndSetting = CreateWindowEx(
		null,
		settingsClassName,
		settingsClassName,
		WS_OVERLAPPEDWINDOW,
		mainRect.right - 5,
		mainRect.top,
		300,
		mHeight,
		null,
		null,
		hInstance,
		null
	);
	AssertIsSuccess(mHwndSetting, "Error creating window");

	ShowWindow(mHwndSetting, showWnd);
	UpdateWindow(mHwndSetting);
	

	//SetParent(mHwndSetting, mHwnd);


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