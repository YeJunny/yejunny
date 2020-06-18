#include "Camera.h"
#include "Engine.h"
#include "SoundManager.h"
#include <DirectXMath.h>
#include <stdio.h>
#include <Mmreg.h>

bool SoundManager::Init(Engine* engine)
{
	mEngine = engine;

	bool bIsSuccess = false;

	// 직접 사운드와 기본 사운드 버퍼를 초기화합니다.
	bIsSuccess = InitializeDirectSound();
	AssertIsSuccess(bIsSuccess, "Initialize Direct Sound - Failed");

	return bIsSuccess;
}

void SoundManager::CleanUp()
{
	// 2 차 버퍼를 해제한다.
	for (auto iter = mSounds.begin(); iter != mSounds.end(); ++iter)
	{
		ShutdownWaveFile(&(*iter).second.SecondaryBuffer, &(*iter).second.Secondary3DBuffer);
	}

	mSounds.clear();

	// Direct Sound API를 셧다운한다.
	ShutdownDirectSound();
}


bool SoundManager::InitializeDirectSound()
{
	// 기본 사운드 장치에 대한 직접 사운드 인터페이스 포인터를 초기화합니다.
	HRESULT hr = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	AssertInitialization(hr, "Direct Sound Create 8 - Failed");

	// 협조 레벨을 우선 순위로 설정하여 기본 사운드 버퍼의 형식을 수정할 수 있습니다.
	hr = m_DirectSound->SetCooperativeLevel(mEngine->GetHanleer(), DSSCL_PRIORITY);
	AssertInitialization(hr, "Direct Sound Set Cooperative Level - Failed");


	// 기본 버퍼 설명을 설정합니다.
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// 기본 사운드 장치에서 기본 사운드 버퍼를 제어합니다.
	hr = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	AssertInitialization(hr, "Direct Sound Create Sound Buffer - Failed");


	// 기본 사운드 버퍼의 형식을 설정합니다.
	// 이 경우 16 비트 스테레오 (cd 오디오 형식)에서 44,100 샘플 / 초로 기록되는 .WAV 파일입니다.
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// 기본 버퍼를 지정된 파형 형식으로 설정합니다.
	if (FAILED(m_primaryBuffer->SetFormat(&waveFormat)))
		AssertInitialization(hr, "Direct Sound Create Sound Buffer - Failed");


	// 리스너 인터페이스를 얻습니다.
	if (FAILED(m_primaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&m_listener)))
		AssertInitialization(hr, "Direct Sound Create Sound Buffer - Failed");


	// 리스너의 초기 위치를 장면의 중간에 설정합니다.
	m_listener->SetPosition(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);

	return true;
}

void SoundManager::ShutdownDirectSound()
{
	// 리스너 인터페이스를 해제합니다.
	if (m_listener)
	{
		m_listener->Release();
		m_listener = 0;
	}

	// 기본 사운드 버퍼 포인터를 놓습니다.
	if (m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	// 직접 사운드 인터페이스 포인터를 놓습니다.
	if (m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}
}

bool SoundManager::AddSound(std::string name, const char* soundPath)
{
	Sound sound;


	// 웨이브 파일을 바이너리 모드로 엽니다.
	FILE* filePtr = nullptr;
	int error = fopen_s(&filePtr, soundPath, "rb");
	if (error != 0)
	{
		return false;
	}

	// 웨이브 파일 헤더를 읽는다
	WaveHeaderType waveFileHeader;
	size_t count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// RIFF 포멧 chunk id 체크한다.
	if ((waveFileHeader.ChunkId[0] != 'R') || (waveFileHeader.ChunkId[1] != 'I') ||
		(waveFileHeader.ChunkId[2] != 'F') || (waveFileHeader.ChunkId[3] != 'F'))
	{
		return false;
	}

	// 파일포멧이 wave 인지 체크 한다
	if ((waveFileHeader.Format[0] != 'W') || (waveFileHeader.Format[1] != 'A') ||
		(waveFileHeader.Format[2] != 'V') || (waveFileHeader.Format[3] != 'E'))
	{
		return false;
	}

	// fmt 포멧 chunk id 체크한다.
	if ((waveFileHeader.SubChunkId[0] != 'f') || (waveFileHeader.SubChunkId[1] != 'm') ||
		(waveFileHeader.SubChunkId[2] != 't') || (waveFileHeader.SubChunkId[3] != ' '))
	{
		return false;
	}

	// 오디오 형식이 WAVE_FORMAT_PCM인지 체크한다
	if (waveFileHeader.AudioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	// 웨이브 파일이 모노 형식인지 스테레오인지 체크한다.
	if (waveFileHeader.NumChannels != 1)
	{
		return false;
	}

	// 웨이브 파일이 44.1 KHz의 샘플 속도로 레코딩되었는지 확인합니다.
	if (waveFileHeader.SampleRate != 44100)
	{
		return false;
	}

	// 웨이브 파일이 16 비트 형식으로 레코딩되었는지 확인합니다.
	if (waveFileHeader.BitsPerSample != 16)
	{
		return false;
	}

	// 헤더 데이터의 chunk를 확인한다.
	if ((waveFileHeader.DataChunkId[0] != 'd') || (waveFileHeader.DataChunkId[1] != 'a') ||
		(waveFileHeader.DataChunkId[2] != 't') || (waveFileHeader.DataChunkId[3] != 'a'))
	{
		return false;
	}

	// 이 웨이브 파일이로드 될 보조 버퍼의 웨이브 형식을 설정합니다.
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// 웨이브 파일이로드 될 2 차 사운드 버퍼의 버퍼 설명을 설정합니다.
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = waveFileHeader.DataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// 특정 버퍼 설정으로 임시 사운드 버퍼를 만듭니다.
	IDirectSoundBuffer* tempBuffer = nullptr;
	HRESULT hr = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	AssertInitialization(hr, "Direct Sound Create Sound Buffer - Failed");


	// 다이렉트 사운드 인터페이스에 대해 버퍼 형식을 테스트하고 보조 버퍼를 만듭니다.
	hr = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)(&sound.SecondaryBuffer));
	AssertInitialization(hr, "Direct Sound Query Interface - Failed");


	// 임시 버퍼를 해제한다.
	tempBuffer->Release();
	tempBuffer = 0;

	// 데이터 청크 헤더의 끝에서 시작하는 웨이브 데이터의 시작 부분으로 이동합니다.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// 웨이브 파일 데이터를 저장할 임시 버퍼를 만듭니다.
	unsigned char* waveData = new unsigned char[waveFileHeader.DataSize];
	if (!waveData)
	{
		return false;
	}

	// 웨이브 파일 데이터를 새로 생성 된 버퍼로 읽어들입니다.
	count = fread(waveData, 1, waveFileHeader.DataSize, filePtr);
	if (count != waveFileHeader.DataSize)
	{
		return false;
	}

	// 읽고 나면 파일을 닫는다.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// 웨이브 데이터를 쓰기 위해 2차 버퍼를 잠급니다.
	unsigned char* bufferPtr = nullptr;
	unsigned long bufferSize = 0;
	hr = (sound.SecondaryBuffer)->Lock(0, waveFileHeader.DataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	AssertInitialization(hr, "Direct Sound Lock - Failed");


	// 웨이브 데이터를 버퍼에 복사합니다.
	memcpy(bufferPtr, waveData, waveFileHeader.DataSize);

	// 데이터가 쓰여진 후 보조 버퍼의 잠금을 해제합니다.
	hr = (sound.SecondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	AssertInitialization(hr, "Direct Sound Unlock - Failed");


	// 보조 버퍼에 복사 된 웨이브 데이터를 해제합니다.
	delete[] waveData;
	waveData = 0;

	// 2 차 사운드 버퍼에 3D 인터페이스 가져 오기.
	hr = (sound.SecondaryBuffer)->QueryInterface(IID_IDirectSound3DBuffer8, (void**)(&sound.Secondary3DBuffer));
	AssertInitialization(hr, "Direct Sound Query Interface - Failed");



	mSounds[name] = sound;

	return true;
}

bool SoundManager::DeleteSound(std::string name)
{
	Sound sound = mSounds[name];

	ShutdownWaveFile(&sound.SecondaryBuffer, &sound.Secondary3DBuffer);

	mSounds.erase(name);

	return true;
}

void SoundManager::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer)
{
	// 3D 인터페이스를 2 차 사운드 버퍼에 놓습니다.
	if (*secondary3DBuffer)
	{
		(*secondary3DBuffer)->Release();
		*secondary3DBuffer = 0;
	}

	// 2 차 사운드 버퍼를 해제한다.
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}
}

bool SoundManager::PlayWaveFile(std::string name)
{
	Sound sound = mSounds[name];


	// 사운드 버퍼의 시작 부분에 위치를 설정합니다.
	HRESULT hr = (sound.SecondaryBuffer)->SetCurrentPosition(0);
	AssertInitialization(hr, "Direct Sound Set Current Position - Failed");

	// 버퍼의 볼륨을 100 %로 설정합니다.
	hr = (sound.SecondaryBuffer)->SetVolume(DSBVOLUME_MAX);
	AssertInitialization(hr, "Direct Sound Set Current Position - Failed");


	// 사운드의 3D 위치를 설정합니다.
	const DirectX::XMVECTOR& tempPlayerPos = mEngine->GetCamera()->GetCamPos();
	
	DirectX::XMFLOAT3 playerPos;
	XMStoreFloat3(&playerPos, tempPlayerPos);

	hr = (sound.Secondary3DBuffer)->SetPosition(playerPos.x, playerPos.y, playerPos.z, DS3D_DEFERRED);
	AssertInitialization(hr, "Direct Sound Set Current Position - Failed");


	// 2 차 사운드 버퍼의 내용을 재생합니다.
	hr = (sound.SecondaryBuffer)->Play(0, 0, 0);
	AssertInitialization(hr, "Direct Sound Set Current Position - Failed");


	return true;
}