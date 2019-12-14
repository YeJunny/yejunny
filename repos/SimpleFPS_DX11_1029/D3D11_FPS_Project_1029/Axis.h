#pragma once

#include "framework.h"
using namespace Microsoft::WRL;
using namespace DirectX;

class Axis
{
public:
	bool Init(const ComPtr<ID3D11Device> pD3DDevice, const XMMATRIX projection);
	void Update(const XMMATRIX view);
	void Render();
	void Release();

private:
	ComPtr<ID3D11Device> m_pD3DDevice;
	ComPtr<ID3D11DeviceContext> m_pD3DContext;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pConstantBuffer;

	XMMATRIX m_world;
	XMMATRIX m_view;
	XMMATRIX m_projection;
};

