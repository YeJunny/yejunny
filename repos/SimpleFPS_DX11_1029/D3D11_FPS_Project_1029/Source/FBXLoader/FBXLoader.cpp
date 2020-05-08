#include "FBXLoader.h"

FBXLoader::FBXLoader()
{
}

FBXLoader::~FBXLoader()
{
}

HRESULT FBXLoader::LoadFbx(CharacterFbxData* charFbxData, const char* fileName)
{
	// Basic Settings

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

	FbxNode* rootNode = fbxScene->GetRootNode();
	assert(rootNode);


	// Member Variables

	mScene = fbxScene;
	mFileName = fileName;
	mCharFbxData = charFbxData;
	mCharFbxData->bHasAnimation = false;


	// Actullay Loading

	LoadByAttributeRecursive(rootNode);


	// CleanUp

	fbxScene->Destroy();
	ios->Destroy();
	manager->Destroy();


	return S_OK;
}

void FBXLoader::LoadByAttributeRecursive(FbxNode* node)
{
#ifdef FBX_WRITE
	static int recursiveNumber = 0;
	char numberStr[10];
	_itoa_s(recursiveNumber, numberStr, 10);
#endif

	if (!node)
	{
		return;
	}

	unsigned int childCount = node->GetChildCount();

#ifdef FBX_WRITE
	std::string fileNameString = mFileName + "_folder\\" + std::string(numberStr) + "_Info_" + node->GetName() + +".txt";
	++recursiveNumber;
	mWriteFile.open(fileNameString.data());
	if (mWriteFile.is_open())
	{
		mWriteFile
			<< "==============================================================================\n"
			<< "	Load Fbx - Vertex, UV, Normal, Animation, etc ...		" << fileNameString << "\n"
			<< "==============================================================================\n"
			<< "Get " << node->GetName() << " ... OK\n"
			<< "childCount is ... " << childCount << "\n";
	}
#endif

	for (int childIndex = 0; childIndex < childCount; ++childIndex)
	{
		FbxNode* childNode = node->GetChild(childIndex);
		assert(childNode);

#ifdef FBX_WRITE
		if (mWriteFile.is_open())
		{
			mWriteFile
				<< "Get Child Node " << childIndex << " Name is ... " << childNode->GetName() << "\n";
		}
#endif

		// Child Node is Null Or Root Or Parent Node.

		if (!childNode->GetNodeAttribute() || childNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::EType::eNull)
		{

#ifdef FBX_WRITE
			if (mWriteFile.is_open())
			{
				mWriteFile
					<< "\tChild " << childIndex << " Attribute is ... nullptr\n";
			}
#endif

			// Is it Root Or Parent Node?

			LoadByAttributeRecursive(childNode);

			continue;
		}

		FbxMesh* mesh = nullptr;
		FbxNodeAttribute::EType NodeAttributeType = childNode->GetNodeAttribute()->GetAttributeType();

		switch (NodeAttributeType)
		{
		case FbxNodeAttribute::eMesh:
		{

#ifdef FBX_WRITE
			if (mWriteFile.is_open())
			{
				mWriteFile
					<< "\tChild " << childIndex << " Node Attribute is ... " << "eMesh\n";
			}
#endif

			mesh = static_cast<FbxMesh*>(childNode->GetNodeAttribute());
			assert(mesh);

			LoadAnimation(childNode, childIndex);
			LoadByeMesh(mesh, childIndex);

			break;
		}
		case FbxNodeAttribute::eSkeleton:
		{

#ifdef FBX_WRITE
			if (mWriteFile.is_open())
			{
				mWriteFile
					<< "\tChild " << childIndex << " Node Attribute is ... " << "eSkeleton\n";
			}
#endif

			LoadByeSkeleton(childNode, 0, 0, -1);

			continue;
		}
		default:
		{

#ifdef FBX_WRITE
			if (mWriteFile.is_open())
			{
				mWriteFile
					<< "\tChild " << childIndex << " Node AttributeType is ... " << NodeAttributeType << " ( eUnknown = 0, eNull = 1, eMarker = 2, eSkeleton = 3, eMesh = 4, eNurbs = 5, ePatch = 6, eCamera = 7, eCameraStereo = 8, eCameraSwitcher = 9, eLight = 10, eOpticalReference = 11, eOpticalMarker = 12, eNurbsCurve = 13, eTrimNurbsSurface = 14, eBoundary = 15, eNurbsSurface = 16, eShape = 17, eLODGroup = 18, eSubDiv = 19, eCachedEffect = 20, Line = 21 )\n";
			}
#endif

			continue;
		}
		} // End Attribute Switch.
	} // End Child Index Loop.




#ifdef FBX_WRITE
	mWriteFile.close();
#endif

}

void FBXLoader::LoadByeSkeleton(FbxNode* node, int depth, int index, int parentIndex)
{
	static int myIndex = 0;


#ifdef FBX_WRITE
	if (mWriteFile.is_open())
	{
		mWriteFile
			<< "\t\tChild " << myIndex << "(Index) is... " << node->GetName() <<
			"\tparentIndex : " << parentIndex << " depth : " << depth << "\n";
	}
#endif

	BoneData skeleton(node->GetName(), parentIndex);
	mCharFbxData->AnimSkeletonData.push_back(skeleton);

	++myIndex;

	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		LoadByeSkeleton(node->GetChild(i), depth + 1, myIndex, index);
	}
}

int FBXLoader::GetBoneIndexByName(FbxString name) const
{
	vector<BoneData>* skeletonData = &(mCharFbxData->AnimSkeletonData);
	int skeletonNum = skeletonData->size();
	for (int i = 0; i < skeletonNum; ++i)
	{
		if (skeletonData->operator[](i).GetName() == name)
		{
			return i;
		}
	}

	assert(false);
}

XMMATRIX FBXLoader::ConvertFbxAMatrixToXMMATRIX(FbxAMatrix& fbxAMatrix)
{
	XMMATRIX convertedMatrix;
	for (int i = 0; i < 4; ++i)
	{
		XMVECTOR rowVector =
		{
			fbxAMatrix.Get(i, 0),
			fbxAMatrix.Get(i, 1),
			fbxAMatrix.Get(i, 2),
			fbxAMatrix.Get(i, 3),
		};
		convertedMatrix.r[i] = rowVector;
	}
	return convertedMatrix;
}

void FBXLoader::LoadAnimation(FbxNode* node, int childIndex)
{
	// Node Attribute is eMesh

	FbxMesh* mesh = node->GetMesh();
	FbxVector4* controlPoints = mesh->GetControlPoints();
	assert(controlPoints);

	// Save Variable Fbx Control Point.
	int controlPointCount = mesh->GetControlPointsCount();
	for (int i = 0; i < controlPointCount; ++i)
	{
		mCharFbxData->FbxControlPointData[i].SetPosition(&controlPoints[i]);
	}

	int deformerCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (deformerCount <= 0)
	{
#ifdef FBX_WRITE
		if (mWriteFile.is_open())
		{
			mWriteFile <<
				"\tNo Animation Data!\n";
		}
#endif
		return;
	}

#ifdef FBX_WRITE
	if (mWriteFile.is_open())
	{
		mWriteFile <<
			"\tChild " << childIndex << " deformerCount is ... " << deformerCount << "\n" <<
			"\tIf Missing Skeleton Index, No Skinning Data." << "\n";
	}

	FbxLongLong startTime;
	FbxLongLong endTime;

#endif


	mCharFbxData->bHasAnimation = true;

	vector<vector<XMMATRIX>> keyframeMatricesPerBone;
	vector<XMMATRIX> keyframe;

	const FbxTime::EMode fbxFreames = FbxTime::eFrames120;
	const int boneCount = mCharFbxData->AnimSkeletonData.size();
	mCharFbxData->InverseMatrixAnim.reserve(boneCount);
	mCharFbxData->KeyframeMatrixPerTime.reserve(boneCount);
	keyframe.reserve(boneCount);
	keyframeMatricesPerBone.reserve(boneCount);

	for (int deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
	{
		FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!skin)
		{
			continue;
		}

		int clusterCount = skin->GetClusterCount();
		for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
		{
			FbxCluster* cluster = skin->GetCluster(clusterIndex);
			FbxNode* clusterNode = cluster->GetLink();
			FbxString name = clusterNode->GetName();
			int boneIndex = GetBoneIndexByName(name);

#ifdef FBX_WRITE
			if (mWriteFile.is_open())
			{
				mWriteFile <<
					"\t\tdeformerIndex : " << deformerIndex << ", " <<
					"clusterIndex : " << clusterIndex << ", " <<
					"Node Name : " << name << ", Skeleton Index: " << boneIndex << "\n";
			}
#endif

			FbxAMatrix geometryTransformationMatrix = FbxAMatrix(
				node->GetGeometricTranslation(FbxNode::eSourcePivot),
				node->GetGeometricRotation(FbxNode::eSourcePivot),
				node->GetGeometricScaling(FbxNode::eSourcePivot));
			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;

			cluster->GetTransformMatrix(transformMatrix);
			cluster->GetTransformLinkMatrix(transformLinkMatrix); // Bone Space => World Space
			FbxAMatrix globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransformationMatrix;

			XMMATRIX globalInverseMatrix = ConvertFbxAMatrixToXMMATRIX(globalBindposeInverseMatrix);
			mCharFbxData->InverseMatrixAnim.push_back(globalInverseMatrix);

			// Relative Vertex = globalBindPoseInverseMatrix * Bindpose Vertex

			unsigned int indicesNum = cluster->GetControlPointIndicesCount();
			for (unsigned int indices = 0; indices < indicesNum; ++indices)
			{
				BlendControlPointData blendData;
				blendData.BlendIndex = clusterIndex;
				blendData.BlendWeight = cluster->GetControlPointWeights()[indices];
				mCharFbxData->FbxControlPointData[cluster->GetControlPointIndices()[indices]].GetBlendData()->push_back(blendData);
			}


			FbxAnimStack* animStack = mScene->GetSrcObject<FbxAnimStack>(0);
			FbxString animStackName = animStack->GetName();
			FbxTakeInfo* takeInfo = mScene->GetTakeInfo(animStackName);
			FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeInfo->mLocalTimeSpan.GetStop();

#ifdef FBX_WRITE
			startTime = start.GetFrameCount(fbxFreames);
			endTime = end.GetFrameCount(fbxFreames);
#endif


			keyframe.clear();
			mCharFbxData->EndTime = end.GetFrameCount(fbxFreames);

			for (FbxLongLong time = start.GetFrameCount(fbxFreames); time <= end.GetFrameCount(fbxFreames); ++time)
			{
				FbxTime currentTime;
				currentTime.SetFrame(time, fbxFreames);

				FbxAMatrix transformOffset = node->EvaluateGlobalTransform(currentTime) * geometryTransformationMatrix;
				FbxAMatrix globalTransform = transformOffset.Inverse() * clusterNode->EvaluateGlobalTransform(currentTime);
				XMMATRIX globalMatrix = ConvertFbxAMatrixToXMMATRIX(globalTransform);

				// Bone's Keyframe Matrics.
				keyframe.push_back(globalMatrix);
			}

			// Keyframe Matrics Per Bone.
			keyframeMatricesPerBone.push_back(keyframe);

		} // End Loop of clusterIndex


		BlendControlPointData blendData;
		blendData.BlendIndex = 0;
		blendData.BlendWeight = 0;

		for (auto iter = mCharFbxData->FbxControlPointData.begin(); iter != mCharFbxData->FbxControlPointData.end(); ++iter)
		{
			for (unsigned int i = iter->second.GetBlendData()->size(); i < 4; ++i)
			{
				iter->second.GetBlendData()->push_back(blendData);
			}

#ifdef FBX_WRITE
			if (mWriteFile.is_open())
			{
				mWriteFile <<
					"\tControl Point Data : \n" <<
					"\t\t[Time] : " << startTime << ", " << endTime << "\n" <<
					"\t\t[Position] : " << iter->second.GetPosition().x << ", " << iter->second.GetPosition().y << ", " << iter->second.GetPosition().z << "\n" <<
					"\t\t[Skeletons] : \n";
				for (auto blendIter = iter->second.GetBlendData()->begin(); blendIter != iter->second.GetBlendData()->end(); ++blendIter)
				{
					mWriteFile <<
						"\t\t\tBlend Index : " << blendIter->BlendIndex << ", Blending Weight : " << blendIter->BlendWeight << "\n";
				}
			}
#endif
		}

#ifdef FBX_WRITE
		mWriteFile <<
			"\t======= Final Matrix =======\n";
#endif

		vector<XMMATRIX> keyframePerTime;
		for (int frameNum = 0; frameNum < keyframeMatricesPerBone[0].size(); ++frameNum)
		{
#ifdef FBX_WRITE
			mWriteFile <<
				"\t\tframe Number : " << frameNum << "\n" <<
				"\t\tMatrix Content : \n";
#endif

			for (int clusterNum = 0; clusterNum < clusterCount; ++clusterNum)
			{
				XMMATRIX finalTransform = XMMatrixTranspose(mCharFbxData->InverseMatrixAnim[clusterNum] * keyframeMatricesPerBone[clusterNum][frameNum]);

				keyframePerTime.push_back(finalTransform);

#ifdef FBX_WRITE
				if (mWriteFile.is_open())
				{
					XMFLOAT4 element;
					for (int j = 0; j < 4; ++j)
					{
						XMStoreFloat4(&element, keyframePerTime[clusterNum].r[j]);
						mWriteFile <<
							"\t\t\t|" <<
							element.x << ", " <<
							element.y << ", " <<
							element.z << ", " <<
							element.w << "|\n";
					}
					mWriteFile << "\n";
				}
#endif
			}

			mCharFbxData->KeyframeMatrixPerTime.push_back(keyframePerTime);

			keyframePerTime.clear();
		}

	} // End Loop of deformerIndex
}

void FBXLoader::LoadByeMesh(FbxMesh* mesh, int childIndex)
{
	int polygonSize = mesh->GetPolygonCount();
	FbxGeometryElementNormal* normalElements = mesh->GetElementNormal();
	assert(normalElements);

	FbxGeometryElementUV* eUV = mesh->GetElementUV();
	assert(eUV);


#ifdef FBX_WRITE

	if (mWriteFile.is_open())
	{
		mWriteFile
			<< "\tChild " << childIndex << " Mesh's Ploygon Count is ... " << polygonSize << "\n"
			<< "\tChild " << childIndex << " Mesh's List of Vertex, UV and Normal ...\n"
			<< "\t\t\t\t\t\t\t\t\t\t\t\t\t\t" << "Vertex\t\t\t\t\t\t\t\t\t\t\t\t" << "UV\t\t\t\t\t\t\t\t\t\t\t\t" << "Normal\n";
	}

#endif


	// Get Vertices, UVs, Normals.
	std::vector<PureVertexData> monoMeshElement;

	int vertexCount = 0;
	switch (normalElements->GetMappingMode())
	{
	case FbxGeometryElement::eByPolygonVertex:
	{
		for (size_t polygonIndex = 0; polygonIndex < polygonSize; ++polygonIndex)
		{
			// Maybe polygonElementSize is 3.
			size_t polygonElementSize = mesh->GetPolygonSize(polygonIndex);

			for (int polygonElementIndex = 0; polygonElementIndex < polygonElementSize; ++polygonElementIndex)
			{
				// Duplicated index(vertexIndex) is possible.
				int vertexIndex = mesh->GetPolygonVertex(polygonIndex, polygonElementIndex);
				int uvIndex = mesh->GetTextureUVIndex(polygonIndex, polygonElementIndex);
				FbxVector2 uv = { 0, 0 };
				if (eUV)
				{
					uv = eUV->GetDirectArray().GetAt(uvIndex);
				}

				int normalIndex = 0;
				if (normalElements->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					normalIndex = vertexCount;
				}
				else if (normalElements->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				{
					normalIndex = normalElements->GetIndexArray().GetAt(vertexCount);
				}
				FbxVector4 normalFbxVector4 = normalElements->GetDirectArray().GetAt(normalIndex);


				PureVertexData vertexElement;
				XMFLOAT3 position = mCharFbxData->FbxControlPointData[vertexIndex].GetPosition();
				vertexElement.Pos = XMFLOAT4(position.x, position.y, position.z, 1.0f);
				vertexElement.UV = XMFLOAT2(static_cast<float>(uv[0]), 1 - static_cast<float>(uv[1]));
				vertexElement.Normal =
					XMFLOAT3(
						static_cast<float>(normalFbxVector4[0]),
						static_cast<float>(normalFbxVector4[1]),
						static_cast<float>(normalFbxVector4[2])
					);
				if (mCharFbxData->bHasAnimation)
				{
					vertexElement.Weights =
						XMFLOAT4(
							mCharFbxData->FbxControlPointData[vertexIndex].GetBlendData()->data()[0].BlendWeight,
							mCharFbxData->FbxControlPointData[vertexIndex].GetBlendData()->data()[1].BlendWeight,
							mCharFbxData->FbxControlPointData[vertexIndex].GetBlendData()->data()[2].BlendWeight,
							mCharFbxData->FbxControlPointData[vertexIndex].GetBlendData()->data()[3].BlendWeight);

					vertexElement.Indices =
						XMINT4(
							mCharFbxData->FbxControlPointData[vertexIndex].GetBlendData()->data()[0].BlendIndex,
							mCharFbxData->FbxControlPointData[vertexIndex].GetBlendData()->data()[1].BlendIndex,
							mCharFbxData->FbxControlPointData[vertexIndex].GetBlendData()->data()[2].BlendIndex,
							mCharFbxData->FbxControlPointData[vertexIndex].GetBlendData()->data()[3].BlendIndex);
				}

				++vertexCount;

				monoMeshElement.push_back(vertexElement);


#ifdef FBX_WRITE
				if (mWriteFile.is_open())
				{
					mWriteFile
						<< "\t\t" << "vertex[" << std::setw(4) << polygonIndex << "][" << std::setw(4) << polygonElementIndex << "]\t|\t"
						<< std::setw(15) << position.x << ", " << std::setw(15) << position.y << ", " << std::setw(15) << position.z
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
		break;
	}
	default:
	{
		assert(false);
		break;
	}
	}

	mCharFbxData->PureVertexData.push_back(monoMeshElement);
}

unsigned int FBXLoader::GetVertexCount() const
{
	return mVertexCount;
}
