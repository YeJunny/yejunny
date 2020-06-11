#include "Camera.h"
#include "Monster.h"
#include "Player.h"
#include <WICTextureLoader.h>

void Player::Shoot()
{
	if (mAccTime >= BOUND_TIME)
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
				if ((*iter)->GetMonsterHP(index) > 0)
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
				(*iter)->SubHP(hitIndex, 1);
				mPickedDist = closestDist;
				++mScore;

				break;
			}
		}

		mbIsShoot = true;
		mAccTime = 0;
	}
}

void Player::ChangeWeapon()
{
	std::wfstream weaponPos;
	weaponPos.open(L"Settings\\Objects\\Weapons\\" + mWeaponName + L".txt");

	weaponPos >> mWeaponX >> mWeaponY >> mWeaponZ;
	weaponPos >> mWeaponScale;

	weaponPos.close();
}

void Player::Init(Engine* engine)
{
	mEngine = engine;


	std::wfstream playerInfo;
	playerInfo.open("Settings\\PlayerInfo.txt");

	playerInfo >> mPos.x >> mPos.y >> mPos.z;
	playerInfo >> mWeaponName;

	playerInfo.close();


	InitWeapon();

	InitMonsterVector();
}

void Player::Update(double deltaTime)
{
	XMVECTOR camRight = mEngine->GetCamera()->GetCamRight();
	XMVECTOR camForward = mEngine->GetCamera()->GetCamForward();
	XMVECTOR camPos = mEngine->GetCamera()->GetCamPos();

	float camPitch = mEngine->GetCamera()->GetCamPitch();
	float camYaw = mEngine->GetCamera()->GetCamYaw();

	XMStoreFloat3(&mPos, mEngine->GetCamera()->GetCamPos());


	// Weapon
	XMVECTOR upVec = XMVectorSet(0.0f, -mWeaponY, 0.0f, 0.0f);
	XMVECTOR rightVec = mWeaponX * camRight;
	XMVECTOR forwardVec = mWeaponZ * camForward;

	XMVECTOR weaponPos = camPos + upVec + rightVec + forwardVec;
	XMMATRIX weaponRot = XMMatrixRotationRollPitchYaw(camPitch - XM_PI, camYaw, 0);
	XMMATRIX weaponScale = XMMatrixScaling(mWeaponScale, mWeaponScale, mWeaponScale);

	mWorldWeapon = weaponScale * weaponRot * XMMatrixTranslationFromVector(weaponPos);

	mWeapon->SetMatWorld(mWorldWeapon);


	// Muzzle Flash
	upVec = XMVectorSet(0.0f, -mMuzzleFlashY, 0.0f, 0.0f);
	rightVec = mMuzzleFlashX * camRight;
	forwardVec = mMuzzleFlashZ * camForward;

	XMVECTOR muzzleFlashPos = camPos + upVec + rightVec + forwardVec;
	XMMATRIX muzzleFlashRot = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	XMMATRIX muzzleFlashScale = XMMatrixScaling(mMuzzleFlashScale, mMuzzleFlashScale, mMuzzleFlashScale);

	mWorldMuzzleFlash = muzzleFlashScale * XMMatrixTranslationFromVector(muzzleFlashPos);


	mAccTime += deltaTime;
}

void Player::Draw()
{
	mWeapon->Draw();

	DrawAim();

	DrawMuzzleFlash();
}

void Player::CleanUp()
{
	mWeapon->CleanUp();
	mTexAim->Release();
	mCBAim->Release();
	mMonsters.clear();
	mTexMuzzleFlash->Release();
	mCBMuzzleFlash->Release();
	mWeapon->CleanUp();
	delete mWeapon;
}

void Player::ConvertMouseToRay(int mouseX, int mouseY, XMVECTOR* pos, XMVECTOR* dir)
{
	XMVECTOR pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR pickRayInViewSpacePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	float PRVecX, PRVecY, PRVecZ;

	XMFLOAT4X4 projection;
	XMStoreFloat4x4(&projection, mEngine->GetCamera()->GetProjMat());

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

	pickRayToWorldSpaceMatrix = XMMatrixInverse(&matInvDeter, mEngine->GetCamera()->GetViewMat());    //Inverse of View Space matrix is World space matrix

	*pos = XMVector3TransformCoord(pickRayInViewSpacePos, pickRayToWorldSpaceMatrix);
	*dir = XMVector3TransformNormal(pickRayInViewSpaceDir, pickRayToWorldSpaceMatrix);
}

void Player::InitMonsterVector()
{
	for (auto iter = mEngine->GetObjects().begin(); iter != mEngine->GetObjects().end(); ++iter)
	{
		if ((*iter)->GetName().find(L"Monster") != std::wstring::npos)
		{
			mMonsters.push_back(reinterpret_cast<Monster*>(*iter));
		}
	}
}

void Player::InitWeapon()
{
	std::wfstream weaponInfo;
	weaponInfo.open(L"Settings\\Objects\\Weapons\\" + mWeaponName + L".txt");

	weaponInfo >> mWeaponX >> mWeaponY >> mWeaponZ;
	weaponInfo >> mWeaponScale;

	weaponInfo >> mMuzzleFlashX >> mMuzzleFlashY >> mMuzzleFlashZ;
	weaponInfo >> mMuzzleFlashScale;

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
	textureFileName.clear();

	mWorldAim = XMMatrixScaling(0.1f, 0.1f, 0.1f);


	std::wfstream muzzleFlashSetting;
	muzzleFlashSetting.open("Settings\\Objects\\Weapons\\MuzzleFlash.txt");
	muzzleFlashSetting >> textureFileName;
	muzzleFlashSetting.close();


	hr = CreateWICTextureFromFile(mEngine->GetD3d11Deivce(), textureFileName.c_str(), nullptr, &mTexMuzzleFlash, 0);
	AssertInitialization(hr, "Direct 3D Create WIC Texture From File - Failed");
	textureFileName.clear();


	D3D11_BUFFER_DESC bdDesc;
	ZeroMemory(&bdDesc, sizeof(bdDesc));
	bdDesc.ByteWidth = sizeof(CBPerObject);
	bdDesc.Usage = D3D11_USAGE_DEFAULT;
	bdDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bdDesc.CPUAccessFlags = 0;
	bdDesc.MiscFlags = 0;

	hr = mEngine->GetD3d11Deivce()->CreateBuffer(&bdDesc, nullptr, &mCBAim);
	AssertInitialization(hr, "Direct 3D Create Buffer - Failed");

	hr = mEngine->GetD3d11Deivce()->CreateBuffer(&bdDesc, nullptr, &mCBMuzzleFlash);
	AssertInitialization(hr, "Direct 3D Create Buffer - Failed");
}

void Player::DrawAim() const
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

void Player::DrawMuzzleFlash()
{
	if (mAccTime <= BOUND_TIME && mbIsShoot)
	{
		ID3D11DeviceContext* d3d11DevCon = mEngine->GetD3d11DevCon();

		UINT strides = sizeof(Vertex);
		UINT offset = 0;

		CBPerObject cbPerObject;
		cbPerObject.World = XMMatrixTranspose(mWorldMuzzleFlash);
		cbPerObject.WVP = XMMatrixTranspose(mWorldMuzzleFlash);

		d3d11DevCon->OMSetBlendState(mEngine->GetTransparencyBlendState(), nullptr, 0xffffffff);

		d3d11DevCon->IASetInputLayout(mEngine->GetBaseInputLayout());
		d3d11DevCon->IASetVertexBuffers(0, 1, mEngine->GetSquareVertBuffer(), &strides, &offset);
		d3d11DevCon->IASetIndexBuffer(*(mEngine->GetSquareIdxBuffer()), DXGI_FORMAT_R32_UINT, 0);

		d3d11DevCon->VSSetShader(mEngine->GetBaseVertShader(), nullptr, 0);
		d3d11DevCon->PSSetShader(mEngine->GetBasePixelShader(), nullptr, 0);

		d3d11DevCon->UpdateSubresource(mCBMuzzleFlash, 0, nullptr, &cbPerObject, 0, 0);
		d3d11DevCon->VSSetConstantBuffers(0, 1, &mCBMuzzleFlash);

		d3d11DevCon->PSSetShaderResources(0, 1, &mTexMuzzleFlash);
		d3d11DevCon->PSSetSamplers(0, 1, mEngine->GetBaseSamplerState());

		d3d11DevCon->RSSetState(mEngine->GetNoCullMode());

		d3d11DevCon->DrawIndexed(6, 0, 0);
	}
	else
	{
		mbIsShoot = false;
	}
}