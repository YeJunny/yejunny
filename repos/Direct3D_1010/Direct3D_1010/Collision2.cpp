#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include "Collision2.h"
#include <stdio.h>

void Collision2::OnInit(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_bIsCollision = false;

	m_box[0].scal = 1.0f;
	m_box[0].tran = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
	m_box[1].scal = 1.5f;
	m_box[1].tran = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	D3DXCreateBox(m_pd3dDevice, 2.0f, 2.0f, 2.0f, &m_pMesh, nullptr);
	D3DXVECTOR3* pVertices;

	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertices);
	D3DXComputeBoundingBox(pVertices, m_pMesh->GetNumVertices(), m_pMesh->GetNumBytesPerVertex(), &m_min, &m_max);
	m_pMesh->UnlockVertexBuffer();

	D3DXMATRIX matWorld;
	D3DXMATRIX matScaling;

	for (int i = 0; i < 2; ++i)
	{
		D3DXMatrixTranslation(&matWorld, m_box[i].tran.x, m_box[i].tran.y, m_box[i].tran.z);
		D3DXMatrixScaling(&matScaling, m_box[i].scal, m_box[i].scal, m_box[i].scal);
		matWorld = matScaling * matWorld;
		D3DXVec3TransformCoord(&m_box[i].min, &m_min, &matWorld);
		D3DXVec3TransformCoord(&m_box[i].max, &m_max, &matWorld);
	}

	D3DXCreateFont(m_pd3dDevice, 20, 0, FW_NORMAL, 1, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("System"), &m_pFont);
}

void Collision2::OnUpdate()
{
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_box[0].tran.x += 0.1f;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_box[0].tran.x -= 0.1f;
	}

	D3DXMATRIX matWorld, matScaling;
	D3DXMatrixTranslation(&matWorld, m_box[0].tran.x, m_box[0].tran.y, m_box[0].tran.z);
	D3DXMatrixScaling(&matScaling, m_box[0].scal, m_box[0].scal, m_box[0].scal);
	matWorld = matScaling * matWorld;
	D3DXVec3TransformCoord(&m_box[0].min, &m_min, &matWorld);
	D3DXVec3TransformCoord(&m_box[0].max, &m_max, &matWorld);

	m_bIsCollision = CheckBoxIntersect(m_box[0].min, m_box[0].max, m_box[1].min, m_box[1].max);
}

void Collision2::OnRender()
{
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	D3DXMATRIX matWorld;
	D3DXMATRIX matScaling;

	for (int i = 0; i < 2; ++i)
	{
		D3DXMatrixTranslation(&matWorld, m_box[i].tran.x, m_box[i].tran.y, m_box[i].tran.z);
		D3DXMatrixScaling(&matScaling, m_box[i].scal, m_box[i].scal, m_box[i].scal);
		matWorld = matScaling * matWorld;
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_pMesh->DrawSubset(0);
	}

	///// print Collision info //////
	wchar_t string[100];
	RECT rt = { 10, 30, 0, 0 };

	swprintf(string, TEXT("Collision: %d"), m_bIsCollision);
	m_pFont->DrawText(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	///////////////////
}

void Collision2::OnRelease()
{
	if (m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	if (m_pFont)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

BOOL Collision2::CheckBoxIntersect(D3DXVECTOR3& min1, D3DXVECTOR3& max1, D3DXVECTOR3& min2, D3DXVECTOR3& max2)
{
	if (min1.x <= max2.x && max1.x >= min2.x &&
		min1.y <= max2.y && max1.y >= min2.y &&
		min1.z <= max2.z && max1.z >= min2.z)
	{
		return TRUE;
	}

	return FALSE;
}
