#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

class InputClass;
class GraphicsClass;

class SystemClass
{
public:
	bool Initialize();
	void Shutdown();
	void Run();

	HRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

	SystemClass();
	SystemClass(SystemClass&);
	~SystemClass();

private:
	bool Frame();
	void InitalizeWindows(int&, int&);
	void ShutdownWindows();

	HINSTANCE m_hInstance;
	HWND m_hwnd;
	LPCWSTR m_appName;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
};

HRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
static SystemClass* AppHandle = nullptr;
