#pragma once

#include "framework.h"
using namespace Microsoft::WRL;
using namespace DirectX;

class Input;
class Timer;

class DxSettings
{
public:
	DxSettings();
	~DxSettings();

	void InitDxSettings(HINSTANCE hInst, HWND hwnd);
	void UpdateDxSettings();
	void RenderDxSettings();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	D3D_FEATURE_LEVEL* mFeatureLevel;
	ComPtr<IDXGISwapChain> mSwapChain;

	ComPtr<ID3D11Device> mD3DDevice;
	ComPtr<ID3D11DeviceContext> mD3DContext;

	ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	ComPtr<ID3D11Texture2D> mDepthStencil;
	ComPtr<ID3D11DepthStencilView> mDepthStencilView;

	std::shared_ptr<Input> mInput;
	std::shared_ptr<Timer> mTimer;

	HWND mhWnd;
	DWORD mFrame;

	XMMATRIX mViewMat;
	XMMATRIX mProjectionMat;

	FLOAT mWidth;
	FLOAT mHeight;
};