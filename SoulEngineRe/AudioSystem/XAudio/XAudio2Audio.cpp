#include "XAudio2Audio.h"
#include "../../SoulMain/Audio/SoundResourceExtraData.h"
#include "../../SoulMain/Resource/Resource.h"
#include "XAudio2AudioBuffer.h"

namespace Soul
{
	XAudio2Audio::XAudio2Audio()
		:
		mXAudio(nullptr),
		mMasteringVoice(nullptr),
		mFormat(std::make_unique<WAVEFORMATEX>())
	{
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		auto hr = XAudio2Create(mXAudio.ReleaseAndGetAddressOf());
		hr = mXAudio->CreateMasteringVoice(&mMasteringVoice);
		XAUDIO2_DEBUG_CONFIGURATION debugConfig = { 0 };
		debugConfig.TraceMask = XAUDIO2_LOG_DETAIL | XAUDIO2_LOG_WARNINGS;
		mXAudio->SetDebugConfiguration(&debugConfig);
		mXAudio->StartEngine();
	}
	bool XAudio2Audio::Active()
	{
		return mXAudio != nullptr;
	}
	IAudioBuffer* XAudio2Audio::InitAudioBuffer(const std::wstring& name, std::shared_ptr<Resource> handle)
	{
		std::shared_ptr<SoundResourceExtraData> extra =
			std::static_pointer_cast<SoundResourceExtraData>(handle->GetExtra());
		if (!mXAudio)
		{
			return nullptr;
		}
		switch (extra->GetSoundType())
		{
		case SOUND_TYPE_MP3:
			break;
		case SOUND_TYPE_WAVE:
			break;
		case SOUND_TYPE_MIDI:
			return nullptr;
		case SOUND_TYPE_OGG:
			return nullptr;
		default:
			return nullptr;
		}

		IXAudio2SourceVoice* sampleHandle = nullptr;

		mXAudio->CreateSourceVoice(&sampleHandle, extra->GetFormat());

		IAudioBuffer* audioBuffer = new XAudio2AudioBuffer(sampleHandle, handle);
		mAllSamples[name] = audioBuffer;

		return audioBuffer;
	}
	void XAudio2Audio::Shutdown()
	{
		if (mInitialized)
		{
			if (mMasteringVoice != nullptr)
				mMasteringVoice->DestroyVoice();

			Audio::Shutdown();
			if (mXAudio)
			{
				mXAudio.Reset();
			}
			mInitialized = false;
			CoUninitialize();
		}
	}
	bool XAudio2Audio::Initialize()
	{
		mInitialized = true;
		return true;
	}
	const std::string& XAudio2Audio::GetAudioSystemName() const
	{
		static std::string name = "xaudio2";
		return name;
	}
}