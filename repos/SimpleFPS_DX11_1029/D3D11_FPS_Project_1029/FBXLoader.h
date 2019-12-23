#pragma once
#include <DirectXMath.h>
#include <fbxsdk.h>
#include <string>
#include <vector>
#include "framework.h"
#include "Object.h"

using namespace DirectX;

class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

	HRESULT LoadFbx(std::vector<VertexElements>* elements, const char* fileName);
	void LoadNode(FbxNode* node);

	void GetVerticesUVsNormalsRecursive(FbxNode* node);
	unsigned int GetVertexCount() const;

private:
	std::vector<VertexElements>* mElements;
	std::string mFileName;
	unsigned int mVertexCount;
};

