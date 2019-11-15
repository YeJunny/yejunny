#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include "Collision1.h"
#include <stdio.h>

void Collision1::OnInit(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_sphere[0].m_center = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	m_sphere[0].m_rad = 3.0f;
	m_sphere[0].m_scal = 1.0f;
	m_sphere[1].m_center = D3DXVECTOR3(5.0f, 0.0f, 0.0f);
	m_sphere[1].m_rad = 3.0f;
	m_sphere[1].m_scal = 1.5f;

	D3DXCreateSphere(m_pd3dDevice, m_sphere[0].m_rad, 20, 20, &m_pMesh, nullptr);

	D3DXCreateFont(m_pd3dDevice, 20, 0, FW_NORMAL, 1, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("System"), &m_pFont);
}

BOOL Collision1::CheckSphereIntersect(D3DXVECTOR3 center1, FLOAT radius1, D3DXVECTOR3 center2, FLOAT radius2)
{
	FLOAT distance;
	D3DXVECTOR3 diff;
	FLOAT rad = radius1 + radius2;
	diff = center2 - center1;

	distance = D3DXVec3Length(&diff);
	if (distance <= rad)
	{
		return TRUE;
	}
	
	return FALSE;
}

void Collision1::OnUpdate()
{
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_sphere[0].m_center.x += 0.1f;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_sphere[0].m_center.x -= 0.1f;
	}

	m_bIsColision = CheckSphereIntersect(m_sphere[0].m_center, m_sphere[0].m_rad * m_sphere[0].m_scal, 
		m_sphere[1].m_center, m_sphere[1].m_rad * m_sphere[1].m_scal);
}

void Collision1::OnRender()
{
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	D3DXMATRIX matWorld;
	D3DXMATRIX matScaling;

	for (int i = 0; i < 2; ++i)
	{
		D3DXMatrixTranslation(&matWorld, m_sphere[i].m_center.x, m_sphere[i].m_center.y, m_sphere[i].m_center.z);
		D3DXMatrixScaling(&matScaling, m_sphere[i].m_scal, m_sphere[i].m_scal, m_sphere[i].m_scal);
		matWorld = matScaling * matWorld;
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_pMesh->DrawSubset(0);
	}

	///// print Collision info //////
	wchar_t string[100];
	RECT rt = { 10, 30, 0, 0 };

	swprintf(string, TEXT("Collision: %d"), m_bIsColision);
	m_pFont->DrawText(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	///////////////////
}

void Collision1::OnRelease()
{
	if (m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
}