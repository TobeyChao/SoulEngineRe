#pragma once
#include "IAudio.h"
namespace Soul
{
	class Audio : public IAudio
	{
	public:
		Audio();
		~Audio();
		virtual void StopAllSounds() override;
		virtual void PauseAllSounds() override;
		virtual void ResumeAllSounds() override;

		virtual void Shutdown() override;
		IAudioBuffer * GetSound(const std::wstring& name);
		bool IsPaused();
	protected:
		std::map<std::wstring, IAudioBuffer *> mAllSamples;
		bool mAllPaused;
		bool mInitialized;
	};
}