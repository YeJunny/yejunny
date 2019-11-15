#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#define MAX_BULLET 10

class Bullet;

class Player
{
public:
	virtual void OnInit(LPDIRECT3DDEVICE9 pd3dDevice);
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnRelease();

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	D3DXMATRIX m_matWorld;
	LPD3DXMESH m_pMesh;

	int m_health;
	D3DXVECTOR3 m_pos;
	float m_scal;
	float m_rotY;
	float m_speed;

	Bullet* m_pBullet[MAX_BULLET];
};

