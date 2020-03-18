#pragma once
#include <DirectXMath.h>
#include <fbxsdk.h>
#include <string>
#include <vector>
#include "../Settings/Include/Framework.h"
#include "../Objects/Object.h"
#include "./CharacterFBXData/CharacterFbxData.h"

#define FBX_WRITE

#ifdef FBX_WRITE
#include <fstream>
#include <iomanip>
#endif

using namespace DirectX;
using namespace std;

class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

	HRESULT LoadFbx(CharacterFbxData* charFbxData, const char* fileName);
	unsigned int GetVertexCount() const;

private:
	void LoadByAttributeRecursive(FbxNode* node);
	void LoadByeMesh(FbxMesh* mesh, int childIndex);
	void LoadByeSkeleton(FbxNode* node, int depth, int index, int parentIndex);
	void LoadAnimation(FbxNode* node, int childIndex);
	int GetBoneIndexByName(FbxString name) const;
	XMMATRIX ConvertFbxAMatrixToXMMATRIX(FbxAMatrix& fbxAMatrix);

private:
	CharacterFbxData* mCharFbxData;
	FbxScene* mScene;
	string mFileName;
	ofstream mWriteFile;
	unsigned int mVertexCount;
};

