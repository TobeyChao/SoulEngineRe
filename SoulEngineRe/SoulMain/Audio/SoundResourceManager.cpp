#include "SoundResourceManager.h"
#include "../Resource/ResourceGroupManager.h"
#include "../Resource/WaveResourceLoader.h"
#include "../Launcher.h"
#include "IAudio.h"
#include "IAudioBuffer.h"

namespace Soul
{
	SoundResourceManager& SoundResourceManager::GetInstance()
	{
		static SoundResourceManager instance;
		return instance;
	}

	SoundResourceManager::SoundResourceManager()
	{
		ResourceGroupManager::GetInstance().RegisterResourceManager(L"Sound", this);
		RegisterLoader(std::make_shared<WaveResourceLoader>());
	}

	SoundResourceManager::~SoundResourceManager()
	{
		ResourceGroupManager::GetInstance().UnregisterResourceManager(L"Sound");
	}

	IAudioBuffer* SoundResourceManager::LoadSound(std::wstring soundName, std::wstring fileName)
	{
		IAudioBuffer* buffer = Launcher::GetInstance().GetActiveAudioSystem()->InitAudioBuffer(soundName, GetHandle(fileName));
		return buffer;
	}
	IAudioBuffer* SoundResourceManager::GetSound(std::wstring soundName)
	{
		return Launcher::GetInstance().GetActiveAudioSystem()->GetSound(soundName);
	}
}
