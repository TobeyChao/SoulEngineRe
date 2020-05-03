#include "AudioBuffer.h"

namespace Soul
{
	AudioBuffer::AudioBuffer(std::shared_ptr<Resource> resource)
	{
		mResource = resource;
		mIsPaused = false;
		mIsLooping = false;
		mVolume = 0;
	}
}