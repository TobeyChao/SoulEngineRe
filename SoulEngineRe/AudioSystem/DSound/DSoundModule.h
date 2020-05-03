#pragma once
#include "../../SoulMain/Module/Module.h"
#include "DSoundAudio.h"

namespace Soul
{
	class DSoundModule final : public Module
	{
	public:
		void Initialize() override;
		void Uninitialize() override;
	private:
		DSoundAudio* mDSoundAudio = nullptr;
	};
}
