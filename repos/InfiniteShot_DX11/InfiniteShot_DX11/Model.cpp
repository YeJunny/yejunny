#include "Engine.h"
#include "Model.h"
using namespace Model_;

HRESULT Model::ImportModelFromFile(std::string modelFileName, Engine* engine)
{
	// Create memory manager
	FbxManager* fbxManager = nullptr;
	fbxManager = FbxManager::Create();
	AssertIsSuccess(fbxManager, "Fbx Sdk Create Manager - Failed");

	FbxIOSettings* fbxIOSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
	AssertIsSuccess(fbxIOSettings, "Fbx Sdk Create IOSettings - Failed");

	fbxManager->SetIOSettings(fbxIOSettings);

	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	AssertIsSuccess(fbxImporter, fbxImporter->GetStatus().GetErrorString());

	bool bIsSuccess = fbxImporter->Initialize(modelFileName.c_str(), -1, fbxManager->GetIOSettings());
	AssertIsSuccess(bIsSuccess, fbxImporter->GetStatus().GetErrorString());

	FbxScene* fbxScene = FbxScene::Create(fbxManager, "myScene");
	AssertIsSuccess(fbxScene, "Fbx Sdk Create Scene - Failed");

	bIsSuccess = fbxImporter->Import(fbxScene);
	AssertIsSuccess(bIsSuccess, "Fbx Sdk Importer Import Scene - Failed");

	fbxImporter->Destroy();

	FbxGeometryConverter geometryConverter(fbxManager);
	bIsSuccess = geometryConverter.Triangulate(fbxScene, true);
	AssertIsSuccess(bIsSuccess, "Fbx Sdk Geometry Converter Triangulate - Failed");
	
	FbxNode* rootNode = fbxScene->GetRootNode();
	AssertIsSuccess(rootNode, "Fbx Sdk Scene Get Root Node - Failed");

	mScene = fbxScene;

	for (int i = modelFileName.size() - 1; i >= 0; --i)
	{
		if (modelFileName[i] == '\\')
		{
			break;
		}

		modelFileName.pop_back();
	}

	mLogDirectory = modelFileName + "Log\\";
	modelFileName.clear();


	mEngine = engine;
	mEngine->SetPercent(0);


	std::wstring modelFileNameW;
	modelFileNameW.assign(mLogDirectory.begin(), mLogDirectory.end());
	int result = _wmkdir(modelFileNameW.c_str());
	if (result == 0)
	{
		OutputDebugStringA("_wkdir Successfule!\n");
	}
	else if (errno != EEXIST)
	{
		AssertIsSuccess(false, "Win32 _wmkdir - Failed");
	}
	

	mEngine->SetPercent(10);


	ImportBaseRecursive(rootNode);
	ImportVertexInfo();


	mEngine->SetPercent(100);


	fbxManager->Destroy();

	return S_OK;
}

void Model_::Model::ReleaseModel()
{
	mVertices.clear();
	mAnimInvMatrix.clear();
	mAnimKeyFramePerTime.clear();
	mSkele.clear();
	mContrlPoint.clear();
}

HRESULT Model::ImportBaseRecursive(FbxNode* pNode)
{
	//=======================================//
#ifdef LOG_WRITE_FBX_DATA
	/*static int recurNum = 0;
	char tempStr[10];
	_itoa_s(recurNum, tempStr, 10);*/
#endif
	//========================================//


	if (!pNode)
	{
		return S_OK;
	}

	UINT childAllNum = pNode->GetChildCount();
	if (childAllNum <= 0)
	{
		return S_OK;
	}

	//============================================================//
#ifdef LOG_WRITE_FBX_DATA
	std::string logDirFileName = mLogDirectory + "Log_" + pNode->GetName() + ".txt";
	//++recurNum;
	mLogStream.open(logDirFileName.c_str());
	if (mLogStream.is_open())
	{
		mLogStream
			<< "+------------------------------------------------------------------------------------------+\n"
			<< "|	Description	: Import Fbx Log File |\n"
			<< "|	Data Set	: Vertices, TexCoord, Normal, Animation, ... |\n"
			<< "|	File Name	: " + logDirFileName << "|\n"
			<< "+------------------------------------------------------------------------------------------+\n"
			<< "Do!		Get " << pNode->GetName() << "\n"
			<< "[Result]		childAllNum : " << childAllNum << "\n";
	}
#endif
	//=============================================================//


	for (UINT childIndex = 0; childIndex < childAllNum; ++childIndex)
	{
		FbxNode* childNode = pNode->GetChild(childIndex);
		AssertIsSuccess(childNode, "Fbx Sdk Get Chlid Node Empty.");


		//=======================================================================//
#ifdef LOG_WRITE_FBX_DATA
		if (mLogStream.is_open())
		{
			mLogStream
				<< "Do!		Get Attribute\n"
				<< "[Result]		child Node Index : " << childIndex	<< " | child Mode Name : " << childNode->GetName() << "\n";
		}
#endif
		//=======================================================================//


		FbxNodeAttribute* currNodeAttrib = childNode->GetNodeAttribute();
		FbxNodeAttribute::EType currNodeAttribType = currNodeAttrib->GetAttributeType();
		if (!currNodeAttrib || currNodeAttribType == FbxNodeAttribute::EType::eNull)
		{
			//============================================================================//
#ifdef LOG_WRITE_FBX_DATA
			if (mLogStream.is_open())
			{
				mLogStream
					<< "[Result]		this attribute : nullptr or parent node\n";
			}
#endif
			//=============================================================================//


			ImportBaseRecursive(childNode);


			continue;
		}


		if (currNodeAttribType == FbxNodeAttribute::eSkeleton)
		{
			//====================================================================================//
#ifdef LOG_WRITE_FBX_DATA
			if (mLogStream.is_open())
			{
				mLogStream
					<< "\t[Result]		this attribute : eSkeleton (child Index:" << childIndex << ")\n";
			}
#endif
			//===================================================================================//


			ImportSkeletonRecursive(childNode);

		}
		else if (currNodeAttribType == FbxNodeAttribute::eMesh)
		{
			mMeshNodes.push_back(childNode);
		}
	}


	mEngine->SetPercent(30);


	return S_OK;
}

HRESULT Model_::Model::ImportVertexInfo()
{
	while (!mMeshNodes.empty())
	{
		FbxNode* currNode = mMeshNodes.back();
		mMeshNodes.pop_back();

		if (!currNode)
		{
			return S_OK;
		}


		//======================================================================//
#ifdef LOG_WRITE_FBX_DATA
		if (mLogStream.is_open())
		{
			mLogStream
				<< "\t[Result]		attribute : eMesh\n";
		}
#endif
		//======================================================================//


		FbxMesh* mesh = static_cast<FbxMesh*>(currNode->GetNodeAttribute());
		AssertIsSuccess(currNode, "Fbx Sdk Child Node Get Node Attribute - Failed");


		mEngine->AddPercent(5);


		ImportAnim(currNode);
		ImportVertexInfoInternal(mesh);
	}


#ifdef LOG_WRITE_FBX_DATA
	mLogStream.close();
#endif

	return S_OK;
}

HRESULT Model::ImportVertexInfoInternal(FbxMesh* pMesh)
{
	int polygonSize = pMesh->GetPolygonCount();
	FbxGeometryElementNormal* normalElements = pMesh->GetElementNormal();
	AssertIsSuccess(normalElements, "Fbx Sdk Mesh Get Element Normal - Failed");

	FbxGeometryElementUV* eUV = pMesh->GetElementUV();
	AssertIsSuccess(normalElements, "Fbx Sdk Mesh Get Element UV - Failed");


	//============================================================================================================================//
#ifdef LOG_WRITE_FBX_DATA

	if (mLogStream.is_open())
	{
		mLogStream
			<< "\t[Result]		Mesh's Ploygon Count is ... " << polygonSize << "\n"
			<< "\t[Result]		Mesh's List of Vertex, UV and Normal ...\n"
			<< "\t\t\t\t\t\t\t\t\t\t\t\t\t\t[Result]		" << "Vertex\t\t\t\t\t\t\t\t\t\t\t\t" << "UV\t\t\t\t\t\t\t\t\t\t\t\t" << "Normal\n";
	}
#endif
	//============================================================================================================================//


	// Get Vertices, UVs, Normals.
	std::vector<Vertex> monoMeshElement;

	int vertexCount = 0;
	switch (normalElements->GetMappingMode())
	{
	case FbxGeometryElement::eByPolygonVertex:
	{
		for (size_t polygonIndex = 0; polygonIndex < polygonSize; ++polygonIndex)
		{
			// Maybe polygonElementSize is 3.
			size_t polygonElementSize = pMesh->GetPolygonSize(polygonIndex);

			for (int polygonElementIndex = 0; polygonElementIndex < polygonElementSize; ++polygonElementIndex)
			{
				// Duplicated index(vertexIndex) is possible.
				int vertexIndex = pMesh->GetPolygonVertex(polygonIndex, polygonElementIndex);
				int uvIndex = pMesh->GetTextureUVIndex(polygonIndex, polygonElementIndex);
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


				Vertex vertexElement;
				DirectX::XMFLOAT3& position = mContrlPoint[vertexIndex].Pos;
				vertexElement.Pos = DirectX::XMFLOAT3(position.x, position.y, position.z);
				vertexElement.TexCoord = DirectX::XMFLOAT2(static_cast<float>(uv[0]), 1 - static_cast<float>(uv[1]));
				vertexElement.Normal =
					DirectX::XMFLOAT3(
						static_cast<float>(normalFbxVector4[0]),
						static_cast<float>(normalFbxVector4[1]),
						static_cast<float>(normalFbxVector4[2])
					);
				if (mbHasAnim)
				{
					vertexElement.Weights =
						DirectX::XMFLOAT4(
							mContrlPoint[vertexIndex].TempAnimInfo.data()[0].Weight,
							mContrlPoint[vertexIndex].TempAnimInfo.data()[1].Weight,
							mContrlPoint[vertexIndex].TempAnimInfo.data()[2].Weight,
							mContrlPoint[vertexIndex].TempAnimInfo.data()[3].Weight);

					vertexElement.Indices =
						DirectX::XMINT4(
							mContrlPoint[vertexIndex].TempAnimInfo.data()[0].Index,
							mContrlPoint[vertexIndex].TempAnimInfo.data()[1].Index,
							mContrlPoint[vertexIndex].TempAnimInfo.data()[2].Index,
							mContrlPoint[vertexIndex].TempAnimInfo.data()[3].Index);
				}

				++vertexCount;

				monoMeshElement.push_back(vertexElement);


				//======================================================================================================================//
#ifdef LOG_WRITE_FBX_DATA
				if (mLogStream.is_open())
				{
					mLogStream
						<< "\t\t[Result]		" << "vertex[" << std::setw(4) << polygonIndex << "][" << std::setw(4) << polygonElementIndex << "]\t|\t"
						<< std::setw(15) << position.x << ", " << std::setw(15) << position.y << ", " << std::setw(15) << position.z
						<< "\t|\t" << std::setw(15) << uv[0] << ", " << std::setw(15) << uv[1]
						<< "\t|\t" << std::setw(15) << normalFbxVector4[0] << ", " << std::setw(15) << normalFbxVector4[1] << ", " << std::setw(15) << normalFbxVector4[2] << "\n";
				}
#endif
				//======================================================================================================================//


			}
		}


		mEngine->AddPercent(15);


		break;
	}
	case FbxGeometryElement::eByControlPoint:
	{
		AssertIsSuccess(false, "Not Supported eByControlPoint.");
		break;
	}
	default:
	{
		AssertIsSuccess(false, "Not Supported Attribute.");
		break;
	}
	}


	mVertices.push_back(monoMeshElement);


	return S_OK;
}

HRESULT Model::ImportSkeletonRecursive(FbxNode* pNode, int depth, int parentIndex, int index)
{
	static int skeleIndex = 0;

	Skeleton skeleton;
	skeleton.Name = pNode->GetName();
	skeleton.ParentIndex = parentIndex;

	mSkele.push_back(skeleton);

	++skeleIndex;

	//===============================================================================================//
#ifdef LOG_WRITE_FBX_DATA
	if (mLogStream.is_open())
	{
		mLogStream
			<< "\t\t[Result]		this Skele Node Name : " << skeleton.Name << " | skele Index:" << skeleIndex << ") | parent Index : " << parentIndex << " | depth : " << depth << "\n";
	}
#endif
	//===============================================================================================//


	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		ImportSkeletonRecursive(pNode->GetChild(i), depth + 1, skeleIndex, index);
	}

	return S_OK;
}

HRESULT Model::ImportAnim(FbxNode* pNode)
{
	// Node Attribute is eMesh

	FbxMesh* pMesh = pNode->GetMesh();
	FbxVector4* controlPoints = pMesh->GetControlPoints();
	assert(controlPoints);

	// Save Variable Fbx Control Point.
	int controlPointCount = pMesh->GetControlPointsCount();
	for (int i = 0; i < controlPointCount; ++i)
	{
		mContrlPoint[i].Pos.x = (controlPoints[i]).mData[0];
		mContrlPoint[i].Pos.y = (controlPoints[i]).mData[1];
		mContrlPoint[i].Pos.z = (controlPoints[i]).mData[2];
	}

	int deformerAllNum = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (deformerAllNum <= 0)
	{

		//==============================================//
#ifdef LOG_WRITE_FBX_DATA
		if (mLogStream.is_open())
		{
			mLogStream << "\tNote:	No Animation Data!\n";
		}
#endif
		//=============================================//


		return S_OK;
	}


	//===========================================================================================//
#ifdef LOG_WRITE_FBX_DATA
	if (mLogStream.is_open())
	{
		mLogStream <<
			"\t[Result]		deformerCount : " << deformerAllNum<< "\n" <<
			"\tNote:	If Miss Skeleton Index, No Skinning Data!\n";
	}

	FbxLongLong startTimeLog;
	FbxLongLong endTimeLog;

#endif
	//===========================================================================================//


	mbHasAnim = true;
	std::vector<std::vector<DirectX::XMMATRIX>> keyFrameMatricesPerSkele;
	std::vector<DirectX::XMMATRIX> keyFrame;

	const FbxTime::EMode fbxFreames = FbxTime::eFrames120;
	const int skeleCount = mSkele.size();

	mAnimInvMatrix.reserve(skeleCount);
	mAnimKeyFramePerTime.reserve(skeleCount);
	keyFrame.reserve(skeleCount);
	keyFrameMatricesPerSkele.reserve(skeleCount);

	for (int deformerIndex = 0; deformerIndex < deformerAllNum; ++deformerIndex)
	{
		FbxSkin* pSkin = reinterpret_cast<FbxSkin*>(pMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!pSkin)
		{
			continue;
		}

		int clusterCount = pSkin->GetClusterCount();
		for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
		{
			FbxCluster* cluster = pSkin->GetCluster(clusterIndex);
			FbxNode* clusterNode = cluster->GetLink();
			FbxString name = clusterNode->GetName();
			int skeleIndex = GetSkeleIndex(name);


#ifdef LOG_WRITE_FBX_DATA
			//=============================================================================//
			if (mLogStream.is_open())
			{
				mLogStream <<
					"\t\t[Result]		deformerIndex : " << deformerIndex << " | clusterIndex : " << clusterIndex << " | Node Name : " << name << " | Skeleton Index : " << skeleIndex << "\n";
			}
#endif
			//=============================================================================//


			FbxAMatrix geometryTransformationMatrix = FbxAMatrix(
				pNode->GetGeometricTranslation(FbxNode::eSourcePivot),
				pNode->GetGeometricRotation(FbxNode::eSourcePivot),
				pNode->GetGeometricScaling(FbxNode::eSourcePivot));
			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;

			cluster->GetTransformMatrix(transformMatrix);
			cluster->GetTransformLinkMatrix(transformLinkMatrix); // Bone Space => World Space
			FbxAMatrix globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransformationMatrix;

			DirectX::XMMATRIX globalInverseMatrix = GetXMMATRIX(&globalBindposeInverseMatrix);
			mAnimInvMatrix.push_back(globalInverseMatrix);

			// Relative Vertex = globalBindPoseInverseMatrix * Bindpose Vertex

			UINT indicesNum = cluster->GetControlPointIndicesCount();
			for (UINT indices = 0; indices < indicesNum; ++indices)
			{
				TempAnimInfo tempAnimInfo;
				tempAnimInfo.Index = clusterIndex;
				tempAnimInfo.Weight = cluster->GetControlPointWeights()[indices];
				mContrlPoint[cluster->GetControlPointIndices()[indices]].TempAnimInfo.push_back(tempAnimInfo);
			}


			FbxAnimStack* animStack = mScene->GetSrcObject<FbxAnimStack>(0);
			FbxString animStackName = animStack->GetName();
			FbxTakeInfo* takeInfo = mScene->GetTakeInfo(animStackName);
			FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeInfo->mLocalTimeSpan.GetStop();


			//=================================================//
#ifdef LOG_WRITE_FBX_DATA
			startTimeLog = start.GetFrameCount(fbxFreames);
			endTimeLog = end.GetFrameCount(fbxFreames);
#endif
			//=================================================//


			keyFrame.clear();
			mTotalFrames = end.GetFrameCount(fbxFreames);

			for (FbxLongLong time = start.GetFrameCount(fbxFreames); time <= end.GetFrameCount(fbxFreames); ++time)
			{
				FbxTime currentTime;
				currentTime.SetFrame(time, fbxFreames);

				FbxAMatrix transformOffset = pNode->EvaluateGlobalTransform(currentTime) * geometryTransformationMatrix;
				FbxAMatrix globalTransform = transformOffset.Inverse() * clusterNode->EvaluateGlobalTransform(currentTime);
				DirectX::XMMATRIX globalMatrix = GetXMMATRIX(&globalTransform);

				// Bone's Keyframe Matrics.
				keyFrame.push_back(globalMatrix);
			}

			// Keyframe Matrics Per Bone.
			keyFrameMatricesPerSkele.push_back(keyFrame);

		} // End Loop of clusterIndex


		TempAnimInfo tempAnimInfo;
		tempAnimInfo.Index = 0;
		tempAnimInfo.Weight = 0;

		for (auto iter = mContrlPoint.begin(); iter != mContrlPoint.end(); ++iter)
		{
			for (UINT i = iter->second.TempAnimInfo.size(); i < 4; ++i)
			{
				iter->second.TempAnimInfo.push_back(tempAnimInfo);
			}


			//==================================================================================================================//
#ifdef LOG_WRITE_FBX_DATA
			if (mLogStream.is_open())
			{
				mLogStream <<
					"\t[Result]	Control Point Data\n" <<
					"\t\t[Result]		[Time] : " << startTimeLog << ", " << endTimeLog << "\n" <<
					"\t\t[Result]		[Position] : " << iter->second.Pos.x << ", " << iter->second.Pos.y << ", " << iter->second.Pos.z << "\n" <<
					"\t\t[Result]		[Skeletons] : \n";
				for (auto tempIter = iter->second.TempAnimInfo.begin(); tempIter != iter->second.TempAnimInfo.end(); ++tempIter)
				{
					mLogStream <<
						"\t\t\t[Result]		Index : " << tempIter->Index << ", Weight : " << tempIter->Weight << "\n";
				}
			}
#endif
			//==================================================================================================================//


		}


		//====================================================================================//
#ifdef LOG_WRITE_FBX_DATA
		mLogStream <<
			"\t[Result]		Final Matrix\n";
#endif
		//====================================================================================//


		std::vector<DirectX::XMMATRIX> keyFramePerTime;
		for (int frameNum = 0; frameNum < keyFrameMatricesPerSkele[0].size(); ++frameNum)
		{
			//===================================================//
#ifdef LOG_WRITE_FBX_DATA
			mLogStream <<
				"\t\t[Result]		Rrame Number : " << frameNum << "\n" <<
				"\t\t[Result]		Matrix Content : \n";
#endif
			//===================================================//


			for (int clusterNum = 0; clusterNum < clusterCount; ++clusterNum)
			{
				DirectX::XMMATRIX finalTransform = DirectX::XMMatrixTranspose(mAnimInvMatrix[clusterNum] * keyFrameMatricesPerSkele[clusterNum][frameNum]);

				keyFramePerTime.push_back(finalTransform);


				//==================================================================================//
#ifdef LOG_WRITE_FBX_DATA
				if (mLogStream.is_open())
				{
					DirectX::XMFLOAT4 element;
					for (int j = 0; j < 4; ++j)
					{
						DirectX::XMStoreFloat4(&element, keyFramePerTime[clusterNum].r[j]);
						mLogStream <<
							"\t\t\t[Result]	|" <<
							element.x << ", " <<
							element.y << ", " <<
							element.z << ", " <<
							element.w << "|\n";
					}
					mLogStream << "\n";
				}
#endif
				//==================================================================================//


			}

			mAnimKeyFramePerTime.push_back(keyFramePerTime);

			keyFramePerTime.clear();
		}

	} // End Loop of deformerIndex

	return S_OK;
}

UINT Model::GetSkeleIndex(FbxString skeleName) const
{
	const std::vector<Skeleton>* skeletons = &mSkele;
	int skeleNum = skeletons->size();
	for (int num = 0; num < skeleNum; ++num)
	{
		if ((*skeletons)[num].Name == skeleName)
		{
			return num;
		}
	}

	AssertIsSuccess(false, "Model Get Skele Index - Not Found!");
}

DirectX::XMMATRIX Model::GetXMMATRIX(const FbxAMatrix const* fbxMatrix)
{
	DirectX::XMMATRIX convMatrix;
	for (int i = 0; i < 4; ++i)
	{
		DirectX::XMVECTOR rowVec =
		{
			fbxMatrix->Get(i, 0),
			fbxMatrix->Get(i, 1),
			fbxMatrix->Get(i, 2),
			fbxMatrix->Get(i, 3),
		};
		convMatrix.r[i] = rowVec;
	}
	return convMatrix;
}
