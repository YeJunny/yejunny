#include "Ground.h"
#include <d3dx9.h>

void Ground::Create(LPDIRECT3DDEVICE9 pd3dDevice, int nRow, int nCol, float fSize)
{
	m_row = nRow;
	m_col = nCol;
	m_pd3dDevice = pd3dDevice;

	// Vertex buffer
	m_numVertices = (m_row + 1) * (m_col + 1);
	GROUNDVERTEX* pGroundVertex = new GROUNDVERTEX[m_numVertices];

	int nIndex = 0;
	D3DXVECTOR3 vPos0(-1.0f * m_col * fSize * 0.5f, 0.0f, m_row * fSize * 0.5f);
	for (int z = 0; z <= m_row; z++)
	{
		for (int x = 0; x <= m_col; x++)
		{
			pGroundVertex[nIndex].pos.x = vPos0.x + x * fSize;
			pGroundVertex[nIndex].pos.y = 0.0f;
			pGroundVertex[nIndex].pos.z = vPos0.z - z * fSize;
			pGroundVertex[nIndex].diffuse = D3DCOLOR_RGBA(255, 50, 255, 255);
			nIndex++;
		}
	}

	m_pd3dDevice->CreateVertexBuffer(m_numVertices * sizeof(GROUNDVERTEX), 0,
		D3DFVF_GROUNDVERTEX, D3DPOOL_DEFAULT, &m_pVB, nullptr);

	void* pVertices;
	m_pVB->Lock(0, 0, &pVertices, 0);
	memcpy(pVertices, pGroundVertex, sizeof(pGroundVertex) * m_numVertices);
	m_pVB->Unlock();

	
	// Index buffer
	m_numIndices = m_col * m_row * 6;
	WORD* pIndex = new WORD[m_numIndices];
	nIndex = 0;
	
	for (int z = 0; z < m_row; z++)
	{
		for (int x = 0; x < m_col; x++)
		{
			pIndex[nIndex++] = (WORD)(z * (m_col + 1) + x);
			pIndex[nIndex++] = (WORD)((z + 1) * (m_col + 1) + x + 1);
			pIndex[nIndex++] = (WORD)((z + 1) * (m_col + 1) + x);

			pIndex[nIndex++] = (WORD)(z * (m_col + 1) + x);
			pIndex[nIndex++] = (WORD)(z * (m_col + 1) + x + 1);
			pIndex[nIndex++] = (WORD)((z + 1) * (m_col + 1) + x + 1);
		}
	}

	void* pIndices;
	m_pd3dDevice->CreateIndexBuffer(m_numIndices * sizeof(WORD), 0, 
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, nullptr);

	m_pIB->Lock(0, 0, &pIndices, 0);
	memcpy(pIndices, (void*)pIndex, sizeof(WORD) * m_numIndices);
	m_pIB->Unlock();

	delete[] pIndex;
	delete[] pGroundVertex;
}

void Ground::OnRender()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(GROUNDVERTEX));

	m_pd3dDevice->SetIndices(m_pIB);
	m_pd3dDevice->SetFVF(D3DFVF_GROUNDVERTEX);
	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_numVertices, 0, m_col * m_row);

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

void Ground::OnRelease()
{
	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB = nullptr;
	}

	if (m_pIB)
	{
		m_pIB->Release();
		m_pIB = nullptr;
	}
}