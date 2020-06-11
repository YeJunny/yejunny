#include "Camera.h"
#include "Engine.h"
#include "Monster.h"
#include <stdlib.h>
#include <time.h>
#include <WICTextureLoader.h>
using namespace DirectX;

bool Monster::Initalize(const int numMonster, const WCHAR shaderFileName[])
{
	Object::Initalize(shaderFileName);


	mNumMonster = numMonster;
    mWorlds = new DirectX::XMMATRIX[mNumMonster];
    mWorldHP = new DirectX::XMMATRIX[mNumMonster];
    mMonsterHP = new int[mNumMonster];

    std::wstring textureFileName;
    int hp;

    std::wfstream monsterSetting;
    monsterSetting.open("Settings\\Objects\\Monsters\\Monster.txt");
    monsterSetting >> hp >> textureFileName;
    monsterSetting.close();

    HRESULT hr = CreateWICTextureFromFile(mEngine->GetD3d11Deivce(), textureFileName.c_str(), nullptr, &mTexHP, 0);
    AssertInitialization(hr, "Direct 3D Create WIC Texture From File - Failed");
    
    textureFileName.clear();
    for (int i = 0; i < mNumMonster; ++i)
    {
        mMonsterHP[i] = hp;
    }


    std::vector<int> randomsX;
    
    srand(time(null));

    for (int num = 0; num < mNumMonster; ++num)
    {
        while (true)
        {
            int randomX = rand() % 8 - 4;

            for (auto iter = randomsX.begin(); iter != randomsX.end(); ++iter)
            {
                if (randomX == *iter)
                {
                    continue;
                }
            }

            randomsX.push_back(randomX);

            break;
        }
    }

    for (int i = 0; i < mNumMonster; i++)
    {
        mMonsterHit.push_back(0);

        //set the loaded bottles world space
        mWorlds[i] = XMMatrixIdentity();

        int randomX = randomsX.back();
        randomsX.pop_back();
        
        int randomZ = rand() % 8 - 4;

        XMMATRIX Rotation = XMMatrixRotationY(3.14f);
        XMMATRIX Scale = XMMatrixScaling(0.3f, 0.3f, 0.3f);
        XMMATRIX Translation = XMMatrixTranslation(randomX, 2.0f, randomZ * 1.0f);

        mWorlds[i] = Rotation * Scale * Translation;

        Scale = XMMatrixScaling(mMonsterHP[i] / 10.0f * 1.5f, 0.3f, 1.0f);
        mWorldHP[i] = Rotation * Scale * Translation;
    }


	return true;
}

void Monster::Update(double deltaTime)
{
	Object::Update(deltaTime);

    for (int i = 0; i < mNumMonster; ++i)
    {
        if (mMonsterHP[i] > 0)
        {
            XMMATRIX Rotation = XMMatrixRotationY(0);
            XMMATRIX Scale = XMMatrixScaling(0.3f, 0.3f, 0.3f);
            XMMATRIX Translation = XMMatrixTranslation(0.0f, 0.01f, 0.0f);
            //mWorlds[i] = Rotation * Scale * Translation;
            mWorlds[i] *= Translation;

            mWorldHP[i] *= Translation;
        }
    }
}

void Monster::CleanUp()
{
	Object::CleanUp();

    mTexHP->Release();

    delete[] mMonsterHP;
    delete[] mWorldHP;

	mMonsterHit.clear();
	delete[] mWorlds;
}

void Monster::Draw()
{
    ID3D11DeviceContext* d3d11DevCon = mEngine->GetD3d11DevCon();

    Light light;
    CBPerFrame constBuffFrame;
    constBuffFrame.Light = light;
    mD3d11DevCon->UpdateSubresource(mCBPerFrameBuffer, 0, nullptr, &constBuffFrame, 0, 0);
    mD3d11DevCon->PSSetConstantBuffers(0, 1, &mCBPerFrameBuffer);

    UINT offset = 0;

    for (int numMon = 0; numMon < mNumMonster; ++numMon)
    {
        if (mMonsterHP[numMon] > 0)
        {
            UINT stride = sizeof(Model_::Vertex);
            mD3d11DevCon->IASetInputLayout(mVertLayout);

            XMMATRIX WVP = mWorlds[numMon] * mEngine->GetCamera()->GetViewMat() * mEngine->GetCamera()->GetProjMat();
            CBPerObject cbPerObj;
            cbPerObj.World = XMMatrixTranspose(mWorlds[numMon]);
            cbPerObj.WVP = XMMatrixTranspose(WVP);
            mD3d11DevCon->UpdateSubresource(mCBPerObjectBuffer, 0, nullptr, &cbPerObj, 0, 0);
            mD3d11DevCon->VSSetConstantBuffers(0, 1, &mCBPerObjectBuffer);


            // Set animation
            if (mModel.GetHasAnim())
            {
                CBAnimMat cbAnimMat;
                for (int num = 0; num < mModel.GetAnimKeyFRamePerTime()[0].size(); ++num)
                {
                    cbAnimMat.AnimMat[num] = mModel.GetAnimKeyFRamePerTime()[static_cast<int>(mCurrTimeAnim)][num];
                }

                mD3d11DevCon->UpdateSubresource(mAnimMatCB, 0, nullptr, &cbAnimMat, 0, 0);
                mD3d11DevCon->VSSetConstantBuffers(1, 1, &mAnimMatCB);
            }


            // Draw Monster
            mD3d11DevCon->PSSetSamplers(0, 1, &mObjectSamplerState);
            mD3d11DevCon->RSSetState(mEngine->GetNoCullMode());

            mD3d11DevCon->VSSetShader(mVS, nullptr, 0);
            mD3d11DevCon->PSSetShader(mPS, nullptr, 0);

            for (int num = 0; num < mModel.GetVertices().size(); ++num)
            {
                mD3d11DevCon->IASetVertexBuffers(0, 1, &mModelVertBufParts[num], &stride, &offset);
                mD3d11DevCon->PSSetShaderResources(0, 1, &mModelTextureParts[num]);
                mD3d11DevCon->Draw(mModel.GetVertices()[num].size(), 0);
            }


            // Draw box collider
            if (mVertLayout)
            {
                mD3d11DevCon->RSSetState(mEngine->GetWireFrameCWMode());

                mD3d11DevCon->VSSetShader(mEngine->GetBaseVertShader(), nullptr, 0);
                mD3d11DevCon->PSSetShader(mEngine->GetBasePixelShader(), nullptr, 0);

                stride = sizeof(XMFLOAT3);
                mD3d11DevCon->IASetVertexBuffers(0, 1, &mColliVertBuf, &stride, &offset);
                mD3d11DevCon->IASetIndexBuffer(mColliIndexBuf, DXGI_FORMAT_R32_UINT, 0);

                mD3d11DevCon->DrawIndexed(36, 0, 0);
            }


            // Draw hp bar
            WVP = mWorldHP[numMon] * mEngine->GetCamera()->GetViewMat() * mEngine->GetCamera()->GetProjMat();
            cbPerObj.World = XMMatrixTranspose(mWorldHP[numMon]);
            cbPerObj.WVP = XMMatrixTranspose(WVP);
            mD3d11DevCon->UpdateSubresource(mCBPerObjectBuffer, 0, nullptr, &cbPerObj, 0, 0);
            mD3d11DevCon->VSSetConstantBuffers(0, 1, &mCBPerObjectBuffer);

            stride = sizeof(Vertex);
            mD3d11DevCon->IASetVertexBuffers(0, 1, mEngine->GetSquareVertBuffer(), &stride, &offset);
            mD3d11DevCon->IASetIndexBuffer(*mEngine->GetSquareIdxBuffer(), DXGI_FORMAT_R32_UINT, 0);
            mD3d11DevCon->RSSetState(mEngine->GetNoCullMode());
            mD3d11DevCon->PSSetShaderResources(0, 1, &mTexHP);
            mD3d11DevCon->DrawIndexed(6, 0, 0);
        }
    }
}

float Monster::Picked(const int numMon, const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR& dir)
{
    for (int i = 0; i < 36 / 3; ++i)
    {
        //Triangle's vertices V1, V2, V3
        XMVECTOR tri1V1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        XMVECTOR tri1V2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        XMVECTOR tri1V3 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

        //Temporary 3d floats for each vertex
        XMFLOAT3 tV1, tV2, tV3;

        //Get triangle 
        tV1 = mColliVert[(i * 3) + 0];
        tV2 = mColliVert[(i * 3) + 1];
        tV3 = mColliVert[(i * 3) + 2];

        tri1V1 = XMVectorSet(tV1.x, tV1.y, tV1.z, 0.0f);
        tri1V2 = XMVectorSet(tV2.x, tV2.y, tV2.z, 0.0f);
        tri1V3 = XMVectorSet(tV3.x, tV3.y, tV3.z, 0.0f);

        //Transform the vertices to world space
        tri1V1 = XMVector3TransformCoord(tri1V1, mWorlds[numMon]);
        tri1V2 = XMVector3TransformCoord(tri1V2, mWorlds[numMon]);
        tri1V3 = XMVector3TransformCoord(tri1V3, mWorlds[numMon]);

        //Find the normal using U, V coordinates (two edges)
        XMVECTOR U = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        XMVECTOR V = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        XMVECTOR faceNormal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

        U = tri1V2 - tri1V1;
        V = tri1V3 - tri1V1;

        //Compute face normal by crossing U, V
        faceNormal = XMVector3Cross(U, V);

        faceNormal = XMVector3Normalize(faceNormal);

        //Calculate a point on the triangle for the plane equation
        XMVECTOR triPoint = tri1V1;

        //Get plane equation ("Ax + By + Cz + D = 0") Variables
        float tri1A = XMVectorGetX(faceNormal);
        float tri1B = XMVectorGetY(faceNormal);
        float tri1C = XMVectorGetZ(faceNormal);
        float tri1D = (-tri1A * XMVectorGetX(triPoint) - tri1B * XMVectorGetY(triPoint) - tri1C * XMVectorGetZ(triPoint));

        //Now we find where (on the ray) the ray intersects with the triangles plane
        float ep1, ep2, t = 0.0f;
        float planeIntersectX, planeIntersectY, planeIntersectZ = 0.0f;
        XMVECTOR pointInPlane = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

        ep1 = (XMVectorGetX(pos) * tri1A) + (XMVectorGetY(pos) * tri1B) + (XMVectorGetZ(pos) * tri1C);
        ep2 = (XMVectorGetX(dir) * tri1A) + (XMVectorGetY(dir) * tri1B) + (XMVectorGetZ(dir) * tri1C);

        //Make sure there are no divide-by-zeros
        if (ep2 != 0.0f)
            t = -(ep1 + tri1D) / (ep2);

        if (t > 0.0f)    //Make sure you don't pick objects behind the camera
        {
            //Get the point on the plane
            planeIntersectX = XMVectorGetX(pos) + XMVectorGetX(dir) * t;
            planeIntersectY = XMVectorGetY(pos) + XMVectorGetY(dir) * t;
            planeIntersectZ = XMVectorGetZ(pos) + XMVectorGetZ(dir) * t;

            pointInPlane = XMVectorSet(planeIntersectX, planeIntersectY, planeIntersectZ, 0.0f);

            //Call function to check if point is in the triangle
            if (IsContainPointInTriangle(tri1V1, tri1V2, tri1V3, pointInPlane))
            {
                //Return the distance to the hit, so you can check all the other pickable objects in your scene
                //and choose whichever object is closest to the camera
                return t / 2.0f;
            }
        }
    }
    //return the max float value (near infinity) if an object was not picked


    return FLT_MAX;
}

bool Monster::IsContainPointInTriangle(const XMVECTOR& triV1, const XMVECTOR& triV2, const XMVECTOR& triV3, const XMVECTOR& point)
{
    //To find out if the point is inside the triangle, we will check to see if the point
    //is on the correct side of each of the triangles edges.

    XMVECTOR cp1 = XMVector3Cross((triV3 - triV2), (point - triV2));
    XMVECTOR cp2 = XMVector3Cross((triV3 - triV2), (triV1 - triV2));
    if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
    {
        cp1 = XMVector3Cross((triV3 - triV1), (point - triV1));
        cp2 = XMVector3Cross((triV3 - triV1), (triV2 - triV1));
        if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
        {
            cp1 = XMVector3Cross((triV2 - triV1), (point - triV1));
            cp2 = XMVector3Cross((triV2 - triV1), (triV3 - triV1));
            if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
            {
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }
    return false;
}