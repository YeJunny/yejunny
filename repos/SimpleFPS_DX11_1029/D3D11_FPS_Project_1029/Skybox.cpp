#include "FBXLoader.h"
#include "Global.h"
#include "Skybox.h"

#include <fstream>

Skybox::Skybox()
{
	FBXLoader fbxLoader;

	fbxLoader.LoadFbx("Fbx\\skybox.fbx");
	mVertexCount = fbxLoader.GetVertexCount();
	assert(mVertexCount);
	mVertices.reset(new VertexElements[mVertexCount]);

	for (UINT i = 0; i < mVertexCount; ++i)
	{
		mVertices[i].Pos = (fbxLoader.GetVertices())[i];
		mVertices[i].Uv = (fbxLoader.GetUVs())[i];
	}

	mLayoutElementNumber = 2;
	mLayout.reset(new D3D11_INPUT_ELEMENT_DESC[mLayoutElementNumber]);
	mLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };

	mWorldMat = XMMatrixScaling(60.0f, 60.0f, 60.0f);
}

Skybox::~Skybox()
{
	mVertices.reset();
}

void Skybox::InitDetail(HWND hWnd)
{
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.CullMode = D3D11_CULL_NONE;
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.FrontCounterClockwise = true;
	mRasterizerState.Reset();
 	HRESULT hr = mD3DDevice->CreateRasterizerState(&cmdesc, mRasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		assert(hr == S_OK);
	}
}
