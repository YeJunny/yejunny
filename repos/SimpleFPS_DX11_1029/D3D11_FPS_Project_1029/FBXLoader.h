#pragma once
#include <fbxsdk.h>
#include "framework.h"
#include <DirectXMath.h>
using namespace DirectX;

class FBXLoader
{
public:
	HRESULT LoadFbx(const char* fileName);
	void LoadNode(FbxNode* node);
	void ProcessControlPoints(FbxMesh* mesh);

	XMFLOAT3* GetVertices() const;
	XMFLOAT2* GetUVs() const;
	XMFLOAT3* GetNormals() const;
	void GetNormals(FbxNode* Node);
	unsigned int GetVertexCount() const;

	FBXLoader();
	~FBXLoader();

private:
	std::unique_ptr<XMFLOAT3[]> mVertices;
	std::unique_ptr<XMFLOAT3[]> mNormals;
	std::unique_ptr<XMFLOAT2[]> mUVs;
	unsigned int mVertexCount;
};

