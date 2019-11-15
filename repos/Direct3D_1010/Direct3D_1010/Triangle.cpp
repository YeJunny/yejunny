#include "Triangle.h"
#include <d3dx9math.h>

struct CUSTOMVERTEX
{
	D3DXVECTOR3 position;
	DWORD Color;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

void Triangle::OnInit(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	CUSTOMVERTEX vertices[] =
	{
		{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DCOLOR_RGBA(255, 0, 0, 255) },
		{ D3DXVECTOR3(1.0f, -1.0f, 1.5f), D3DCOLOR_RGBA(255, 255, 0, 255) },
		{ D3DXVECTOR3(1.0f, -1.0f, -1.5f), D3DCOLOR_RGBA(255, 0, 255, 255) }
	};

	if ( FAILED( m_pd3dDevice->CreateVertexBuffer(
								3 * sizeof(CUSTOMVERTEX),
								0,
								D3DFVF_CUSTOMVERTEX,
								D3DPOOL_DEFAULT,
								&m_pVB,
								NULL ) ) )
	{
		return;
	}

	void* pVertices;
	if (FAILED(m_pVB->Lock(0, sizeof(vertices), &pVertices, 0)))
	{
		return;
	}
	memcpy(pVertices, vertices, sizeof(vertices));
	m_pVB->Unlock();



	D3DXVECTOR3 vDiff0 = vertices[2].position - vertices[0].position;
	D3DXVECTOR3 vDiff1 = vertices[1].position - vertices[0].position;
	D3DXVECTOR3 n0, n1, n2;

	D3DXVec3Cross(&n0, &vDiff1, &vDiff0);
	D3DXVec3Normalize(&n0, &n0);
	n2 = n1 = n0;

	CUSTOMVERTEX normals[6] =
	{
		{ n0 + vertices[0].position, D3DCOLOR_RGBA(255, 0, 0, 255) },
		{ vertices[0].position, D3DCOLOR_RGBA(255, 0, 0, 255) },

		{ n1 + vertices[1].position, D3DCOLOR_RGBA(0, 255, 0, 255) },
		{ vertices[1].position, D3DCOLOR_RGBA(0, 255, 0, 255) },

		{ n2 + vertices[2].position, D3DCOLOR_RGBA(0, 0, 255, 255) },
		{ vertices[2].position, D3DCOLOR_RGBA(0, 0, 255, 255) },
	};

	if (FAILED(m_pd3dDevice->CreateVertexBuffer(
								6 * sizeof(CUSTOMVERTEX),
								0,
								D3DFVF_CUSTOMVERTEX,
								D3DPOOL_DEFAULT,
								&m_pNB,
								NULL)))
	{
		return;
	}

	void* pNormals;
	if (FAILED(m_pNB->Lock(0, sizeof(normals), &pNormals, 0)))
	{
		return;
	}
	memcpy(pNormals, normals, sizeof(normals));
	m_pNB->Unlock();
}


void Triangle::OnRender()
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );

	m_pd3dDevice->SetStreamSource(0, m_pNB, 0, sizeof(CUSTOMVERTEX));
	m_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 3);

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

void Triangle::OnRelease()
{
	if ( m_pVB )
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
}