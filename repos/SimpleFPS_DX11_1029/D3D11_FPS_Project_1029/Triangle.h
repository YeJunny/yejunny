#pragma once

#include "framework.h"

class Triangle
{
public:
	bool Init(ID3D11Device* pD3DDevice, ID3D11DeviceContext* pD3DContext);
	void Update();
	void Render();
	void Release();

private:
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pD3DContext;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pVertexBuffer;
};

