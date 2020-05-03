#include "XAudio2AudioBuffer.h"
#include "../../SoulMain/Resource/Resource.h"

namespace Soul
{
	XAudio2AudioBuffer::XAudio2AudioBuffer(IXAudio2SourceVoice * voice, std::shared_ptr<Resource> resource)
		:
		AudioBuffer(resource),
		mXAudioBuffer(std::make_unique<XAUDIO2_BUFFER>()),
		mVoice(voice)
	{
		mPlayPos = 0u;
	}

	void * XAudio2AudioBuffer::Get()
	{
		return mVoice;
	}

	bool XAudio2AudioBuffer::Play(int volume, bool looping)
	{
		mXAudioBuffer->pAudioData = mResource->Buffer();
		// tell the source voice not to expect any data after this buffer
		mXAudioBuffer->Flags = XAUDIO2_END_OF_STREAM;
		mXAudioBuffer->AudioBytes = mResource->Size();
		if (looping)
		{
			mXAudioBuffer->LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		else
		{
			mXAudioBuffer->LoopCount = XAUDIO2_NO_LOOP_REGION;
		}
		if (FAILED(mVoice->SubmitSourceBuffer(mXAudioBuffer.get())))
		{
			return false;
		}
		if (FAILED(mVoice->SetVolume((float)volume / 100.0f)))
		{
			return false;
		}
		
		mVoice->Start(0);

		mIsPaused = false;
		return true;
	}

	bool XAudio2AudioBuffer::Pause()
	{
		IXAudio2SourceVoice* pSourceVoice = (IXAudio2SourceVoice*)Get();

		if (pSourceVoice == nullptr)
			return false;

		if (!mIsPaused)
		{
			pSourceVoice->Stop(0, XAUDIO2_COMMIT_NOW);
			mIsPaused = true;
		}
		return true;
	}

	bool XAudio2AudioBuffer::Stop()
	{
		IXAudio2SourceVoice* pSourceVoice = (IXAudio2SourceVoice*)Get();
		if (pSourceVoice == nullptr)
			return false;
		if (!mIsPaused)
		{
			mIsPaused = true;
			if (FAILED(pSourceVoice->Stop(0, XAUDIO2_COMMIT_NOW)))
				return false;
			if (FAILED(pSourceVoice->FlushSourceBuffers()))
				return false;
		}
		return true;
	}

	bool XAudio2AudioBuffer::Resume()
	{
		return false;
	}

	bool XAudio2AudioBuffer::TogglePause()
	{
		return false;
	}

	bool XAudio2AudioBuffer::IsPlaying()
	{
		return false;
	}

	void XAudio2AudioBuffer::SetVolume(int volume)
	{
	}

	void XAudio2AudioBuffer::SetPosition(unsigned long newPosition)
	{
	}

	float XAudio2AudioBuffer::GetProgress()
	{
		return 0.0f;
	}
}