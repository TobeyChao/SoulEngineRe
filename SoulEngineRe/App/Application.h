#pragma once
#include "../SoulMain/Launcher.h"
#include "../SoulMain/RenderSystem/FrameEventReceiver.h"
#include "../Platform/Win/CPUUse.h"

namespace Soul
{
	namespace App
	{
		class Application : public FrameEventReceiver
		{
		public:
			explicit Application();
			~Application();
			bool Initialize();
			void UnInitialize();

			virtual bool Setup();
			Launcher* GetLauncher() const
			{
				return mLauncher;
			}
			bool FrameStarted() override;
			bool FrameUpdated() override;
			bool FrameEnd() override;
		protected:
			int GetCPUUSe();

		protected:
			// Render Window
			RenderWindow* mWindow;

		private:
			CPUUse* mCPUUse;
			Launcher* mLauncher;
		};
	}
}