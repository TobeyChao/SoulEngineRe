#include "DSoundModule.h"
#include "../../SoulMain/Launcher.h"

namespace Soul
{
	void DSoundModule::Initialize()
	{
		mDSoundAudio = new DSoundAudio();
		Launcher::GetInstance().AddAudioSystem(mDSoundAudio);
	}
	void DSoundModule::Uninitialize()
	{
		mDSoundAudio->Shutdown();
		delete mDSoundAudio;
		mDSoundAudio = nullptr;
	}
}
