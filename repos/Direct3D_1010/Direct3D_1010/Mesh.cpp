#include "Mesh.h"

void Mesh::OnInit(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	D3DXLoadMeshFromX(TEXT("Temple.x"), D3DXMESH_SYSTEMMEM, m_pd3dDevice,
		NULL, &m_pD3DMtrlBuffer, NULL, &m_dwNumMaterials, &m_pMesh);

	D3DXMATERIAL* d3dxMaterials =
		(D3DXMATERIAL*)m_pD3DMtrlBuffer->GetBufferPointer();
	m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
	m_pMeshTexture = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

	for (int i = 0; i < m_dwNumMaterials; ++i)
	{
		m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;
		m_pMeshTexture = NULL;
		
		D3DXCreateTextureFromFile(m_pd3dDevice,
				TEXT("stone.jpg"),
				&m_pMeshTexture[i]);
		
	}

	m_pD3DMtrlBuffer->Release();
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

}


void Mesh::OnRender()
{
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	for (DWORD i = 0; i < m_dwNumMaterials; ++i)
	{
		m_pd3dDevice->SetMaterial(&m_pMeshMaterials[i]);
		m_pMesh->DrawSubset(i);
	}
}

void Mesh::OnRelease()
{
	if (m_pMeshMaterials)
	{
		delete[] m_pMeshMaterials;
		m_pMeshMaterials = NULL;
	}

	if (m_pMeshTexture)
	{
		for (int i = 0; i < m_dwNumMaterials; ++i)
		{
			if (m_pMeshTexture[i])
			{
				m_pMeshTexture[i]->Release();
				m_pMeshTexture[i] = NULL;
			}
		}

		delete[] m_pMeshTexture;
		m_pMeshTexture = NULL;
	}

	if (m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
}