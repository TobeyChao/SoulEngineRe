#pragma once
#include <dsound.h>
#include "../../SoulMain/Audio/AudioBuffer.h"

namespace Soul
{
	class DSoundAudioBuffer final : public AudioBuffer
	{
	public:
		DSoundAudioBuffer(LPDIRECTSOUNDBUFFER8 sample, std::shared_ptr<Resource> resource);

		void* Get() override;

		bool Play(int volume, bool looping) override;
		bool Pause() override;
		bool Stop() override;
		bool Resume() override;

		bool TogglePause() override;
		bool IsPlaying() override;
		void SetVolume(int volume) override;
		void SetPosition(unsigned long newPosition) override;

		float GetProgress() override;
	private:
		HRESULT FillBufferWithSound();
	protected:
		LPDIRECTSOUNDBUFFER8 mSample;
	};
}