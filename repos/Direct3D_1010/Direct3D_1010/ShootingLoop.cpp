#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include "Ground.h"
#include "Player.h"
#include "ShootingLoop.h"
#include <d3dx9.h>
#include <stdio.h>

void ShootingLoop::OnInit()
{
	m_pGround = new Ground();
	m_pPlayer = new Player();
	m_nFPSCount = 0;

	// set camera
	m_Eye.x = 0.0f;
	m_Eye.y = 5.0f;
	m_Eye.z = -7.0f;

	m_At.x = 0.0f;
	m_At.y = 0.0f;
	m_At.z = 20.0f;

	m_Up.x = 0.0f;
	m_Up.y = 1.0f;
	m_Up.z = 0.0f;


	// set view-port
	RECT rect;
	D3DVIEWPORT9 vp;
	GetClientRect(m_hWnd, &rect);

	vp.X = 0;
	vp.Y = 0;
	vp.Width = rect.right - rect.left;
	vp.Height = rect.bottom - rect.top;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;


	// set view, projection, view-port trans
	D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up);
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	m_pd3dDevice->SetViewport(&vp);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);


	// initailize
	m_pGround->Create(m_pd3dDevice, 30, 5, 1.0f);
	m_pPlayer->OnInit(m_pd3dDevice);

	// font
	D3DXCreateFont(m_pd3dDevice, 20, 0, FW_NORMAL, 1, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("System"), &m_pFont);
}

void ShootingLoop::OnRender()
{
	///// Calculate FPS /////
	wchar_t string[100];
	RECT rt = { 10, 10, 0, 0 };

	swprintf(string, TEXT("FPS: %d"), m_nFPS);
	m_pFont->DrawText(NULL, string, -1, &rt,
		DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_nFPSCount++;
	/////////////////////////

	m_pGround->OnRender();
	m_pPlayer->OnRender();

	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void ShootingLoop::OnUpdate()
{
	///// Print FPS //////
	DWORD dwCurrentTime = GetTickCount64();
	static DWORD dwOldTime = GetTickCount64();
	static DWORD dwAccumulateTime = 0;

	m_dwElapsedTime = dwCurrentTime - dwOldTime;
	dwOldTime = dwCurrentTime;

	dwAccumulateTime += m_dwElapsedTime;
	if (dwAccumulateTime >= 1000)
	{
		m_nFPS = m_nFPSCount;
		m_nFPSCount = 0;
		dwAccumulateTime = 0;
	}
	////////////////////////

	m_pPlayer->OnUpdate();
}

void ShootingLoop::OnRelease()
{
	m_pGround->OnRelease();
	m_pPlayer->OnRelease();
}