#include "Engine.h"
#include "SecondWindow.h"
#pragma comment(lib, "DirectXTK.lib")

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR		lpCmdLine,
	_In_ int		nCmdShow)
{
	Engine engine;
	SecondWindow secondWindow;

	HRESULT hr = engine.InitializeWindow(hInstance, nCmdShow, true, L"Infinite Shoot FPS!", L"InfiniteShotClass", 800, 600);
	AssertInitialization(hr, "Window Initialization");

	hr = engine.InitializeDirect3d11App(hInstance);
	AssertInitialization(hr, "Direct3D Initialization - Failed");

	hr = engine.InitScene();
	AssertInitialization(hr, "Initialize Scene - Failed");

	hr = engine.LoadResource();
	AssertInitialization(hr, "Load Resource - Failed");

	secondWindow.Init(engine.GetSecondHandler(), &engine);
	
	while (engine.ProcessMessage())
	{
		secondWindow.Update();
		secondWindow.Draw();
		engine.UpdateScene();
		engine.DrawScene();
	}

	engine.ReleaseObject();

	return 0;
}