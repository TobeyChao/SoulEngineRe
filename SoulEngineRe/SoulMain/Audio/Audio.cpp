#include "Audio.h"
#include "IAudioBuffer.h"

namespace Soul
{
	Audio::Audio()
		:
		mInitialized(false),
		mAllPaused(false)
	{
	}

	Audio::~Audio()
	{
	}

	void Audio::StopAllSounds()
	{
		for (auto it = mAllSamples.begin(); it != mAllSamples.end(); it++)
		{
			IAudioBuffer *audioBuffer = (*it).second;
			audioBuffer->Stop();
		}
		mAllPaused = true;
	}

	void Audio::PauseAllSounds()
	{
		for (auto it = mAllSamples.begin(); it != mAllSamples.end(); it++)
		{
			IAudioBuffer *audioBuffer = (*it).second;
			audioBuffer->Pause();
		}
		mAllPaused = true;
	}

	void Audio::ResumeAllSounds()
	{
		for (auto it = mAllSamples.begin(); it != mAllSamples.end(); it++)
		{
			IAudioBuffer *audioBuffer = (*it).second;
			audioBuffer->Resume();
		}
		mAllPaused = false;
	}

	void Audio::Shutdown()
	{
		auto i = mAllSamples.begin();

		while (i != mAllSamples.end())
		{
			IAudioBuffer * audioBuffer = (*i).second;
			audioBuffer->Stop();
			i = mAllSamples.erase(i);
		}
	}

	bool Audio::IsPaused()
	{
		return mAllPaused;
	}
	IAudioBuffer * Audio::GetSound(const std::wstring& name)
	{
		return mAllSamples[name];
	}
}