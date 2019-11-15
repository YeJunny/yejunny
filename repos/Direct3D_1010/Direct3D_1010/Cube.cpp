#include "Cube.h"
#include <d3dx9math.h>

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct CUSTOMVERTEX
{
	D3DXVECTOR3 position;
	DWORD Color;
};

void Cube::OnInit( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pd3dDevice = pd3dDevice;

	CUSTOMVERTEX vertices[] = 
	{
		{ D3DXVECTOR3(-1.0f, 1.0f,  1.0f), D3DCOLOR_RGBA(255, 0, 255, 255) },
		{ D3DXVECTOR3( 1.0f, 1.0f,  1.0f), D3DCOLOR_RGBA(0, 255, 0, 255) },
		{ D3DXVECTOR3( 1.0f, 1.0f, -1.0f), D3DCOLOR_RGBA(0, 0, 255, 255) },
		{ D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 0, 255) },

		{ D3DXVECTOR3(-1.0f, -1.0f,  1.0f), D3DCOLOR_RGBA(255, 0, 0, 255) },
		{ D3DXVECTOR3( 1.0f, -1.0f,  1.0f), D3DCOLOR_RGBA(0, 255, 255, 255) },
		{ D3DXVECTOR3( 1.0f, -1.0f, -1.0f), D3DCOLOR_RGBA(0, 0, 255, 255) },
		{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 0, 255) },
	};

	WORD indices[] = 
	{
		0,1,3,
		3,1,2,

		4, 6, 5,
		4, 7, 6,
		0, 3, 7,
		0, 7, 4, 
		1, 5, 6,
		1, 6, 2,
		3, 2, 6,
		3, 6, 7, 
		0, 4, 5,	// 이 두개
		0, 5, 1		// 자리 바꾸면 어떻게 될까?
	};

	// 정점버퍼 셋팅
	m_pd3dDevice->CreateVertexBuffer( sizeof(vertices),
										0,
										D3DFVF_CUSTOMVERTEX,
										D3DPOOL_DEFAULT,
										&m_pVB,
										NULL );
	void* pVertices;
	if (FAILED ( m_pVB->Lock( 0, sizeof(vertices), &pVertices, 0 ) ) )
	{
		return;
	}
	memcpy( pVertices, &vertices, sizeof(vertices) );
	m_pVB->Unlock();

	// 인덱스 버퍼 셋팅
	m_pd3dDevice->CreateIndexBuffer( sizeof(indices),
										0,
										D3DFMT_INDEX16,
										D3DPOOL_DEFAULT,
										&m_pIB,
										NULL );

	void* pIndices;
	if ( FAILED( m_pIB->Lock( 0, sizeof(indices), &pIndices, 0 ) ) )
	{
		return;
	}
	memcpy( pIndices, &indices, sizeof(indices) );
	m_pIB->Unlock();
}

void Cube::OnRender()
{
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(CUSTOMVERTEX) );
	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	m_pd3dDevice->SetIndices( m_pIB );
	m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0,
										8, 0, 6 * 2 );

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW );
}

void Cube::OnRelease()
{
	if ( m_pVB )
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	if ( m_pIB )
	{
		m_pIB->Release();
		m_pIB = NULL;
	}
}
