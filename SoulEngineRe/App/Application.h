#pragma once
#include "../SoulMain/Launcher.h"
#include "../SoulMain/RenderSystem/FrameEventReceiver.h"

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
			Launcher* GetAdmin() const
			{
				return mAdmin;
			}
			bool FrameStarted() override;
			bool FrameUpdated() override;
			bool FrameEnd() override;

		protected:
			// Render Window
			RenderWindow* mWindow;

		private:
			Launcher* mAdmin;
		};
	}
}