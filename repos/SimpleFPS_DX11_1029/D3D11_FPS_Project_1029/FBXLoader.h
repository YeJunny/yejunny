#pragma once
#include "fbxsdk.h"
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
	unsigned int GetVertexCount() const;

	FBXLoader();
	~FBXLoader();

private:
	FbxManager* mManager;
	FbxIOSettings* mIos;
	FbxScene* mScene;
	FbxMesh* mMesh;

	XMFLOAT3* mVertices;
	XMFLOAT2* mUVs;
	unsigned int mVertexCount;
};
