#pragma once

#include "framework.h"
using namespace Microsoft::WRL;
using namespace DirectX;

class Input;

class D3DSetting
{
public:
	bool InitD3D_win(HINSTANCE hInst, HWND hwnd);
	void Update_win();
	void Render_win();
	void Release_win();

	virtual bool Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;

	D3DSetting();
	~D3DSetting();

protected:
	D3D_FEATURE_LEVEL* mFeatureLevel;
	ComPtr<IDXGISwapChain> mSwapChain;

	ComPtr<ID3D11Device> mpD3DDevice;
	ComPtr<ID3D11DeviceContext> mpD3DContext;

	ID3D11RenderTargetView* mpRenderTargetView;
	ID3D11Texture2D* mpDepthStencil;
	ID3D11DepthStencilView* mpDepthStencilView;

	std::shared_ptr<Input> mInput;
	HWND mhWnd;

	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;

	FLOAT mWidth;
	FLOAT mHeight;
};

