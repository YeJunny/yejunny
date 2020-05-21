#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d10_1.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "D2D1.lib")
#pragma comment(lib, "dwrite.lib")

#include <d3d11.h>
#include <d3d10_1.h>
#include <dxgi.h>
#include <d2d1.h>
#include <dwrite.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <sstream>
#include <vector>

#include "Object.h"
#include "WindowManager.h"

class Engine : public WindowManager
{
public:
	HRESULT InitializeWindow(HINSTANCE hInstance, int showWnd, bool bIsWindowed, const TCHAR* titleName, const TCHAR* className, INT width, INT height);
	HRESULT InitializeDirect3d11App(HINSTANCE hInstance);
	bool ProcessMessage();
	HRESULT InitScene();
	void UpdateScene();
	void DrawScene();
	void RenderText(std::wstring text, int inInt);
	void ReleaseObject();

public:
	ID3D11RasterizerState* GetCWcullMode() const { return mCWcullMode; }
	ID3D11RasterizerState* GetCCWcullMode() const { return mCCWcullMode; }
	ID3D11RasterizerState* GetNoCullMode() const { return mNoCullMode; }

private:
	HRESULT InitD2D_D3D101_DWrite(IDXGIAdapter1* adapter);
	void InitD2DScreenTexture();

private:
	enum EObjectName
	{
		Triangle,
	};

	// Drawing Text
	ID3D11BlendState* mTextTransparency = nullptr;
	ID3D11Buffer* mCBTextBuffer = nullptr;
	ID3D11SamplerState* mTextSamplerState = nullptr;
	ID3D10Device1* mD3d101Device = nullptr;
	IDXGIKeyedMutex* mKeyedMutex11 = nullptr;
	IDXGIKeyedMutex* mKeyedMutex10 = nullptr;
	ID2D1RenderTarget* mD2dRenderTarget = nullptr;
	ID2D1SolidColorBrush* mBrush = nullptr;
	ID3D11Texture2D* mBackbuffer11 = nullptr;
	ID3D11Texture2D* mSharedTex11 = nullptr;
	ID3D11Buffer* mD2dVertBuffer = nullptr;
	ID3D11Buffer* mD2dIndexBuffer = nullptr;
	ID3D11ShaderResourceView* mD2dTexture = nullptr;
	IDWriteFactory* mDWriteFactory = nullptr;
	IDWriteTextFormat* mTextFormat = nullptr;

	// Core
	IDXGISwapChain* mSwapChain = nullptr;
	ID3D11Device* mD3d11Device = nullptr;
	ID3D11DeviceContext* mD3d11DevCon = nullptr;
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ID3D11DepthStencilView* mDepthStencilView = nullptr;
	ID3D11RasterizerState* mCCWcullMode = nullptr;
	ID3D11RasterizerState* mCWcullMode = nullptr;
	ID3D11RasterizerState* mNoCullMode = nullptr;

	std::vector<Object*> mObjects;
};

