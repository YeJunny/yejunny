#include "FBXLoader.h"
#include <fstream>
#include <iomanip>

#define FBX_WRITE

FBXLoader::FBXLoader()
{
}

FBXLoader::~FBXLoader()
{
}

HRESULT FBXLoader::LoadFbx(std::vector<VertexElements>* elements, const char* fileName)
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

	FbxGeometryConverter geometryConvert(manager);
	bSuccess = geometryConvert.Triangulate(fbxScene, true);
	assert(bSuccess);

	importer->Destroy();

	mFileName = fileName;
	mElements = elements;

	FbxNode* fbxRootNode = fbxScene->GetRootNode();
	assert(fbxRootNode);
	GetVerticesUVsNormalsRecursive(fbxRootNode);

	manager->Destroy();
	return S_OK;
}

void FBXLoader::GetVerticesUVsNormalsRecursive(FbxNode* node)
{
#ifdef FBX_WRITE
	static int number = 0;
	char numberStr[10];
	_itoa_s(number, numberStr, 10);
#endif
	static size_t vertexCount = 0;

	if (!node)
	{
		return;
	}

	int childCount = node->GetChildCount();
#ifdef FBX_WRITE
	std::string fileNameString = mFileName + "_folder\\" + std::string(numberStr) + "_Info_" + node->GetName() + +".txt";
	number++;
	std::ofstream writeFile(fileNameString.data());
	if (writeFile.is_open())
	{
		writeFile
			<< "==============================================================================\n"
			<< "	First Load: Vertex, UV, Normal		" << fileNameString << "\n"
			<< "==============================================================================\n"
			<< "get " << node->GetName() << " ... OK\n"
			<< "childCount is ... " << childCount << "\n";
	}
#endif
	for (int i = 0; i < childCount; ++i)
	{
		FbxNode* fbxChildNode = node->GetChild(i);
		assert(fbxChildNode);
#ifdef FBX_WRITE
		if (writeFile.is_open())
		{
			writeFile
				<< "get child " << i << " name is ... " << fbxChildNode->GetName() << "\n";
		}
#endif
		if (!fbxChildNode->GetNodeAttribute() || fbxChildNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::EType::eNull)
		{
#ifdef FBX_WRITE
			if (writeFile.is_open())
			{
				writeFile
					<< "\tchild " << i << " Attribute is ... nullptr\n";
			}
#endif
			GetVerticesUVsNormalsRecursive(fbxChildNode);

			continue;
		}

		FbxNodeAttribute::EType attributeType = fbxChildNode->GetNodeAttribute()->GetAttributeType();
		FbxMesh* mesh;
		switch (attributeType)
		{
		case FbxNodeAttribute::eMesh:
		{

#ifdef FBX_WRITE
			if (writeFile.is_open())
			{
				writeFile
					<< "\tchild " << i << " NodeAttribute is ... " << "eMesh\n";
			}
#endif
			FbxGeometryConverter geometryConvert(node->GetFbxManager());
			//mesh = static_cast<FbxMesh*>(geometryConvert.Triangulate(fbxChildNode->GetNodeAttribute(), true));
			mesh = static_cast<FbxMesh*>(fbxChildNode->GetNodeAttribute());

			break;
		}
		default:
		{

#ifdef FBX_WRITE
			if (writeFile.is_open())
			{
				writeFile
					<< "\tchild " << i << " NodeAttributeType is ... " << attributeType << " ( eUnknown = 0, eNull = 1, eMarker = 2, eSkeleton = 3, eMesh = 4, eNurbs = 5, ePatch = 6, eCamera = 7, eCameraStereo = 8, eCameraSwitcher = 9, eLight = 10, eOpticalReference = 11, eOpticalMarker = 12, eNurbsCurve = 13, eTrimNurbsSurface = 14, eBoundary = 15, eNurbsSurface = 16, eShape = 17, eLODGroup = 18, eSubDiv = 19, eCachedEffect = 20, Line = 21 )\n";
			}
#endif
			continue;
		}
		}

		//////////////// NodeAttributeType == eMesh ///////////////////
		assert(attributeType == FbxNodeAttribute::eMesh, L"FBXLoader, attributeType all not eMesh");
		assert(mesh);

		FbxVector4* vertices = mesh->GetControlPoints();
		assert(vertices);
		int polygonCount = mesh->GetPolygonCount();

		FbxGeometryElementNormal* normalElements = mesh->GetElementNormal();
		assert(normalElements);

#ifdef FBX_WRITE
		if (writeFile.is_open())
		{
			writeFile
				<< "\tchild " << i << " mesh's ploygonCount is ... " << polygonCount << "\n"
				<< "\tchild " << i << " mesh's list of vertex, uv and normal ...\n"
				<< "\t\t\t\t\t\t\t\t\t\t\t\t\t\t" << "Vertex\t\t\t\t\t\t\t\t\t\t\t\t" << "UV\t\t\t\t\t\t\t\t\t\t\t\t" << "Normal\n";
		}
#endif

		switch (normalElements->GetMappingMode())
		{
		case FbxGeometryElement::eByPolygonVertex:
		{
			for (size_t polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
			{
				size_t polygonSize = mesh->GetPolygonSize(polygonIndex);

				for (int k = 0; k < polygonSize; ++k)
				{
					int vertexIndex = mesh->GetPolygonVertex(polygonIndex, k);

					FbxGeometryElementUV* eUV = mesh->GetElementUV();
					assert(eUV);
					int uvIndex = mesh->GetTextureUVIndex(polygonIndex, k);
					FbxVector2 uv = eUV->GetDirectArray().GetAt(uvIndex);

					int normalIndex = 0;
					if (normalElements->GetReferenceMode() == FbxGeometryElement::eDirect)
					{
						normalIndex = mVertexCount;
					}
					if (normalElements->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						normalIndex = normalElements->GetIndexArray().GetAt(mVertexCount);
					}
					FbxVector4 normalFbxVector4 = normalElements->GetDirectArray().GetAt(normalIndex);

					VertexElements vertexElement =
					{
						XMFLOAT3(
							static_cast<float>(vertices[vertexIndex].mData[0]),
							static_cast<float>(vertices[vertexIndex].mData[1]),
							static_cast<float>(vertices[vertexIndex].mData[2])),

						XMFLOAT2(static_cast<float>(uv[0]), static_cast<float>(uv[1])),

						XMFLOAT3(
							static_cast<float>(normalFbxVector4[0]),
							static_cast<float>(normalFbxVector4[1]), 
							static_cast<float>(normalFbxVector4[2]))
					};
					mElements->push_back(vertexElement);

					vertexCount++;

#ifdef FBX_WRITE
					if (writeFile.is_open())
					{
						writeFile
							<< "\t\t" << "vertex[" << std::setw(4) << polygonIndex << "][" << std::setw(4) << k << "]\t|\t" 
							<< std::setw(15) << vertices[vertexIndex].mData[0] << ", " << std::setw(15) << vertices[vertexIndex].mData[1] << ", " << std::setw(15) << vertices[vertexIndex].mData[2]
							<< "\t|\t" << std::setw(15) << uv[0] << ", " << std::setw(15) << uv[1] 
							<< "\t|\t" << std::setw(15) << normalFbxVector4[0] << ", " << std::setw(15) << normalFbxVector4[1] << ", " << std::setw(15) << normalFbxVector4[2] << "\n";
					}
#endif
				}
			}
			break;
		}
		case FbxGeometryElement::eByControlPoint:
		{
			assert(false);
		}
		default:
		{
			break;
		}
		}
	}
	assert(vertexCount);
	mVertexCount = vertexCount;
#ifdef FBX_WRITE
	writeFile.close();
#endif
}

unsigned int FBXLoader::GetVertexCount() const
{
	return mVertexCount;
}