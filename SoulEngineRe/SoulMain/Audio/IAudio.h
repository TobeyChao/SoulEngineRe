#pragma once
#include "../SoulPCH.h"
namespace Soul
{
	enum AudioEngineType
	{
		AUDIO_ENGINE_TYPE_DSOUND,
		AUDIO_ENGINE_TYPE_XAUDIO,
	};
	enum SoundType
	{
		SOUND_TYPE_FIRST,
		SOUND_TYPE_MP3 = SOUND_TYPE_FIRST,
		SOUND_TYPE_WAVE,
		SOUND_TYPE_MIDI,
		SOUND_TYPE_OGG,

		// This needs to be the last sound type
		SOUND_TYPE_COUNT,
		SOUND_TYPE_UNKNOWN,
	};
	class IAudio
	{
	public:
		virtual bool Active() = 0;

		virtual IAudioBuffer *InitAudioBuffer(const std::wstring& name, std::shared_ptr<Resource> handle) = 0;

		virtual void StopAllSounds() = 0;
		virtual void PauseAllSounds() = 0;
		virtual void ResumeAllSounds() = 0;

		virtual bool Initialize() = 0;
		virtual void Shutdown() = 0;
		virtual IAudioBuffer * GetSound(const std::wstring& name) = 0;

		virtual const std::string& GetAudioSystemName() const = 0;
	};
}