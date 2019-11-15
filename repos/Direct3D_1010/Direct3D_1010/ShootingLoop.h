#pragma once

#include "D3DApp.h"
#include <d3dx9.h>

class Ground;
class Player;

class ShootingLoop : public D3DApp
{
protected:
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

private:
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXVECTOR3 m_Eye, m_At, m_Up;

	// font
	LPD3DXFONT m_pFont;

	DWORD m_dwElapsedTime;
	UINT m_nFPS, m_nFPSCount;

	// object
	Ground* m_pGround;
	Player* m_pPlayer;
};

