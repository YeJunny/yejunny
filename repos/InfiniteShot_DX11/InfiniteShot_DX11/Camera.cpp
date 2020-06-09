#include "Camera.h"
#include "ErrorLogger.h"
#include "Engine.h"
#include "Monster.h"
#include "MonsterInfo.h"
#include "Object.h"
#include <WICTextureLoader.h>

void Camera::Init(HINSTANCE hInstance, HWND hwnd, Engine* engine)
{
	HRESULT hr = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		reinterpret_cast<void**>(&mDirectInput),
		nullptr
	);
	AssertInitialization(hr, "Direct Input Create - Failed");

	hr = mDirectInput->CreateDevice(GUID_SysKeyboard, &mDIKeyboard, nullptr);
	AssertInitialization(hr, "Direct Input Create Device - Failed");

	hr = mDirectInput->CreateDevice(GUID_SysMouse, &mDIMouse, nullptr);
	AssertInitialization(hr, "Direct Input Create Device - Failed");

	hr = mDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	AssertInitialization(hr, "Direct Input Set Data Format - Failed");

	hr = mDIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	AssertInitialization(hr, "Direct Input Set Cooperative Level - Failed");

	hr = mDIMouse->SetDataFormat(&c_dfDIMouse);
	AssertInitialization(hr, "Direct Input Set Data Format - Failed");

	hr = mDIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	AssertInitialization(hr, "Direct Input Set Cooperative Level - Failed");


	float playerPos[3];

	std::wfstream playerInfo;
	playerInfo.open("Settings\\PlayerInfo.txt");
	
	playerInfo >> playerPos[0] >> playerPos[1] >> playerPos[2];
	playerInfo >> mWeaponName;

	playerInfo.close();

	mCamPosition = XMVectorSet(playerPos[0], playerPos[1], playerPos[2], 0.0f);
	mCamTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	mCamUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	mCamView = XMMatrixLookAtLH(mCamPosition, mCamTarget, mCamUp);
	mCamProjection = XMMatrixPerspectiveFovLH(0.4f * XM_PI, 800 / 600.0f, 0.1f, 1000.0f);


	mEngine = engine;


	InitWeapon();

	InitMonster();
}

void Camera::UpdateCamera()
{
	mCamRotationMat = XMMatrixRotationRollPitchYaw(mCamPitch, mCamYaw, 0);
	mCamTarget = XMVector3TransformCoord(DEFAULT_FORWARD, mCamRotationMat);
	mCamTarget = XMVector3Normalize(mCamTarget);

	XMMATRIX rotationYTempMat;
	rotationYTempMat = XMMatrixRotationY(mCamYaw);

	mCamRight = XMVector3TransformCoord(DEFAULT_RIGHT, rotationYTempMat);
	mCamUp = XMVector3TransformCoord(mCamUp, rotationYTempMat);
	mCamForward = XMVector3TransformCoord(DEFAULT_FORWARD, rotationYTempMat);

	mCamPosition += mMoveLeftRight * mCamRight;
	mCamPosition += mMoveBackForward * mCamForward;

	mMoveLeftRight = 0.0f;
	mMoveBackForward = 0.0f;

	mCamTarget = mCamPosition + mCamTarget;

	mCamView = XMMatrixLookAtLH(mCamPosition, mCamTarget, mCamUp);


	// Weapon
	XMVECTOR weaponPos = mCamPosition - XMVectorSet(0.0f, mWeaponY, 0.0f, 0.0f) + mWeaponX * mCamRight + mWeaponZ * mCamForward;
	XMMATRIX weaponRot = XMMatrixRotationRollPitchYaw(mCamPitch - XM_PI, mCamYaw, 0);
	XMMATRIX weaponScale = XMMatrixScaling(mWeaponScale, mWeaponScale, mWeaponScale);

	mWorldWeapon = weaponScale * weaponRot * XMMatrixTranslationFromVector(weaponPos);

	mWeapon->SetMatWorld(mWorldWeapon);
}

void Camera::DrawAim() const
{
	ID3D11DeviceContext* d3d11DevCon = mEngine->GetD3d11DevCon();

	UINT strides = sizeof(Vertex);
	UINT offset = 0;

	CBPerObject cbPerObject;
	cbPerObject.World = XMMatrixTranspose(mWorldAim);
	cbPerObject.WVP = XMMatrixTranspose(mWorldAim);

	d3d11DevCon->OMSetBlendState(mEngine->GetTransparencyBlendState(), nullptr, 0xffffffff);

	d3d11DevCon->IASetInputLayout(mEngine->GetBaseInputLayout());
	d3d11DevCon->IASetVertexBuffers(0, 1, mEngine->GetSquareVertBuffer(), &strides, &offset);
	d3d11DevCon->IASetIndexBuffer(*(mEngine->GetSquareIdxBuffer()), DXGI_FORMAT_R32_UINT, 0);

	d3d11DevCon->VSSetShader(mEngine->GetBaseVertShader(), nullptr, 0);
	d3d11DevCon->PSSetShader(mEngine->GetBasePixelShader(), nullptr, 0);

	d3d11DevCon->UpdateSubresource(mCBAim, 0, nullptr, &cbPerObject, 0, 0);
	d3d11DevCon->VSSetConstantBuffers(0, 1, &mCBAim);

	d3d11DevCon->PSSetShaderResources(0, 1, &mTexAim);
	d3d11DevCon->PSSetSamplers(0, 1, mEngine->GetBaseSamplerState());

	d3d11DevCon->RSSetState(mEngine->GetCWcullMode());

	d3d11DevCon->DrawIndexed(6, 0, 0);
}

void Camera::ConvertMouseToRay(int mouseX, int mouseY, XMVECTOR* pos, XMVECTOR* dir)
{
	XMVECTOR pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR pickRayInViewSpacePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	float PRVecX, PRVecY, PRVecZ;

	XMFLOAT4X4 projection;
	XMStoreFloat4x4(&projection, mCamProjection);

	//Transform 2D pick position on screen space to 3D ray in View space
	PRVecX = (((2.0f * mouseX) / mEngine->GetClientWidth()) - 1) / projection._11;
	PRVecY = -(((2.0f * mouseY) / mEngine->GetClientHeight()) - 1) / projection._22;
	PRVecZ = 1.0f;    //View space's Z direction ranges from 0 to 1, so we set 1 since the ray goes "into" the screen

	pickRayInViewSpaceDir = XMVectorSet(PRVecX, PRVecY, PRVecZ, 0.0f);

	//Uncomment this line if you want to use the center of the screen (client area)
	//to be the point that creates the picking ray (eg. first person shooter)
	//pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Transform 3D Ray from View space to 3D ray in World space
	XMMATRIX pickRayToWorldSpaceMatrix;
	XMVECTOR matInvDeter;    //We don't use this, but the xna matrix inverse function requires the first parameter to not be null

	pickRayToWorldSpaceMatrix = XMMatrixInverse(&matInvDeter, mCamView);    //Inverse of View Space matrix is World space matrix

	*pos = XMVector3TransformCoord(pickRayInViewSpacePos, pickRayToWorldSpaceMatrix);
	*dir = XMVector3TransformNormal(pickRayInViewSpaceDir, pickRayToWorldSpaceMatrix);
}

void Camera::InitWeapon()
{
	std::wfstream weaponInfo;
	weaponInfo.open(L"Settings\\Objects\\Weapons\\" + mWeaponName + L".txt");

	weaponInfo >> mWeaponX >> mWeaponY >> mWeaponZ;
	weaponInfo >> mWeaponScale;

	std::wstring shaderFileName;
	std::wstring modelFileNameW;
	std::wstring textureFileName;
	std::vector<std::wstring> textureFileNames;

	weaponInfo >> shaderFileName;
	weaponInfo >> modelFileNameW;

	std::string modelFileName;
	modelFileName.assign(modelFileNameW.begin(), modelFileNameW.end());

	while (true)
	{
		if (!(weaponInfo >> textureFileName))
		{
			break;
		}

		textureFileNames.push_back(textureFileName);
	}

	weaponInfo.close();


	mWeapon = new Object();
	mWeapon->Setup(mEngine->GetD3d11Deivce(), mEngine->GetD3d11DevCon(), mEngine);
	mWeapon->ImportModel(modelFileName.c_str());
	mWeapon->SetTextureFileNamesVector(textureFileNames);
	mWeapon->Initalize(shaderFileName.c_str());
	mWeapon->SetName(mWeaponName);
	textureFileName.clear();
	mWeapon->ImportTextures();
	
	
	std::wfstream aimSetting;
	aimSetting.open("Settings\\Objects\\Weapons\\Aim.txt");
	aimSetting >> textureFileName;
	aimSetting.close();


	HRESULT hr = CreateWICTextureFromFile(mEngine->GetD3d11Deivce(), textureFileName.c_str(), nullptr, &mTexAim, 0);
	AssertInitialization(hr, "Direct 3D Create WIC Texture From File - Failed");

	mWorldAim = XMMatrixScaling(0.1f, 0.1f, 0.1f);


	D3D11_BUFFER_DESC bdDesc;
	ZeroMemory(&bdDesc, sizeof(bdDesc));
	bdDesc.ByteWidth = sizeof(CBPerObject);
	bdDesc.Usage = D3D11_USAGE_DEFAULT;
	bdDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bdDesc.CPUAccessFlags = 0;
	bdDesc.MiscFlags = 0;

	hr = mEngine->GetD3d11Deivce()->CreateBuffer(&bdDesc, nullptr, &mCBAim);
	AssertInitialization(hr, "Direct 3D Create Buffer - Failed");
}

void Camera::InitMonster()
{
	for (auto iter = mEngine->GetObjects().begin(); iter != mEngine->GetObjects().end(); ++iter)
	{
		if ((*iter)->GetName().find(L"Monster") != std::wstring::npos)
		{
			mMonsters.push_back(reinterpret_cast<Monster*>(*iter));
		}
	}
}

void Camera::CleanUp()
{
	mDIKeyboard->Release();
	mDIMouse->Release();
	mDirectInput->Release();
	mWeapon->CleanUp();
	mTexAim->Release();
	mCBAim->Release();
	mMonsters.clear();
	mWeapon->CleanUp();
	delete mWeapon;
}

void Camera::DrawWeapon() const
{
	mWeapon->Draw();

	DrawAim();
}

void Camera::DetectInput(double deltaTime)
{
	static bool bIsShoot = false;

	DIMOUSESTATE mouseCurrState;
	BYTE keyboardState[256];

	mDIKeyboard->Acquire();
	mDIMouse->Acquire();

	mDIKeyboard->GetDeviceState(sizeof(keyboardState), keyboardState);
	mDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	float dist = 15.0f * deltaTime;

	if (keyboardState[DIK_A] & 0x80)
	{
		mMoveLeftRight -= dist;
	}

	if (keyboardState[DIK_D] & 0x80)
	{
		mMoveLeftRight += dist;
	}

	if (keyboardState[DIK_W] & 0x80)
	{
		mMoveBackForward += dist;
	}

	if (keyboardState[DIK_S] & 0x80)
	{
		mMoveBackForward -= dist;
	}

	if (keyboardState[DIK_R] & 0x80)
	{
		std::wfstream weaponPos;
		weaponPos.open(L"Settings\\Objects\\Weapons\\" + mWeaponName + L".txt");

		weaponPos >> mWeaponX >> mWeaponY >> mWeaponZ;
		weaponPos >> mWeaponScale;

		weaponPos.close();
	}

	if (mouseCurrState.rgbButtons[0] & 0x80)
	{
		if (!bIsShoot)
		{
			float tempDist;
			float closestDist = FLT_MAX;
			int hitIndex;

			const int pointX = mEngine->GetClientWidth() / 2;
			const int pointY = mEngine->GetClientHeight() / 2;

			XMVECTOR prwsPos, prwsDir;
			
			ConvertMouseToRay(pointX, pointY, &prwsPos, &prwsDir);

			XMFLOAT3 pos, dir;
			XMStoreFloat3(&pos, prwsPos);
			XMStoreFloat3(&dir, prwsDir);

			for (auto iter = mMonsters.begin(); iter != mMonsters.end(); ++iter)
			{
				for (int index = 0; index < (*iter)->GetNumMonster(); ++index)
				{
					if ((*iter)->GetMonsterHit(index) == 0)
					{
						tempDist = (*iter)->Picked(index, prwsPos, prwsDir);
						if (tempDist < closestDist)
						{
							closestDist = tempDist;
							hitIndex = index;
						}
					}
				}

				if (closestDist < FLT_MAX)
				{
					(*iter)->SetMonsterHit(hitIndex, 1);
					mPickedDist = closestDist;
					++mScore;

					break;
				}
			}

			bIsShoot = true;
		}
	}

	if (!(mouseCurrState.rgbButtons[0] & 0x80))
	{
		bIsShoot = false;
	}

	if (mouseCurrState.lX != mMouseLastState.lX || mouseCurrState.lY != mMouseLastState.lY)
	{
		mCamYaw += (mouseCurrState.lX * 0.001f);
		mCamPitch += (mouseCurrState.lY * 0.001f);
		mMouseLastState = mouseCurrState;
	}

	UpdateCamera();
}