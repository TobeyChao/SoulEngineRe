#pragma once
#include "../SoulPCH.h"
#include "../Resource/ResourceManager.h"
namespace Soul
{
	class SoundResourceManager : public ResourceManager, public Singleton<SoundResourceManager>
	{
	public:
		SoundResourceManager();
		~SoundResourceManager();
		IAudioBuffer * LoadSound(std::wstring soundName, std::wstring fileName);
		IAudioBuffer * GetSound(std::wstring soundName);
	};
}