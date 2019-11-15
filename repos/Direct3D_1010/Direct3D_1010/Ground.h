#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#define D3DFVF_GROUNDVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct GROUNDVERTEX
{
	D3DXVECTOR3 pos;
	DWORD diffuse;
};

class Ground
{
public:
	virtual void Create(LPDIRECT3DDEVICE9 pd3dDevice, int nRom, int nCol, float fSize);
	virtual void OnRender();
	virtual void OnRelease();

private:
	DWORD m_numVertices, m_numIndices;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	int m_col, m_row;
};
