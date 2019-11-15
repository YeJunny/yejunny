#include "FBXLoader.h"

FBXLoader::FBXLoader()
	: mMesh(nullptr)
	, mScene(nullptr)
{
}

HRESULT FBXLoader::LoadFbx(const char* fileName)
{
	if (mManager == nullptr)
	{
		mManager = FbxManager::Create();
		mIos = FbxIOSettings::Create(mManager, IOSROOT);
		mManager->SetIOSettings(mIos);
	}

	FbxImporter* pImporter = FbxImporter::Create(mManager, "");
	FbxScene* pFbxScene = FbxScene::Create(mManager, "");

	bool bSuccess = pImporter->Initialize(fileName, -1, mManager->GetIOSettings());
	if (!bSuccess) return E_FAIL;

	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess) return E_FAIL;

	pImporter->Destroy();

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (!pFbxRootNode) return E_FAIL;

	for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
	{
		FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

		if (pFbxChildNode->GetNodeAttribute() == NULL)
			continue;

		FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

		if (AttributeType != FbxNodeAttribute::eMesh)
			continue;

		mMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();

		FbxVector4* pVertices = mMesh->GetControlPoints();
		mVertices = new XMFLOAT3[mMesh->GetPolygonCount() * 3];
		mVertexCount = 0;

		for (int j = 0; j < mMesh->GetPolygonCount(); j++)
		{
			int iNumVertices = mMesh->GetPolygonSize(j);
			assert(iNumVertices == 3);

			for (int k = 0; k < iNumVertices; k++)
			{
				int iControlPointIndex = mMesh->GetPolygonVertex(j, k);

				XMFLOAT3 vertex;
				vertex.x = (float)pVertices[iControlPointIndex].mData[0];
				vertex.y = (float)pVertices[iControlPointIndex].mData[1];
				vertex.z = (float)pVertices[iControlPointIndex].mData[2];
				mVertices[mVertexCount++] = vertex;
			}
		}
	}
	return S_OK;
}

FBXLoader::~FBXLoader()
{
	if (mManager)
	{
		mManager->Destroy();
	}
}

XMFLOAT3* FBXLoader::GetVertices() const
{
	return mVertices;
}

unsigned int FBXLoader::GetVertexCount() const
{
	return mVertexCount;
}