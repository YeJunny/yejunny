#pragma once

#include <d3d9.h>

class D3DApp
{
public:
	HRESULT InitD3D(HWND hWnd);
	void Update();
	void Render();
	void Cleanup();

protected:
	virtual void OnInit() = 0;
	virtual void OnRender() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRelease() = 0;

	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	HWND				m_hWnd;
};

