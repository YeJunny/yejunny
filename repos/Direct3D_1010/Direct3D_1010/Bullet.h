#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class Bullet
{
public:
	void Create(LPDIRECT3DDEVICE9 pd3dDevice, D3DXVECTOR3 pos);
	void OnRender();

	D3DXVECTOR3 GetPos();
	void SetPos(float x, float y, float z);
	~Bullet();

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPD3DXMESH m_pMesh;

	D3DXVECTOR3 m_pos;
	float m_speed;
	float m_scal;
};

