#pragma once
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#include <dsound.h>
#include <mmsystem.h>
#include "../../SoulMain/Audio/Audio.h"

namespace Soul
{
	class DSoundAudio final : public Audio
	{
	public:
		DSoundAudio();
		bool Active() override;

		IAudioBuffer* InitAudioBuffer(const std::wstring& name, std::shared_ptr<Resource> handle) override;

		void Shutdown() override;
		bool Initialize() override;

		[[nodiscard]] const std::string& GetAudioSystemName() const override;
	protected:
		IDirectSound8* mDirectSound;

	private:
		HRESULT SetPrimaryBufferFormat(DWORD dwPrimaryChannels,
			DWORD dwPrimaryFreq,
			DWORD dwPrimaryBitRate);
	};
}