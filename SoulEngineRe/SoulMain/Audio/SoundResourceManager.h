#pragma once
#include "../SoulPCH.h"
#include "../Resource/ResourceManager.h"
namespace Soul
{
	class SoundResourceManager : public ResourceManager
	{
	public:
		~SoundResourceManager();
		static SoundResourceManager & GetInstance();
		IAudioBuffer * LoadSound(std::wstring soundName, std::wstring fileName);
		IAudioBuffer * GetSound(std::wstring soundName);
	private:
		SoundResourceManager();
	};
}