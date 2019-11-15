#pragma once

#include <d3d9.h>
#include <d3dx9math.h>

class Texture
{
public:
	virtual void OnInit(LPDIRECT3DDEVICE9 pd3dDevice);
	virtual void OnRender();
	virtual void OnRelease();

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DTEXTURE9 m_pTexture;

	D3DXMATRIX m_matScale[2];
	D3DXMATRIX m_matTranslate[2];
};

