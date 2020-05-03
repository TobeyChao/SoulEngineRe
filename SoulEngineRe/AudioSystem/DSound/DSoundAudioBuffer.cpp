#include "DSoundAudioBuffer.h"
#include "../../SoulMain/Resource/Resource.h"

namespace Soul
{
	DSoundAudioBuffer::DSoundAudioBuffer(LPDIRECTSOUNDBUFFER8 sample, std::shared_ptr<Resource> resource)
		:
		AudioBuffer(resource),
		mSample(sample)
	{
		mPlayPos = 0u;
		FillBufferWithSound();
	}

	void * DSoundAudioBuffer::Get()
	{
		return mSample;
	}

	bool DSoundAudioBuffer::Play(int volume, bool looping)
	{
		mVolume = volume;
		mIsLooping = looping;

		mIsPaused = false;

		LPDIRECTSOUNDBUFFER pDSB = (LPDIRECTSOUNDBUFFER)Get();
		if (!pDSB)
		{
			return false;
		}

		SetVolume(volume);

		DWORD dwFlags = looping ? DSBPLAY_LOOPING : 0L;
		return (mSample->Play(0, 0, dwFlags) == S_OK);
	}

	bool DSoundAudioBuffer::Pause()
	{
		LPDIRECTSOUNDBUFFER pDSB = (LPDIRECTSOUNDBUFFER)Get();

		if (!pDSB)
			return false;
		mIsPaused = true;
		pDSB->GetCurrentPosition(&mPlayPos, nullptr);
		pDSB->Stop();
		pDSB->SetCurrentPosition(0);	// rewinds buffer to beginning.
		return true;
	}

	bool DSoundAudioBuffer::Stop()
	{
		LPDIRECTSOUNDBUFFER pDSB = (LPDIRECTSOUNDBUFFER)Get();
		if (!pDSB)
			return false;
		mIsPaused = true;
		pDSB->Stop();
		return true;
	}

	bool DSoundAudioBuffer::Resume()
	{
		mIsPaused = false;
		mSample->SetCurrentPosition(mPlayPos);
		return Play(GetVolume(), IsLooping());
	}

	bool DSoundAudioBuffer::TogglePause()
	{
		if (mIsPaused)
		{
			Resume();
		}
		else
		{
			Pause();
		}
		return true;
	}

	bool DSoundAudioBuffer::IsPlaying()
	{
		DWORD dwStatus = 0;
		LPDIRECTSOUNDBUFFER pDSB = (LPDIRECTSOUNDBUFFER)Get();
		pDSB->GetStatus(&dwStatus);
		bool bIsPlaying = ((dwStatus & DSBSTATUS_PLAYING) != 0);
		return bIsPlaying;
	}

	void DSoundAudioBuffer::SetVolume(int volume)
	{
		LONG vol = 0;
		float coeff = (float)volume / 100.0f;
		//logarithmic scale
		float dbValue = 20.0f * log10f(coeff);
		vol = (LONG)dbValue * 100;
		if (volume == 0)
		{
			vol = DSBVOLUME_MIN;
		}
		else if (volume == 100)
		{
			vol = DSBVOLUME_MAX;
		}
		LPDIRECTSOUNDBUFFER pDSB = (LPDIRECTSOUNDBUFFER)Get();
		pDSB->SetVolume(vol);
	}

	void DSoundAudioBuffer::SetPosition(unsigned long newPosition)
	{
		mSample->SetCurrentPosition(newPosition);
	}

	float DSoundAudioBuffer::GetProgress()
	{
		LPDIRECTSOUNDBUFFER pDSB = (LPDIRECTSOUNDBUFFER)Get();
		DWORD progress = 0;
		pDSB->GetCurrentPosition(&progress, NULL);
		float length = (float)mResource->Size();
		return (float)progress / length;
	}

	HRESULT DSoundAudioBuffer::FillBufferWithSound()
	{
		int pcmBufferSize = mResource->Size();
		unsigned char* bufferPtr;
		unsigned long bufferSize;
		// Lock the secondary buffer to write wave data into it.
		HRESULT result = mSample->Lock(0, pcmBufferSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
		if (FAILED(result))
		{
			return false;
		}
		// Copy the wave data into the buffer.
		memcpy(bufferPtr, mResource->Buffer(), pcmBufferSize);
		// Free redundancy buffer.
		mResource->ReleaseBuffer();
		// Unlock the secondary buffer after the data has been written to it.
		result = mSample->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
		if (FAILED(result))
		{
			return false;
		}
		return S_OK;
	}
}