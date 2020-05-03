#pragma once
#include "../../SoulMain/Module/Module.h"
#include "XAudio2Audio.h"

namespace Soul
{
	class XAudioModule final : public Module
	{
	public:
		void Initialize() override;
		void Uninitialize() override;
	private:
		XAudio2Audio* mXAudio2Audio = nullptr;
	};
}
