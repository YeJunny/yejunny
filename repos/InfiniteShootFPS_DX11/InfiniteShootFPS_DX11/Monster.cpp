#include "Camera.h"
#include "Engine.h"
#include "Math.h"
#include "Monster.h"
#include "Player.h"
#include "Utilities.h"
#include <stdlib.h>
#include <time.h>
#include <WICTextureLoader.h>
using namespace DirectX;

bool Monster::Init(const std::string name, const std::string type, const std::string modelPath, const std::vector<std::string> const& texturesPath, const std::string shaderPath, const int numMonster, Engine* engine)
{
	Object::Init(name, type, modelPath, texturesPath, shaderPath, engine);


	mNumMonster = numMonster;
    mWorldMon = new DirectX::XMMATRIX[mNumMonster];
    mWorldHP = new DirectX::XMMATRIX[mNumMonster];
    mWorldColli = new DirectX::XMMATRIX[mNumMonster];
    mMonHP = new int[mNumMonster];
    mPlayerDist = new float[mNumMonster];
    mPos = new DirectX::XMVECTOR[mNumMonster];
    mMonRight = new DirectX::XMVECTOR[mNumMonster];
    mMonForward = new DirectX::XMVECTOR[mNumMonster];


    json monstersJson = Utilities::ParseJsonFile("Settings/Object/Monster/Monsters.json");
    json monsterJson = monstersJson[name];

    std::string hpTexture = monsterJson["HPTexture"];
    
    HRESULT hr = CreateWICTextureFromFile(mEngine->GetD3d11Deivce(), StringConverter::StringToWide(hpTexture).c_str(), nullptr, &mTexHP, 0);
    AssertInitialization(hr, "Direct 3D Create WIC Texture From File - Failed");
    

    mMaxHP = monsterJson["HP"];
    for (int i = 0; i < mNumMonster; ++i)
    {
        mMonHP[i] = mMaxHP;
    }


    std::vector<float> colliderOffset = monsterJson["ColliderOffset"].get<std::vector<float>>();
    std::vector<float> colliderScale = monsterJson["ColliderScale"].get<std::vector<float>>();

    mColliOffset.x = colliderOffset[0];
    mColliOffset.y = colliderOffset[1];
    mColliOffset.z = colliderOffset[2];

    Object::CreateBoxCollider(colliderScale[0], colliderScale[1], colliderScale[2]);


    mAttackTimeAnim = monsterJson["AnimTime"]["Attack"];


    SpawnMonsters();

	return true;
}

void Monster::Update(double deltaTime)
{
	Object::Update(deltaTime);

    static float accTime = 0;
    XMVECTOR camPos = mEngine->GetCamera()->GetCamPos();

    for (int i = 0; i < mNumMonster; ++i)
    {
        if (mMonHP[i] > 0)
        {
            XMVECTOR dir = camPos - mPos[i];
            mPlayerDist[i] = XMVectorGetX(XMVector3Length(dir));

            XMVECTOR rotQuter = Math::LookRotationQuaternion(dir, mEngine->GetCamera()->GetCamUp());

            XMMATRIX scale = XMMatrixScaling(0.3f, 0.3f, 0.3f);
            XMMATRIX rotation = XMMatrixRotationQuaternion(rotQuter);
            XMMATRIX translationMon = XMMatrixTranslationFromVector(mPos[i]);
            mWorldMon[i] = scale * rotation * translationMon;

            scale = XMMatrixScaling(mMonHP[i] * 0.1f, 0.2f, 1.0f);
            XMMATRIX translationHP = translationMon * XMMatrixTranslationFromVector(XMVector3Normalize(dir) + XMVectorSet(0.0f, 0.8f, 0.0f, 0.0f));
            mWorldHP[i] = scale * rotation * translationHP;

            scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
            XMMATRIX translationColli = translationMon * XMMatrixTranslation(mColliOffset.x, mColliOffset.y, mColliOffset.z);
            mWorldColli[i] = scale * rotation * translationColli;
        }
        else
        {
            if (accTime >= 5.0f)
            {
                SpawnMonster(i);

                accTime = 0;
            }
            else
            {
                accTime += deltaTime;
            }
        }
    }

}

void Monster::CleanUp()
{
	Object::CleanUp();

    mTexHP->Release();

    delete[] mWorldMon;
    delete[] mWorldHP;
    delete[] mWorldColli;
    delete[] mMonHP;
    delete[] mPos;
    delete[] mPlayerDist;
    delete[] mMonRight;
    delete[] mMonForward;
}

void Monster::Draw()
{
    ID3D11DeviceContext* d3d11DevCon = mEngine->GetD3d11DevCon();

    Light light;
    CBPerFrame constBuffFrame;
    constBuffFrame.Light = light;
    d3d11DevCon->UpdateSubresource(mCBPerFrameBuffer, 0, nullptr, &constBuffFrame, 0, 0);
    d3d11DevCon->PSSetConstantBuffers(0, 1, &mCBPerFrameBuffer);

    UINT offset = 0;
    CBPerObject cbPerObj;

    for (int numMon = 0; numMon < mNumMonster; ++numMon)
    {
        if (mMonHP[numMon] > 0)
        {
            UINT stride = sizeof(Model_::Vertex);
            d3d11DevCon->IASetInputLayout(mVertLayout);

            XMMATRIX WVP = mWorldMon[numMon] * mEngine->GetCamera()->GetViewMat() * mEngine->GetCamera()->GetProjMat();
            cbPerObj.World = XMMatrixTranspose(mWorldMon[numMon]);
            cbPerObj.WVP = XMMatrixTranspose(WVP);

            d3d11DevCon->UpdateSubresource(mCBPerObjectBuffer, 0, nullptr, &cbPerObj, 0, 0);
            d3d11DevCon->VSSetConstantBuffers(0, 1, &mCBPerObjectBuffer);


            // Set animation
            if (mModel.GetHasAnim())
            {
                CBAnimMat cbAnimMat;
                for (int num = 0; num < mModel.GetAnimKeyFRamePerTime()[0].size(); ++num)
                {
                    cbAnimMat.AnimMat[num] = mModel.GetAnimKeyFRamePerTime()[static_cast<int>(mCurrTimeAnim)][num];
                }

                d3d11DevCon->UpdateSubresource(mAnimMatCB, 0, nullptr, &cbAnimMat, 0, 0);
                d3d11DevCon->VSSetConstantBuffers(1, 1, &mAnimMatCB);
            }


            // Draw monster
            d3d11DevCon->PSSetSamplers(0, 1, &mObjectSamplerState);
            d3d11DevCon->RSSetState(mEngine->GetNoCullMode());

            d3d11DevCon->VSSetShader(mVS, nullptr, 0);
            d3d11DevCon->PSSetShader(mPS, nullptr, 0);

            for (int num = 0; num < mModel.GetVertices().size(); ++num)
            {
                d3d11DevCon->IASetVertexBuffers(0, 1, &mModelVertBufParts[num], &stride, &offset);
                d3d11DevCon->PSSetShaderResources(0, 1, &mModelTextureParts[num]);
                d3d11DevCon->Draw(mModel.GetVertices()[num].size(), 0);
            }


            // Draw box collider
            if (mVertLayout)
            {
                WVP = mWorldColli[numMon] * mEngine->GetCamera()->GetViewMat() * mEngine->GetCamera()->GetProjMat();
                cbPerObj.World = XMMatrixTranspose(mWorldColli[numMon]);
                cbPerObj.WVP = XMMatrixTranspose(WVP);

                d3d11DevCon->UpdateSubresource(mCBPerObjectBuffer, 0, nullptr, &cbPerObj, 0, 0);
                d3d11DevCon->VSSetConstantBuffers(0, 1, &mCBPerObjectBuffer);

                d3d11DevCon->RSSetState(mEngine->GetWireFrameCWMode());

                d3d11DevCon->VSSetShader(mEngine->GetBaseVertShader(), nullptr, 0);
                d3d11DevCon->PSSetShader(mEngine->GetBasePixelShader(), nullptr, 0);

                stride = sizeof(XMFLOAT3);
                d3d11DevCon->IASetVertexBuffers(0, 1, &mColliVertBuf, &stride, &offset);
                d3d11DevCon->IASetIndexBuffer(mColliIndexBuf, DXGI_FORMAT_R32_UINT, 0);

                d3d11DevCon->DrawIndexed(36, 0, 0);
            }


            // Draw hp bar
            WVP = mWorldHP[numMon] * mEngine->GetCamera()->GetViewMat() * mEngine->GetCamera()->GetProjMat();
            cbPerObj.World = XMMatrixTranspose(mWorldHP[numMon]);
            cbPerObj.WVP = XMMatrixTranspose(WVP);

            d3d11DevCon->UpdateSubresource(mCBPerObjectBuffer, 0, nullptr, &cbPerObj, 0, 0);
            d3d11DevCon->VSSetConstantBuffers(0, 1, &mCBPerObjectBuffer);

            stride = sizeof(Vertex);
            d3d11DevCon->IASetVertexBuffers(0, 1, mEngine->GetSquareVertBuffer(), &stride, &offset);
            d3d11DevCon->IASetIndexBuffer(*mEngine->GetSquareIdxBuffer(), DXGI_FORMAT_R32_UINT, 0);
            d3d11DevCon->RSSetState(mEngine->GetNoCullMode());
            d3d11DevCon->PSSetShaderResources(0, 1, &mTexHP);
            d3d11DevCon->DrawIndexed(6, 0, 0);
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
        tri1V1 = XMVector3TransformCoord(tri1V1, mWorldColli[numMon]);
        tri1V2 = XMVector3TransformCoord(tri1V2, mWorldColli[numMon]);
        tri1V3 = XMVector3TransformCoord(tri1V3, mWorldColli[numMon]);

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

void Monster::SpawnMonsters()
{
    const int rangeX = 18;
    const int rangeZ = 11;
    const int rangeOrigZ = 5;

    srand(time(null));

    for (int i = 0; i < mNumMonster; i++)
    {   
        float randomX = static_cast<float>(rand() % (rangeX * 2) - rangeX);
        float randomZ = static_cast<float>(rand() % (rangeZ * 2) - rangeZ + rangeOrigZ);
        mPos[i] = XMVectorSet(randomX, 2.0f, randomZ * 1.0f, 0.0f);
    }
}

void Monster::SpawnMonster(int index)
{
    const int rangeX = 18;
    const int rangeZ = 11;
    const int rangeOrigZ = 5;

    srand(time(null));

    float randomX = static_cast<float>(rand() % (rangeX * 2) - rangeX);
    float randomZ = static_cast<float>(rand() % (rangeZ * 2) - rangeZ + rangeOrigZ);
    mPos[index] = XMVectorSet(randomX, 2.0f, randomZ * 1.0f, 0.0f);
    mMonHP[index] = mMaxHP;
}
