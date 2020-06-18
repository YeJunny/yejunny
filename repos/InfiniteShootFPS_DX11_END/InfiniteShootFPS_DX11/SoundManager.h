#pragma once

#pragma comment(lib, "dsound.lib")

#include <dsound.h>
#include <unordered_map>

class SoundManager
{
private:
	struct WaveHeaderType
	{
		char ChunkId[4];
		unsigned long ChunkSize;
		char Format[4];
		char SubChunkId[4];
		unsigned long SubChunkSize;
		unsigned short AudioFormat;
		unsigned short NumChannels;
		unsigned long SampleRate;
		unsigned long BytesPerSecond;
		unsigned short BlockAlign;
		unsigned short BitsPerSample;
		char DataChunkId[4];
		unsigned long DataSize;
	};

	struct Sound
	{
		IDirectSoundBuffer8* SecondaryBuffer = nullptr;
		IDirectSound3DBuffer8* Secondary3DBuffer = nullptr;
	};

public:
	SoundManager() = default;
	SoundManager(const SoundManager&) = delete;

	bool Init(Engine* engine);
	bool AddSound(std::string name, const char* soundPath);
	bool DeleteSound(std::string name);
	void CleanUp();
	bool PlayWaveFile(std::string name);

private:
	bool InitializeDirectSound();
	void ShutdownDirectSound();
	void ShutdownWaveFile(IDirectSoundBuffer8**, IDirectSound3DBuffer8**);

private:
	IDirectSound8* m_DirectSound = nullptr;
	IDirectSoundBuffer* m_primaryBuffer = nullptr;
	IDirectSound3DListener8* m_listener = nullptr;

	std::unordered_map<std::string, Sound> mSounds;

	// Core
	Engine* mEngine;
};
