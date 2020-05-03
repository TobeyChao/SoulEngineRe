#pragma once
#include <wrl\client.h>
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>
#include <xapofx.h>
#include "../../SoulMain/Audio/Audio.h"

namespace Soul
{
	class XAudio2Audio final : public Audio
	{
	public:
		XAudio2Audio();
		bool Active() override;

		IAudioBuffer* InitAudioBuffer(const std::wstring& name, std::shared_ptr<Resource> handle) override;

		void Shutdown() override;
		bool Initialize() override;

		[[nodiscard]] const std::string& GetAudioSystemName() const override;
	private:
		Microsoft::WRL::ComPtr<IXAudio2>	mXAudio;
		IXAudio2MasteringVoice* mMasteringVoice;
		X3DAUDIO_HANDLE	mX3DAudio;

		std::unique_ptr<WAVEFORMATEX>	mFormat;
	};
}