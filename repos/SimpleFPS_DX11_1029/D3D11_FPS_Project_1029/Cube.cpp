#include "Cube.h"
#include "FBXLoader.h"
#include "Global.h"

#include <fstream>

Cube::Cube()
{
	FBXLoader fbxLoader;

	fbxLoader.LoadFbx("Fbx\\skybox.fbx");
	mVertexCount = fbxLoader.GetVertexCount();
	assert(mVertexCount, L"Cube, mVertexCount == 0");
	mVertices.reset(new VertexElements[mVertexCount]);

	std::string fileName = "CubeUVs.txt";
	std::ofstream writeFile(fileName.data());
	for (UINT i = 0; i < mVertexCount; ++i)
	{
		mVertices[i].Pos = (fbxLoader.GetVertices())[i];
		mVertices[i].Uv = (fbxLoader.GetUVs())[i];
		if (writeFile.is_open())
		{
			writeFile << "(" << mVertices[i].Uv.x << ", " << mVertices[i].Uv.y << ")\n";
		}
	}
	writeFile << mVertexCount << "\n";
	writeFile.close();

	mLayoutElementNumber = 2;
	mLayout.reset(new D3D11_INPUT_ELEMENT_DESC[mLayoutElementNumber]);
	mLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	mLayout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };

	mWorld = XMMatrixScaling(60.0f, 60.0f, 60.0f);
}

Cube::~Cube()
{
	mVertices.reset();
}

void Cube::Update(const XMMATRIX view)
{
	/*static float radians = 0.0f;
	ULONGLONG diffTick = mTimer->GetDiffTick();
	radians += diffTick * 0.001f;
	if (radians >= XM_PI * 2)
	{
		radians -= XM_PI * 2;
	}

	mWorld = XMMatrixRotationY(radians) * XMMatrixTranslation(10.0f, 0.5f, 10.0f);
	*/
	mView = view;
}