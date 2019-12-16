#include "FBXLoader.h"

FBXLoader::FBXLoader()
	: mMesh(nullptr)
	, mScene(nullptr)
{
}

HRESULT FBXLoader::LoadFbx(const char* fileName)
{
	if (!mManager)
	{
		mManager = FbxManager::Create();
		mIos = FbxIOSettings::Create(mManager, IOSROOT);
		mManager->SetIOSettings(mIos);
	}

	FbxImporter* pImporter = FbxImporter::Create(mManager, "");
	FbxScene* pFbxScene = FbxScene::Create(mManager, "");

	bool bSuccess = pImporter->Initialize(fileName, -1, mManager->GetIOSettings());
	assert(bSuccess, L"pImporter->Initialize() error");

	bSuccess = pImporter->Import(pFbxScene);
	assert(bSuccess, L"pImporter->Import() error");

	pImporter->Destroy();

	FbxGeometryConverter geometryConvert(mManager);
	geometryConvert.Triangulate(pFbxScene, true);

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
	assert(pFbxRootNode, L"pFbxScene->GetRootNode() error");

	int childCount = pFbxRootNode->GetChildCount();
	for (int i = 0; i < childCount; i++)
	{
		FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);
		/*if (!pFbxChildNode->GetNodeAttribute())
		{
			continue;
		}*/

		FbxNodeAttribute::EType attributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

		switch (attributeType)
		{
		case FbxNodeAttribute::eMesh:
		{
			mMesh = static_cast<FbxMesh*>(pFbxChildNode->GetNodeAttribute());

			break;
		}
		default:
		{
			continue;
		}
		}

		assert(attributeType == FbxNodeAttribute::eMesh, L"FBXLoader, attributeType all not eMesh");
		assert(mMesh, L"FBXLoader, attributeType all not eMesh");
		
		FbxVector4* pVertices = mMesh->GetControlPoints();
		assert(pVertices, L"pVertices == nullptr");
		int polygonCount = mMesh->GetPolygonCount();
		mVertices.reset(new XMFLOAT3[polygonCount * 3]);
		mUVs.reset(new XMFLOAT2[polygonCount * 3]);
		mVertexCount = 0;

		for (int j = 0; j < polygonCount; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				int controlPointIndex = mMesh->GetPolygonVertex(j, k);
				XMFLOAT3 vertex =
				{ 
					static_cast<float>(pVertices[controlPointIndex].mData[0]),
					static_cast<float>(pVertices[controlPointIndex].mData[1]),
					static_cast<float>(pVertices[controlPointIndex].mData[2])
				};
				mVertices[mVertexCount] = vertex;

				FbxVector2 uv;
				int uvIndex = mMesh->GetTextureUVIndex(j, k);
				FbxGeometryElementUV* eUV = mMesh->GetElementUV();
				assert(eUV, L"eUV == nullptr");
				uv = eUV->GetDirectArray().GetAt(uvIndex);
				XMFLOAT2 UVs =
				{
					static_cast<float>(uv[0]),
					static_cast<float>(uv[1]),
				};
				mUVs[mVertexCount] = UVs;

				mVertexCount++;
			}
		}
	}
	assert(mVertexCount, L"VertexCount == 0");

	return S_OK;
}

FBXLoader::~FBXLoader()
{
	if (mManager)
	{
		mManager->Destroy();
	}

	mVertices.reset();
	mUVs.reset();
}

XMFLOAT3* FBXLoader::GetVertices() const
{
	return mVertices.get();
}

XMFLOAT2* FBXLoader::GetUVs() const
{
	return mUVs.get();
}

unsigned int FBXLoader::GetVertexCount() const
{
	return mVertexCount;
}