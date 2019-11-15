#include "Material.h"
#include <d3dx9math.h>

struct CUSTOMVERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL)

void Material::OnInit( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pd3dDevice = pd3dDevice;

	CUSTOMVERTEX vertices[] = 
	{
		{ D3DXVECTOR3(-1.0f, -0.3f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(-1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },

		{ D3DXVECTOR3(-1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },

		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, -0.3f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },

		{ D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, -0.3f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },

		{ D3DXVECTOR3(-1.0f, 0.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(-1.0f, -0.3f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },

		{ D3DXVECTOR3(-1.0f, -0.3f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },

		{ D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, 0.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },

		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, -0.3f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, 0.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) },

	};

	m_pd3dDevice->CreateVertexBuffer( sizeof(vertices), 0, D3DFVF_CUSTOMVERTEX,
										D3DPOOL_DEFAULT, &m_pVB, NULL );

	D3DXVECTOR3 vS0, vS1;
	for (int i = 0; i < 8; i++)
	{
		vS0 = vertices[i * 3 + 2].vPos - vertices[i * 3].vPos;
		vS1 = vertices[i * 3 + 1].vPos - vertices[i * 3].vPos;
		D3DXVec3Cross( &vertices[i * 3].vNormal, &vS1, &vS0 );
		D3DXVec3Normalize(&vertices[i * 3].vNormal, &vertices[i * 3].vNormal);
	
		vertices[i * 3 + 2].vNormal = vertices[i * 3 + 1].vNormal = vertices[i * 3].vNormal;
	}

	void* pVertices;
	if (FAILED ( m_pVB->Lock(0, sizeof(vertices), &pVertices, 0) ) )
	{
		return;
	}
	memcpy( pVertices, vertices, sizeof(vertices) );
	m_pVB->Unlock();

	D3DCOLORVALUE diffuse;
	diffuse.r = 1.0f;
	diffuse.g = 1.0f;
	diffuse.b = 1.0f;
	diffuse.a = 1.0f;
	ZeroMemory(&m_Material, sizeof(D3DMATERIAL9));
	m_Material.Ambient = m_Material.Diffuse = diffuse;
	m_pd3dDevice->SetMaterial(&m_Material);

	m_Light.Type = D3DLIGHT_DIRECTIONAL;
	m_Light.Ambient.g = m_Light.Ambient.b = m_Light.Ambient.r = 1.0f;
	m_Light.Ambient.a = 0.5f;
	m_Light.Direction.x = -1.0f;
	m_Light.Direction.y = -0.4f;
	m_Light.Direction.z = 0.5f;

	m_pd3dDevice->SetLight(0, &m_Light);
	m_pd3dDevice->LightEnable(0, TRUE);

}

void Material::OnRender()
{
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 8 );
}

void Material::OnUpdate()
{
}

void Material::OnRelease()
{
	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
}

