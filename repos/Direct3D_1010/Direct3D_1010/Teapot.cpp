#include "Teapot.h"

void Teapot::OnInit(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	D3DXCreateTeapot(m_pd3dDevice, &m_pTeapotMesh, NULL);
}

void Teapot::OnRender()
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DXMATRIX matRotationY, matWorld, matTrans, matScaling;
	float fScaling[3] = { 0.3f, 0.6f, 1.0f };
	D3DXVECTOR3 vTrans[3] = { D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								D3DXVECTOR3(2.0f, 0.0f, 0.0f),
								D3DXVECTOR3(5.0f, 0.0f, 0.0f) };
	D3DXMatrixRotationY(&matRotationY, GetTickCount64() * 0.004f);

	// 첫번째 티포트
	D3DXMatrixScaling(&matScaling, fScaling[0], fScaling[0], fScaling[0]);
	D3DXMatrixTranslation(&matTrans, vTrans[0].x, vTrans[0].y, vTrans[0].z);
	matWorld = matScaling * matTrans * matRotationY;
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pTeapotMesh->DrawSubset(0);

	// 두번째 티포트
	D3DXMatrixScaling(&matScaling, fScaling[1], fScaling[1], fScaling[1]);
	D3DXMatrixTranslation(&matTrans, vTrans[1].x, vTrans[1].y, vTrans[1].z);
	matWorld = matScaling * matRotationY * matTrans;
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pTeapotMesh->DrawSubset(0);

	// 세번째 티포트
	D3DXMatrixScaling(&matScaling, fScaling[2], fScaling[2], fScaling[2]);
	D3DXMatrixTranslation(&matTrans, vTrans[2].x, vTrans[2].y, vTrans[2].z);
	matWorld = matScaling * matTrans * matRotationY;
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pTeapotMesh->DrawSubset(0);

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void Teapot::OnRelease()
{
	if (m_pTeapotMesh)
	{
		m_pTeapotMesh->Release();
		m_pTeapotMesh = NULL;
	}
}
