#pragma once

#include <d3d9.h>
#include <D3dx9math.h>

class Teapot
{
public:
	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice);
	void OnRender();
	void OnRelease();

private:
	LPD3DXMESH m_pTeapotMesh;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
};

