#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Engine;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Color;
};

class SecondWindow
{
public:
	HRESULT Init(HWND hWnd, Engine* engine);

	HRESULT InitBuffer();

	void Update();
	void Draw();

private:
	ID3D11Device* mD3d11Device;
	ID3D11DeviceContext* mD3d11DevCon;
	IDXGISwapChain* mSwapChain;
	ID3D11RenderTargetView* mRenderTargetView;

	ID3D11Buffer* mVertBuffer;
	ID3D11Buffer* mIndexBuffer;
	
	ID3D11Buffer* mConstBuffer;

	ID3D11VertexShader* mVS;
	ID3D11PixelShader* mPS;
	ID3D11InputLayout* mInputLayout;

	ID3D11RasterizerState* mCWCullMode;
	ID3D11DepthStencilView* mDepthStencilView;

	// Core
	Engine* mEngine;
};

