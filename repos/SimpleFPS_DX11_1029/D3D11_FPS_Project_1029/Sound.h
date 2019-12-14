#pragma once

#include "framework.h"
using namespace DirectX;

class Sound
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	Sound();
	Sound(const Sound&);
	~Sound();

	bool Initialize(HWND, const char* soundFile);
	void Shutdown();
	bool PlayWaveFile(const XMFLOAT3&  playerPos);

private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(const char*, IDirectSoundBuffer8**, IDirectSound3DBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**, IDirectSound3DBuffer8**);

private:
	IDirectSound8* m_DirectSound = nullptr;
	IDirectSoundBuffer* m_primaryBuffer = nullptr;
	IDirectSound3DListener8* m_listener = nullptr;
	IDirectSoundBuffer8* m_secondaryBuffer1 = nullptr;
	IDirectSound3DBuffer8* m_secondary3DBuffer1 = nullptr;
};

