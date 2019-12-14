#include "FBXLoader.h"
#include "Global.h"
#include "Ground.h"

Ground::Ground()
{
	FBXLoader* fbxLoader = new FBXLoader();
	fbxLoader->LoadFbx("Fbx\\Book.fbx");

	mVertexCount = fbxLoader->GetVertexCount();
	assert(mVertexCount, L"Ground, mVertexCount == 0");
	mVertices.reset(new SimpleVertex[mVertexCount]);
	for (unsigned int i = 0; i < mVertexCount; ++i)
	{
		mVertices[i].Pos = (fbxLoader->GetVertices())[i];
		mVertices[i].Color = XMFLOAT4(0.1f, 5.1f, 0.1f, 1.0f);
	}

	mWorld = XMMatrixRotationRollPitchYaw(XM_PI / 2, 0.0f, 0.0f) *
		XMMatrixTranslation(0.0f, -0.6f, 0.0f);
	delete fbxLoader;

	mLayoutElementNumber = 2;
	mLayout.reset(new D3D11_INPUT_ELEMENT_DESC[mLayoutElementNumber]);
	mLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

Ground::~Ground()
{
	mVertices.reset();
}