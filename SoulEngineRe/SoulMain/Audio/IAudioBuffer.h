#pragma once
#include "../SoulPCH.h"
namespace Soul
{
	class IAudioBuffer
	{
	public:
		virtual void *Get() = 0;
		virtual std::shared_ptr<Resource> GetResource() = 0;

		virtual bool Play(int volume, bool looping) = 0;
		virtual bool Pause() = 0;
		virtual bool Stop() = 0;
		virtual bool Resume() = 0;

		virtual bool TogglePause() = 0;
		virtual bool IsPlaying() = 0;
		virtual bool IsLooping() const = 0;
		virtual void SetVolume(int volume) = 0;
		virtual void SetPosition(unsigned long newPosition) = 0;
		virtual int GetVolume() const = 0;
		virtual float GetProgress() = 0;
	};
}