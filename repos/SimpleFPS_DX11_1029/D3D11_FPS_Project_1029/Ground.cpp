#include "FBXLoader.h"
#include "Global.h"
#include "Ground.h"

Ground::Ground()
{
	FBXLoader* fbxLoader = new FBXLoader();
	fbxLoader->LoadFbx("Fbx\\Book.fbx");

	mVertexCount = fbxLoader->GetVertexCount();
	assert(mVertexCount, L"Ground, mVertexCount == 0");
	mVertices.reset(new VertexElements[mVertexCount]);
	for (unsigned int i = 0; i < mVertexCount; ++i)
	{
		mVertices[i].Pos = (fbxLoader->GetVertices())[i];
		mVertices[i].Uv = (fbxLoader->GetUVs())[i];
	}

	mWorldMat = XMMatrixRotationRollPitchYaw(XM_PI / 2, 0.0f, 0.0f) *
		XMMatrixTranslation(0.0f, -0.6f, 0.0f);
	delete fbxLoader;

	mLayoutElementNumber = 2;
	mLayout.reset(new D3D11_INPUT_ELEMENT_DESC[mLayoutElementNumber]);
	mLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

Ground::~Ground()
{
	mVertices.reset();
}