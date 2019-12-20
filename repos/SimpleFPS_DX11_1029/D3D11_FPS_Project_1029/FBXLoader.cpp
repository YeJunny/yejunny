#include "FBXLoader.h"

FBXLoader::FBXLoader()
{
}

FBXLoader::~FBXLoader()
{
	mVertices.reset();
	mUVs.reset();
	mNormals.reset();
}

HRESULT FBXLoader::LoadFbx(const char* fileName)
{
	// Init & Setting
	FbxManager* manager = FbxManager::Create();
	assert(manager);
	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	assert(ios);
	manager->SetIOSettings(ios);

	FbxImporter* importer = FbxImporter::Create(manager, "");
	assert(importer);
	FbxScene* fbxScene = FbxScene::Create(manager, "");
	assert(fbxScene);

	bool bSuccess = importer->Initialize(fileName, -1, manager->GetIOSettings());
	assert(bSuccess);

	bSuccess = importer->Import(fbxScene);
	assert(bSuccess);
	importer->Destroy();

	FbxGeometryConverter geometryConvert(manager);
	geometryConvert.Triangulate(fbxScene, true);


	// Process Calculation
	FbxNode* fbxRootNode = fbxScene->GetRootNode();
	assert(fbxRootNode);
	GetNormals(fbxRootNode);

	FbxMesh* mesh;
	int childCount = fbxRootNode->GetChildCount();
	for (int i = 0; i < childCount; i++)
	{
		FbxNode* fbxChildNode = fbxRootNode->GetChild(i);
		if (!fbxChildNode->GetNodeAttribute())
		{
			continue;
		}

		FbxNodeAttribute::EType attributeType = fbxChildNode->GetNodeAttribute()->GetAttributeType();
		switch (attributeType)
		{
		case FbxNodeAttribute::eMesh:
		{
			mesh = static_cast<FbxMesh*>(fbxChildNode->GetNodeAttribute());

			break;
		}
		default:
		{
			continue;
		}
		}

		assert(attributeType == FbxNodeAttribute::eMesh, L"FBXLoader, attributeType all not eMesh");
		assert(mesh);

		FbxVector4* vertices = mesh->GetControlPoints();
		assert(vertices);
		int polygonCount = mesh->GetPolygonCount();
		mVertices.reset(new XMFLOAT3[polygonCount * 3]);
		mUVs.reset(new XMFLOAT2[polygonCount * 3]);
		mVertexCount = 0;

		for (int j = 0; j < polygonCount; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				int vertexIndex = mesh->GetPolygonVertex(j, k);
				XMFLOAT3 vertex =
				{
					static_cast<float>(vertices[vertexIndex].mData[0]),
					static_cast<float>(vertices[vertexIndex].mData[1]),
					static_cast<float>(vertices[vertexIndex].mData[2])
				};
				mVertices[mVertexCount] = vertex;

				FbxGeometryElementUV* eUV = mesh->GetElementUV();
				assert(eUV);
				int uvIndex = mesh->GetTextureUVIndex(j, k);
				FbxVector2 uv = eUV->GetDirectArray().GetAt(uvIndex);

				XMFLOAT2 UVs =
				{
					static_cast<float>(uv[0]),
					static_cast<float>(uv[1]),
				};
				mUVs[mVertexCount] = UVs;

				mVertexCount++;
			}
		}

		break;
	}
	assert(mVertexCount);

	if (manager)
	{
		manager->Destroy();
	}

	return S_OK;
}

XMFLOAT3* FBXLoader::GetVertices() const
{
	return mVertices.get();
}

XMFLOAT2* FBXLoader::GetUVs() const
{
	return mUVs.get();
}

XMFLOAT3* FBXLoader::GetNormals() const
{
	return mNormals.get();
}

unsigned int FBXLoader::GetVertexCount() const
{
	return mVertexCount;
}

void FBXLoader::GetNormals(FbxNode* node)
{
	if (!node)
		return;

	FbxMesh* lMesh = node->GetMesh();
	for (int i = 0; !lMesh; ++i)
	{
		lMesh = node->GetChild(i)->GetMesh();
	}
	
	if (lMesh)
	{
		FbxGeometryElementNormal* lNormalElement = lMesh->GetElementNormal();
		FbxVector4* vertexElement = lMesh->GetControlPoints();
		FbxGeometryElementUV* uvElement = lMesh->GetElementUV();

		switch (lNormalElement->GetMappingMode())
		{
		/*case FbxGeometryElement::eByControlPoint:
		{
			mNormals.reset(new XMFLOAT3[lMesh->GetControlPointsCount()]);

			for (int lVertexIndex = 0; lVertexIndex < lMesh->GetControlPointsCount(); lVertexIndex++)
			{
				int lNormalIndex = 0;

				if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
					lNormalIndex = lVertexIndex;

				if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					lNormalIndex = lNormalElement->GetIndexArray().GetAt(lVertexIndex);

				FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
				
				mNormals[lVertexIndex] =
				{
					static_cast<float>(lNormal[0]),
					static_cast<float>(lNormal[1]),
					static_cast<float>(lNormal[2])
				};
			}

			break;
		}*/
		case FbxGeometryElement::eByPolygonVertex:
		{
			int lIndexByPolygonVertex = 0;

			/*mVertices.reset(new XMFLOAT3[lMesh->GetPolygonCount() * 3]);
			mUVs.reset(new XMFLOAT2[lMesh->GetPolygonCount() * 3]);*/
			mNormals.reset(new XMFLOAT3[lMesh->GetPolygonCount() * 3]);

			for (int lPolygonIndex = 0; lPolygonIndex < lMesh->GetPolygonCount(); lPolygonIndex++)
			{
				int lPolygonSize = lMesh->GetPolygonSize(lPolygonIndex);

				for (int i = 0; i < lPolygonSize; i++)
				{
					int lNormalIndex = 0;
					int uvIndex = 0;
					int vertexIndex = lMesh->GetPolygonVertex(lPolygonIndex, i);

					if (uvElement->GetMappingMode() == FbxGeometryElement::eDirect)
					{
						uvIndex = lIndexByPolygonVertex;
					}
					if (uvElement->GetMappingMode() == FbxGeometryElement::eIndexToDirect)
					{
						uvIndex = uvElement->GetIndexArray().GetAt(lIndexByPolygonVertex);
					}

					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
					{
						lNormalIndex = lIndexByPolygonVertex;
					}
					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);
					}

					FbxVector2 uv = uvElement->GetDirectArray().GetAt(uvIndex);
					FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
					
					/*mVertices[lIndexByPolygonVertex] =
					{
						static_cast<float>(vertexElement[vertexIndex].mData[0]),
						static_cast<float>(vertexElement[vertexIndex].mData[1]),
						static_cast<float>(vertexElement[vertexIndex].mData[2])
					};
					mUVs[lIndexByPolygonVertex] =
					{
						static_cast<float>(uv[0]),
						static_cast<float>(uv[1])
					};*/
					mNormals[lIndexByPolygonVertex] =
					{
						static_cast<float>(lNormal[0]),
						static_cast<float>(lNormal[1]),
						static_cast<float>(lNormal[2])
					};
					
					lIndexByPolygonVertex++;
				}
			}
			
			mVertexCount = lIndexByPolygonVertex;
			break;
		}
		default:
		{
			break;
		}
		}
	}
	/*int i, lCount = node->GetChildCount();
	for (i = 0; i < lCount; i++)
	{
		GetNormals(node->GetChild(i));
	}*/
}