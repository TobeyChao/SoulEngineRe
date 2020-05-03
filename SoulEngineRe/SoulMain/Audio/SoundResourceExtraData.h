#pragma once
#include <Windows.h>
#include "IAudio.h"
#include "../Resource/IResourceExtraData.h"
namespace Soul
{
	class SoundResourceExtraData : public IResourceExtraData
	{
		friend class WaveResourceLoader;
		friend class OggResourceLoader;
	public:
		SoundResourceExtraData();
		virtual ~SoundResourceExtraData() { }
		virtual std::wstring ToString() { return L"SoundResourceExtraData"; }
		enum SoundType GetSoundType() { return mSoundType; }
		WAVEFORMATEX const * GetFormat() { return &mWavFormatEx; } //const data non-const pointer
		int GetLengthMilli() const { return mLengthMilli; }

	private:
		enum SoundType mSoundType;	// is this an Ogg, WAV, etc.?
		bool mIsInitialized;		// has the sound been initialized
		WAVEFORMATEX mWavFormatEx;	// description of the PCM format
		int mLengthMilli;			// how long the sound is in milliseconds
	};
}