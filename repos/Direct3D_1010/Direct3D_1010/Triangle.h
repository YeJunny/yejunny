#pragma once

#include <d3d9.h>

class Triangle
{
public:
	void OnInit( LPDIRECT3DDEVICE9 pd3dDevice );
	void OnRender();
	void OnRelease();

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DVERTEXBUFFER9 m_pNB;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
};

