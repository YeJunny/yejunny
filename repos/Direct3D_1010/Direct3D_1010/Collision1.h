#pragma once

#include <d3d9.h>
#include <d3dx9.h>

struct SphereProperty
{
	FLOAT m_rad;
	FLOAT m_scal;
	D3DXVECTOR3 m_center;
};

class Collision1
{
public:
	virtual void OnInit(LPDIRECT3DDEVICE9 pd3dDevice);
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnRelease();
	BOOL CheckSphereIntersect(D3DXVECTOR3 center1, FLOAT radius1, D3DXVECTOR3 center2, FLOAT radius2);

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPD3DXMESH m_pMesh;
	SphereProperty m_sphere[2];
	BOOL m_bIsColision;

	LPD3DXFONT m_pFont;
};

