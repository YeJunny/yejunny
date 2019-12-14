#pragma once

#include "framework.h"
#include "Timer.h"
using namespace Microsoft::WRL;
using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
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
	virtual void Init(const ComPtr<ID3D11Device> pD3DDevice, HWND hWnd, const WCHAR* shaderFile,
		const XMMATRIX projection, std::shared_ptr<Timer> timer);
	virtual void InitDetail(HWND hWnd);
	virtual void Update(const XMMATRIX view);
	virtual void Render();

	Object();
	~Object();

protected:
	// Shared Points
	ComPtr<ID3D11Device> mpD3DDevice;
	ComPtr<ID3D11DeviceContext> mpD3DContext;

	ComPtr<ID3D11VertexShader> mpVertexShader;
	ComPtr<ID3D11PixelShader> mpPixelShader;
	ComPtr<ID3D11InputLayout> mpVertexLayout;

	ComPtr<ID3D11Buffer> mpVertexBuffer;
	ComPtr<ID3D11Buffer> mpConstantBuffer;

	std::unique_ptr<D3D11_INPUT_ELEMENT_DESC[]> mLayout;
	UINT mLayoutElementNumber;
	
	ComPtr<ID3D11ShaderResourceView> mpTextureRV;
	ComPtr<ID3D11SamplerState> mpSamplerState;

	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;

	UINT mVertexCount;
	std::unique_ptr<SimpleVertex[]> mVertices;
	std::shared_ptr<Timer> mTimer;
};

