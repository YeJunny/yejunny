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
	if (!bSuccess)
	{
		return E_FAIL;
	}

	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess)
	{
		return E_FAIL;
	}
	pImporter->Destroy();

	FbxGeometryConverter geometryConvert(mManager);
	geometryConvert.Triangulate(pFbxScene, true);

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
	if (!pFbxRootNode)
	{
		return E_FAIL;
	}

	for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
	{
		FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);
		if (!pFbxChildNode->GetNodeAttribute())
		{
			continue;
		}

		FbxNodeAttribute::EType attributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

		switch (attributeType)
		{
		case FbxNodeAttribute::eMesh:
		{
			mMesh = reinterpret_cast<FbxMesh*>(pFbxChildNode->GetNodeAttribute());

			break;
		}
		default:
		{
			continue;
		}
		}

		if (!mMesh)
		{
			assert(mMesh, L"FBXLoader, attributeType all not eMesh");
			return E_FAIL;
		}

		FbxVector4* pVertices = mMesh->GetControlPoints();
		mVertices = new XMFLOAT3[mMesh->GetPolygonCount() * 3];
		mUVs = new XMFLOAT2[mMesh->GetPolygonCount() * 3];
		mVertexCount = 0;

		for (int j = 0; j < mMesh->GetPolygonCount(); j++)
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
				FbxGeometryElementUV* eUV = mMesh->GetElementUV(0);
				int uvIndex = mMesh->GetTextureUVIndex(j, k);
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

XMFLOAT2* FBXLoader::GetUVs() const
{
	return mUVs;
}

unsigned int FBXLoader::GetVertexCount() const
{
	return mVertexCount;
}