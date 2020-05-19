#pragma once

#include "PureVertexData.h"
#include "BoneData.h"
#include "ControlPointData.h"
#include <vector>
#include <unordered_map>
using namespace std;

struct CharacterFbxData
{
	vector<vector<PureVertexData>> PureVertexData;
	vector<XMMATRIX> InverseMatrixAnim;
	vector<vector<XMMATRIX>> KeyframeMatrixPerTime;
	int EndTime;
	bool bHasAnimation;


	// Temp
	vector<BoneData> AnimSkeletonData;
	unordered_map<unsigned int, ControlPointData> FbxControlPointData;
};