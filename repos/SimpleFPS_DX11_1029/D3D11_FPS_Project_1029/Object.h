#pragma once

#include "framework.h"
#include "Timer.h"
using namespace Microsoft::WRL;
using namespace DirectX;

struct VertexElements
{
	XMFLOAT3 Pos;
	XMFLOAT2 Uv;
};

struct ConstantBuffer
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;
};

class Object
{
public:
	virtual void Init(const ComPtr<ID3D11Device> pD3DDevice, HWND hWnd, 
		const WCHAR* shaderFile, const WCHAR* textureFile,
		const XMMATRIX projection, std::shared_ptr<Timer> timer);
	virtual void InitDetail(HWND hWnd);
	virtual void Update(const XMMATRIX view);
	virtual void Render();

	Object();
	~Object();

protected:
	// Shared Points
	ComPtr<ID3D11Device> mD3DDevice;
	ComPtr<ID3D11DeviceContext> mD3DContext;
	ComPtr<ID3D11RasterizerState> mRasterizerState;

	ComPtr<ID3D11VertexShader> mVertexShader;
	ComPtr<ID3D11PixelShader> mPixelShader;
	ComPtr<ID3D11InputLayout> mVertexLayout;

	ComPtr<ID3D11Buffer> mVertexBuffer;
	ComPtr<ID3D11Buffer> mConstantBuffer;

	std::unique_ptr<D3D11_INPUT_ELEMENT_DESC[]> mLayout;
	UINT mLayoutElementNumber;
	
	ComPtr<ID3D11Resource> mTexture2D;
	ComPtr<ID3D11ShaderResourceView> mTextureRV;
	ComPtr<ID3D11SamplerState> mSamplerState;

	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;

	UINT mVertexCount;
	std::unique_ptr<VertexElements[]> mVertices;
	std::shared_ptr<Timer> mTimer;
};

