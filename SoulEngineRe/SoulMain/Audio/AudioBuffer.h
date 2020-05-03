#pragma once
#include "IAudioBuffer.h"

namespace Soul
{
	class AudioBuffer : public IAudioBuffer
	{
	public:
		virtual std::shared_ptr<Resource> GetResource() override { return mResource; }
		virtual bool IsLooping() const override { return mIsLooping; }
		virtual int GetVolume() const override { return mVolume; }
	protected:
		AudioBuffer(std::shared_ptr<Resource >resource);
	protected:
		std::shared_ptr<Resource> mResource;
		bool mIsPaused;			// Is the sound paused
		bool mIsLooping;			// Is the sound looping
		int mVolume;				//the volume
		unsigned long mPlayPos;	//position
	};
}