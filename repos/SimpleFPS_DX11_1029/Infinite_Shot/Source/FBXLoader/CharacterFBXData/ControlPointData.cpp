#include "ControlPointData.h"

ControlPointData::ControlPointData()
{
	mBlendData.reserve(4);
}

XMFLOAT3 ControlPointData::GetPosition() const
{
	return mPosition;
}

std::vector<BlendControlPointData>* ControlPointData::GetBlendData()
{
	return &mBlendData;
}

void ControlPointData::SetPosition(FbxVector4* vertices)
{
	mPosition.x = vertices->mData[0];
	mPosition.y = vertices->mData[1];
	mPosition.z = vertices->mData[2];
}
