#pragma once

#include "framework.h"
#include "Timer.h"
using namespace Microsoft::WRL;
using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
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
	virtual bool Init(const ComPtr<ID3D11Device> pD3DDevice, const WCHAR* shaderFile, 
		const XMMATRIX projection, std::shared_ptr<Timer> timer);
	virtual void InitIndexBuffer();
	virtual void InitDetail();
	virtual void Update(const XMMATRIX view);
	virtual void Render();
	virtual void Release();

	Object();

protected:
	std::shared_ptr<Timer> mTimer;

	ComPtr<ID3D11Device> mpD3DDevice;
	ComPtr<ID3D11DeviceContext> mpD3DContext;

	ID3D11VertexShader* mpVertexShader;
	ID3D11InputLayout* mpVertexLayout;
	ID3D11PixelShader* mpPixelShader;

	ID3D11Buffer* mpVertexBuffer;
	ID3D11Buffer* mpConstantBuffer;

	ID3D11RasterizerState* mpWireFrame;

	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;

	UINT mVertexCount;
	SimpleVertex* mVertices;
};

