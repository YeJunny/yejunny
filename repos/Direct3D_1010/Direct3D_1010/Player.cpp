#include "Bullet.h"
#include "Player.h"

void Player::OnInit(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_health = 10;
	m_scal = 0.5f;
	m_rotY = - 3.141592 / 2.0f;
	m_speed = 0.005f;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 1.5f);

	D3DXCreateTeapot(m_pd3dDevice, &m_pMesh, nullptr);
}

void Player::OnUpdate()
{
	static DWORD oldTime = 0;
	DWORD curTime = GetTickCount64();
	DWORD elapsedTime = curTime - oldTime;
	oldTime = curTime;

	if (GetAsyncKeyState(VK_LEFT) && (m_pos.x >= -2.5f))
	{
		m_pos.x -= elapsedTime * m_speed;
	}

	if (GetAsyncKeyState(VK_RIGHT) && (m_pos.x <= 2.5))
	{
		m_pos.x += elapsedTime * m_speed;
	}

	if (GetAsyncKeyState(VK_UP) && (m_pos.z <= 10.0f))
	{
		m_pos.z += elapsedTime * m_speed;
	}

	if (GetAsyncKeyState(VK_DOWN) && (m_pos.z >= 1.0f))
	{
		m_pos.z -= elapsedTime * m_speed;
	}

	if (GetAsyncKeyState(VK_SPACE) < 0)
	{
		static DWORD oldBulletFireTime = 0;
		DWORD diffTime = curTime - oldBulletFireTime;
		oldBulletFireTime = curTime;

		if (diffTime >= 60)
		{
			for (int i = 0; i < MAX_BULLET; ++i)
			{
				if (!m_pBullet[i])
				{
					m_pBullet[i] = new Bullet();
					m_pBullet[i]->Create(m_pd3dDevice, m_pos);
					break;
				}
			}
		}
	}

	for (int i = 0; i < MAX_BULLET; ++i)
	{
		if (m_pBullet[i])
		{
			D3DXVECTOR3 pos = m_pBullet[i]->GetPos();
			m_pBullet[i]->SetPos(pos.x, pos.y, pos.z + 0.5f);


			if (m_pBullet[i]->GetPos().z >= 15.0f)
			{
				delete m_pBullet[i];
				m_pBullet[i] = nullptr;
			}
		}
	}
}

void Player::OnRender()
{
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DXMATRIX matScaling, matRotationY, matTrans;
	D3DXMatrixScaling(&matScaling, m_scal, m_scal, m_scal);
	D3DXMatrixRotationY(&matRotationY, m_rotY);
	D3DXMatrixTranslation(&matTrans, m_pos.x, m_pos.y, m_pos.z);

	m_matWorld = matScaling * matRotationY * matTrans;
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pMesh->DrawSubset(0);

	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (m_pBullet[i])
		{
			m_pBullet[i]->OnRender();
		}
	}
}

void Player::OnRelease()
{
	if (m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	for (int i = 0; i < MAX_BULLET; i++)
	{
		delete m_pBullet[i];
		m_pBullet[i] = nullptr;
	}
}
