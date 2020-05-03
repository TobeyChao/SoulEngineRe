#include "XAudioModule.h"
#include "../../SoulMain/Launcher.h"

namespace Soul
{
	void XAudioModule::Initialize()
	{
		mXAudio2Audio = new XAudio2Audio();
		Launcher::GetInstance().AddAudioSystem(mXAudio2Audio);
	}
	void XAudioModule::Uninitialize()
	{
		mXAudio2Audio->Shutdown();
		delete mXAudio2Audio;
		mXAudio2Audio = nullptr;
	}
}
