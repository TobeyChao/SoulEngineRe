#include "DSoundAudio.h"
#include "../../Platform/Win/WindowsUtils.h"
#include "../../SoulMain/Audio/SoundResourceExtraData.h"
#include "../../SoulMain/Resource/Resource.h"
#include "DSoundAudioBuffer.h"

namespace Soul
{
	DSoundAudio::DSoundAudio()
		:
		mDirectSound(nullptr)
	{}

	bool DSoundAudio::Active()
	{
		return mDirectSound != nullptr;
	}

	IAudioBuffer* DSoundAudio::InitAudioBuffer(const std::wstring& name, std::shared_ptr<Resource> handle)
	{
		std::shared_ptr<SoundResourceExtraData> extra =
			std::static_pointer_cast<SoundResourceExtraData>(handle->GetExtra());

		if (!mDirectSound)
		{
			return nullptr;
		}

		switch (extra->GetSoundType())
		{
		case SOUND_TYPE_MP3:
			break;
		case SOUND_TYPE_WAVE:
			break;
		case SOUND_TYPE_MIDI:
			return nullptr;
		case SOUND_TYPE_OGG:
			return nullptr;
		default:
			return nullptr;
		}

		LPDIRECTSOUNDBUFFER8 sampleHandle = nullptr;

		DSBUFFERDESC dsbd;
		ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
		dsbd.dwSize = sizeof(DSBUFFERDESC);
		dsbd.dwFlags = DSBCAPS_CTRLVOLUME;
		dsbd.dwBufferBytes = handle->Size();
		dsbd.guid3DAlgorithm = GUID_NULL;
		dsbd.lpwfxFormat = const_cast<WAVEFORMATEX*>(extra->GetFormat());

		IDirectSoundBuffer* tempBuffer = nullptr;
		HRESULT result = mDirectSound->CreateSoundBuffer(&dsbd, &tempBuffer, NULL);
		if (FAILED(result))
		{
			return nullptr;
		}

		// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
		result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&sampleHandle);
		if (FAILED(result))
		{
			return nullptr;
		}

		// Release the temporary buffer.
		tempBuffer->Release();
		tempBuffer = nullptr;

		// Add handle to the list
		IAudioBuffer* audioBuffer = new DSoundAudioBuffer(sampleHandle, handle);
		mAllSamples[name] = audioBuffer;

		return audioBuffer;
	}
	void DSoundAudio::Shutdown()
	{
		if (mInitialized)
		{
			Audio::Shutdown();
			if (mDirectSound)
			{
				mDirectSound->Release();
				mDirectSound = nullptr;
			}
			mInitialized = false;
		}
	}

	bool DSoundAudio::Initialize()
	{
		if (mInitialized)
			return true;

		mAllSamples.clear();

		if (mDirectSound)
		{
			mDirectSound->Release();
			mDirectSound = nullptr;
		}

		HRESULT hr;

		// Create IDirectSound using the primary sound device
		if (FAILED(hr = DirectSoundCreate8(NULL, &mDirectSound, NULL)))
			return false;

		if (Platform::WindowsUtils::HWnd == nullptr)
		{
			return false;
		}

		// Set DirectSound coop level 
		if (FAILED(hr = mDirectSound->SetCooperativeLevel(Platform::WindowsUtils::HWnd, DSSCL_PRIORITY)))
			return false;

		if (FAILED(hr = SetPrimaryBufferFormat(2u, 44100u, 16u)))
			return false;

		mInitialized = true;

		return true;
	}

	const std::string& DSoundAudio::GetAudioSystemName() const
	{
		static std::string name = "dsound";
		return name;
	}

	HRESULT DSoundAudio::SetPrimaryBufferFormat(DWORD dwPrimaryChannels, DWORD dwPrimaryFreq, DWORD dwPrimaryBitRate)
	{
		HRESULT hr;
		LPDIRECTSOUNDBUFFER pDSBPrimary = nullptr;

		if (!mDirectSound)
			return CO_E_NOTINITIALIZED;

		// Get the primary buffer 
		DSBUFFERDESC dsbd;
		ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
		dsbd.dwSize = sizeof(DSBUFFERDESC);
		dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;;
		dsbd.dwBufferBytes = 0;
		dsbd.lpwfxFormat = nullptr;

		if (FAILED(hr = mDirectSound->CreateSoundBuffer(&dsbd, &pDSBPrimary, nullptr)))
			return hr;

		WAVEFORMATEX wfx;
		ZeroMemory(&wfx, sizeof(WAVEFORMATEX));
		wfx.wFormatTag = (WORD)WAVE_FORMAT_PCM;
		wfx.nChannels = (WORD)dwPrimaryChannels;
		wfx.nSamplesPerSec = (DWORD)dwPrimaryFreq;
		wfx.wBitsPerSample = (WORD)dwPrimaryBitRate;
		wfx.nBlockAlign = (WORD)(wfx.wBitsPerSample / 8 * wfx.nChannels);
		wfx.nAvgBytesPerSec = (DWORD)(wfx.nSamplesPerSec * wfx.nBlockAlign);
		wfx.cbSize = 0;

		if (FAILED(hr = pDSBPrimary->SetFormat(&wfx)))
			return hr;

		if (pDSBPrimary)
		{
			pDSBPrimary->Release();
			pDSBPrimary = nullptr;
		}

		return S_OK;
	}
}