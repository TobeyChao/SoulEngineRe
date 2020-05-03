#pragma once
#include <xaudio2.h>
#include "../../SoulMain/Audio/AudioBuffer.h"

namespace Soul
{
	class XAudio2AudioBuffer final : public AudioBuffer
	{
	public:
		XAudio2AudioBuffer(IXAudio2SourceVoice * voice, std::shared_ptr<Resource> resource);

		void *Get() override;

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
		std::unique_ptr<XAUDIO2_BUFFER> mXAudioBuffer;
		IXAudio2SourceVoice * mVoice;
	};
}