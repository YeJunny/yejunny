#pragma once

#include "../../Settings/Include/Framework.h"
#include <fbxsdk.h>
using namespace DirectX;

class BoneData
{
public:
	BoneData(FbxString name, int parentIndex);

	void Init(FbxString name, int parentIndex);
	FbxString GetName() const;

private:
	int mParentIndex;
	FbxString mName;
};

