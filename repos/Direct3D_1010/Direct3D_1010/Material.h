#pragma once

#include <d3d9.h>

class Material
{
public:
	void OnInit( LPDIRECT3DDEVICE9 pd3dDevice );
	void OnRender();
	void OnUpdate();
	void OnRelease();

	
	//void SetMaterial(float r, float g, float b);
	//void SetDirectionalLight();

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	D3DMATERIAL9 m_Material;
	D3DLIGHT9 m_Light;
};

