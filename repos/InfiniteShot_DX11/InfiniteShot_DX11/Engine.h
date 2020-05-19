#pragma once

#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include "WindowManager.h"

#include "Object.h"

class Engine : public WindowManager
{
public:
	bool InitializeWindow(HINSTANCE hInstance, int showWnd, bool bIsWindowed, const TCHAR* titleName, const TCHAR* className, INT width, INT height);
	bool InitializeDirect3d11App(HINSTANCE hInstance);
	bool ProcessMessage();
	bool InitScene();
	void UpdateScene();
	void DrawScene();
	void ReleaseObject() const;

private:
	enum EObjectName
	{
		Triangle,
	};

	IDXGISwapChain* mSwapChain;
	ID3D11Device* mD3d11Device;
	ID3D11DeviceContext* mD3d11DevCon;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;

	std::vector<Object*> mObjects;
};

