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
	DWORD m_dwNumMaterials;	// Material ����
	LPD3DXBUFFER m_pD3DMtrlBuffer;	// ����
	LPD3DXMESH m_pMesh;				// �޽�
	D3DMATERIAL9* m_pMeshMaterials;  // �޽� Materials
	LPDIRECT3DTEXTURE9* m_pMeshTexture;	// �޽� Texture
};

