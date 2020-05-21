#include "Engine.h"
#pragma comment(lib, "DirectXTK.lib")

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR		lpCmdLine,
	_In_ int		nCmdShow)
{
	Engine engine;

	HRESULT hr = engine.InitializeWindow(hInstance, nCmdShow, true, L"Infinite Shot!", L"InfiniteShotClass", 800, 600);
	
	AssertInitialization(hr, "Window Initialization");

	hr = engine.InitializeDirect3d11App(hInstance);
	
	AssertInitialization(hr, "Direct3D Initialization - Failed");

	hr = engine.InitScene();
	
	AssertInitialization(hr, "Initialize Scene = Failed");

	while (engine.ProcessMessage())
	{
		engine.UpdateScene();
		engine.DrawScene();
	}

	engine.ReleaseObject();

	return 0;
}