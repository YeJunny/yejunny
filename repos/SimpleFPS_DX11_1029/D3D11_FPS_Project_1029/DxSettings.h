#pragma once

#include "framework.h"
using namespace Microsoft::WRL;
using namespace DirectX;

class Input;
class Timer;

class DxSettings
{
public:
	void InitDxSettings(HINSTANCE hInst, HWND hwnd);
	void UpdateDxSettings();
	void RenderDxSettings();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	DxSettings();
	~DxSettings();

protected:
	D3D_FEATURE_LEVEL* mFeatureLevel;
	ComPtr<IDXGISwapChain> mSwapChain;

	ComPtr<ID3D11Device> mpD3DDevice;
	ComPtr<ID3D11DeviceContext> mpD3DContext;

	ComPtr<ID3D11RenderTargetView> mpRenderTargetView;
	ComPtr<ID3D11Texture2D> mpDepthStencil;
	ComPtr<ID3D11DepthStencilView> mpDepthStencilView;

	std::shared_ptr<Input> mInput;
	std::shared_ptr<Timer> mTimer;

	HWND mhWnd;

	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;

	FLOAT mWidth;
	FLOAT mHeight;
};

