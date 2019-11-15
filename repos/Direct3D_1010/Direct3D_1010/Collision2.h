#pragma once

#include <d3d9.h>
#include <d3dx9.h>

struct BoxProperty
{
	D3DXVECTOR3 min;
	D3DXVECTOR3 max;
	D3DXVECTOR3 tran;
	FLOAT scal;
};

class Collision2
{
public:
	virtual void OnInit(LPDIRECT3DDEVICE9 pd3dDevice);
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnRelease();
	BOOL CheckBoxIntersect(D3DXVECTOR3& min1, D3DXVECTOR3& max1, D3DXVECTOR3& min2, D3DXVECTOR3& max2);

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPD3DXFONT m_pFont;

	BOOL m_bIsCollision;
	BoxProperty m_box[2];
	LPD3DXMESH m_pMesh;

	D3DXVECTOR3 m_min;
	D3DXVECTOR3 m_max;
};

