#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include "Axis.h"
#include "Collision2.h"
#include "GameLoop.h"
#include <d3dx9.h>
#include <stdio.h>

GameLoop::~GameLoop()
{
	delete m_pAxis;
	delete m_pCollision2;
}

void GameLoop::OnInit()
{
	m_pAxis = new Axis();
	m_pCollision2 = new Collision2();

	// 멤버변수 초기화
	m_Eye.x = 0.0f;
	m_Eye.y = 5.0;
	m_Eye.z = -20.0f;

	m_At.x = 0.0f;
	m_At.y = 0.0f;
	m_At.z = 0.0f;

	m_Up.x = 0.0f;
	m_Up.y = 1.0f;
	m_Up.z = 0.0f;


	// 뷰 포트 구성
	RECT rect;
	D3DVIEWPORT9 vp;
	GetClientRect( m_hWnd, &rect );

	vp.X = 0;
	vp.Y = 0;
	vp.Width = rect.right - rect.left;
	vp.Height = rect.bottom - rect.top;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	

	// 뷰포트, 뷰변환, 투영변환 셋업
	D3DXMatrixLookAtLH( &m_matView, &m_Eye, &m_At, &m_Up );
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	m_pd3dDevice->SetViewport( &vp );
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);


	// initailize
	m_pAxis->OnInit( m_pd3dDevice );
	m_pCollision2->OnInit( m_pd3dDevice );

	// font
	D3DXCreateFont(m_pd3dDevice, 20, 0, FW_NORMAL, 1, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("System"), &m_pFont);
	m_nFPSCount = 0;
}

void GameLoop::OnRender()
{
	///// Calculate FPS /////
	wchar_t string[100];
	RECT rt = { 10, 10, 0, 0 };

	swprintf(string, TEXT("FPS: %d"), m_nFPS);
	m_pFont->DrawText(NULL, string, -1, &rt,
		DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_nFPSCount++;
	/////////////////////////

	m_pAxis->OnRender();

	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pCollision2->OnRender();
}

void GameLoop::OnUpdate()
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

	m_pCollision2->OnUpdate();
}

void GameLoop::OnRelease()
{
	m_pAxis->OnRelease();
	m_pCollision2->OnRelease();
}