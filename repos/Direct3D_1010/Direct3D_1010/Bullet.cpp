#include "Bullet.h"

void Bullet::Create(LPDIRECT3DDEVICE9 pd3dDevice, D3DXVECTOR3 pos)
{
	m_pd3dDevice = pd3dDevice;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 1.5f);
	m_speed = 1.5f;
	m_scal = 0.7f;
	m_pos = pos;

	D3DXCreateCylinder(m_pd3dDevice, 0.2f, 0.2f, 0.2f, 20, 10, &m_pMesh, nullptr);
}

void Bullet::OnRender()
{
	D3DXMATRIX matWorld, matScaling, matTrans;
	D3DXMatrixTranslation(&matTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixScaling(&matScaling, m_scal, m_scal, m_scal);
	matWorld = matScaling * matTrans;
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pMesh->DrawSubset(0);
}

D3DXVECTOR3 Bullet::GetPos()
{
	return m_pos;
}

void Bullet::SetPos(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}

Bullet::~Bullet()
{
	if (m_pMesh)
	{
		m_pMesh->Release();
	}
}
