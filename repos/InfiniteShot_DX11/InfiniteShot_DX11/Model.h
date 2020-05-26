#pragma once

//#pragma comment(lib, "libfbxsdk.lib")
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

#include <DirectXMath.h>
#include <fbxsdk.h>
#include <unordered_map>
#include <vector>
#include <Windows.h>
#include "ErrorLogger.h"

#define LOG_WRITE_FBX_DATA

#ifdef LOG_WRITE_FBX_DATA
#include <fstream>
#include <iomanip>
#endif

namespace Model_
{
	struct Vertex
	{
		DirectX::XMFLOAT4 Pos;
		DirectX::XMFLOAT2 TexCoord;
		DirectX::XMFLOAT3 Normal;
		// Animation infomation
		DirectX::XMFLOAT4 Weights;
		DirectX::XMINT4 Indices;
	};

	struct Skeleton
	{
		int ParentIndex;
		FbxString Name;
	};

	struct TempAnimInfo
	{
		float Weight;
		int Index;
	};

	struct ContrlPoint
	{
		DirectX::XMFLOAT3 Pos;
		std::vector<TempAnimInfo> TempAnimInfo;
	};

	class Model
	{
	public:
		HRESULT ImportModelFromFile(std::string modelFileName);
		void ReleaseModel();

	public:
		std::vector<std::vector<Vertex>> const& GetVertices() const { return mVertices; }

	private:
		HRESULT ImportBaseRecursive(FbxNode* pNode);
		HRESULT ImportVertexInfo(FbxMesh* pMesh, int childIndex);
		HRESULT ImportSkeletonRecursive(FbxNode* pNode, int depth = 0, int parentIndex = -1, int index = 0);
		HRESULT ImportAnim(FbxNode* pNode, int childIndex);
		UINT GetSkeleIndex(FbxString skeleName) const;
		DirectX::XMMATRIX GetXMMATRIX(const FbxAMatrix const* fbxMatrix);

	private:
		bool mbHasAnim = false;
		int mEndTime = 0;
		std::vector<std::vector<Vertex>> mVertices;
		std::vector<DirectX::XMMATRIX> mAnimInvMatrix;
		std::vector<std::vector<DirectX::XMMATRIX>> mAnimKeyFramePerTime;

		FbxScene* mScene;
		std::ofstream mLogStream;
		std::string mLogDirectory;

		std::vector<Skeleton> mSkele;
		std::unordered_map<UINT, ContrlPoint> mContrlPoint;
	};
}