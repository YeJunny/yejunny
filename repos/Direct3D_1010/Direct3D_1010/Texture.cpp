#include "Texture.h"

struct CUSTOMVERTEX
{
	D3DXVECTOR3 vPos;
	float tu, uv;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)

void Texture::OnInit( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pd3dDevice = pd3dDevice;

	CUSTOMVERTEX vertices[] = 
	{
		{ D3DXVECTOR3(-1.0f, 1.0f, 0.0f), 0.0f, 0.0f },
		{ D3DXVECTOR3( 1.0f, 1.0f, 0.0f), 1.0f, 0.0f },
		{ D3DXVECTOR3(-1.0f, -1.0f, 0.0f), 0.0f, 1.0f },
		{ D3DXVECTOR3( 1.0f, -1.0f, 0.0f), 1.0f, 1.0f },
	};

	m_pd3dDevice->CreateVertexBuffer( sizeof(vertices), 0, D3DFVF_CUSTOMVERTEX,
										D3DPOOL_DEFAULT, &m_pVB, 0 );

	void* pVertices;
	m_pVB->Lock( 0, sizeof(vertices), &pVertices, 0 );
	memcpy( pVertices, vertices, sizeof(vertices) );
	m_pVB->Unlock();

	D3DXCreateTextureFromFile( m_pd3dDevice, TEXT("earth.jpg"), &m_pTexture );

	D3DXMatrixScaling(&m_matScale[0], 0.5f, 0.5f, 0.5f);
	D3DXMatrixScaling(&m_matScale[1], 1.5f, 1.5f, 1.5f);

	D3DXMatrixTranslation( &m_matTranslate[0], -2.0f, 0.0f, 0.0f );
	D3DXMatrixTranslation( &m_matTranslate[1], 1.0f, 0.0f, 0.0f );

}

void Texture::OnRender()
{
	D3DXMATRIX matWorld;

	m_pd3dDevice->SetTexture( 0, m_pTexture );

	m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(CUSTOMVERTEX) );

	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < 2; i++)
	{
		matWorld = m_matScale[i] * m_matTranslate[i];
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}	

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

void Texture::OnRelease()
{

}