#include "BoneData.h"

BoneData::BoneData(FbxString name, int parentIndex)
	: mName(name)
	, mParentIndex(parentIndex)
{
}

void BoneData::Init(FbxString name, int parentIndex)
{
	mName = name;
	mParentIndex = parentIndex;
}

FbxString BoneData::GetName() const
{
	return mName;
}
