#include "Engine.h"
#pragma comment(lib, "DirectXTK.lib")

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR		lpCmdLine,
	_In_ int		nCmdShow)
{
	Engine engine;

	if (!engine.InitializeWindow(hInstance, nCmdShow, true, L"Infinite Shot!", L"InfiniteShotClass", 800, 600))
	{
		ErrorLogger::Log(E_FAIL, "Window Initialization");
		exit(EXIT_FAILURE);
	}

	if (!engine.InitializeDirect3d11App(hInstance))
	{
		ErrorLogger::Log(E_FAIL, "Direct3D Initialization - Failed");
		exit(EXIT_FAILURE);
	}

	if (!engine.InitScene())
	{
		ErrorLogger::Log(E_FAIL, "Initialize Scene = Failed");
		exit(EXIT_FAILURE);
	}

	while (engine.ProcessMessage())
	{
		engine.UpdateScene();
		engine.DrawScene();
	}

	engine.ReleaseObject();

	return 0;
}