#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class Mesh
{
public:
	virtual void OnInit(LPDIRECT3DDEVICE9 pd3dDevice);
	virtual void OnRender();
	virtual void OnRelease();

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice; 
	DWORD m_dwNumMaterials;	// Material 갯수
	LPD3DXBUFFER m_pD3DMtrlBuffer;	// 버퍼
	LPD3DXMESH m_pMesh;				// 메쉬
	D3DMATERIAL9* m_pMeshMaterials;  // 메쉬 Materials
	LPDIRECT3DTEXTURE9* m_pMeshTexture;	// 메쉬 Texture
};

