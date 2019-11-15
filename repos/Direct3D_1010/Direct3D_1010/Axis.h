#pragma once

#include <d3d9.h>

class Axis
{
public:
	virtual void OnInit( LPDIRECT3DDEVICE9 pd3dDevice );
	virtual void OnRender();
	virtual void OnRelease();

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
};

