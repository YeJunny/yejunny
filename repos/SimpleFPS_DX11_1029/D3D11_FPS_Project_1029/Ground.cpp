#include "FBXLoader.h"
#include "Global.h"
#include "Ground.h"

#include <fstream>

Ground::Ground()
{
	FBXLoader* fbxLoader = new FBXLoader();
	fbxLoader->LoadFbx("Fbx\\sphere3k.fbx");

	mVertexCount = fbxLoader->GetVertexCount();
	Assert(mVertexCount);
	mVertices.reset(new VertexElements[mVertexCount]);

	std::string fileName = "GroundNormals.txt";
	std::ofstream writeFile(fileName.data());
	for (UINT i = 0; i < mVertexCount; ++i)
	{
		mVertices[i].Pos = (fbxLoader->GetVertices())[i];
		mVertices[i].Uv = (fbxLoader->GetUVs())[i];
		mVertices[i].Normal = (fbxLoader->GetNormals())[i];
		if (writeFile.is_open())
		{
			writeFile << "(" << mVertices[i].Normal.x << ", " << mVertices[i].Normal.y << ", " << mVertices[i].Normal.z << ")\n";
		}
	}
	writeFile << mVertexCount << "\n";
	writeFile.close();
	delete fbxLoader;

	mLayoutElementNumber = 3;
	mLayout.reset(new D3D11_INPUT_ELEMENT_DESC[mLayoutElementNumber]);
	mLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

Ground::~Ground()
{
	mVertices.reset();
}

void Ground::Update(const XMMATRIX& viewMat)
{
	static float radians = 0.0f;
	ULONGLONG diffTick = mTimer->GetDiffTick();
	radians += diffTick * 0.001f;
	if (radians >= XM_PI * 2)
	{
		radians = 0;
	}

	mWorldMat = XMMatrixScaling(0.1f, 0.1f, 0.1f) *
		XMMatrixRotationRollPitchYaw(0, radians, 0.0f) *
		XMMatrixTranslation(10.0f, 0.5f, 10.0f);
	
	mViewMat = viewMat;
}