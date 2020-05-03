#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "../../SoulMain/Core/Mathematics/SVector2.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

namespace Soul
{
	namespace Input
	{
		class DXInput
		{
		public:
			static DXInput& GetInstance();
			bool Initialize(HWND);
			void Shutdown();
			int Update();

			const Core::SVector2 GetMouseLocation();
			bool IsPressed(int);
			bool IsLastPressed(int);
			bool IsButtonDown(int);
			bool IsLastButtonDown(int);
			DIMOUSESTATE GetMouseState() { return m_mouseState; }
			DIMOUSESTATE GetLastMouseState() { return m_mouseLastState; }
		private:
			DXInput();
			bool ReadKeyboard();
			bool ReadMouse();
		private:
			IDirectInput8* m_directInput;

			IDirectInputDevice8* m_keyboard;
			IDirectInputDevice8* m_mouse;
			BYTE m_keyboardState[256];
			BYTE m_lastKeyboardState[256];
			DIMOUSESTATE m_mouseState;
			DIMOUSESTATE m_mouseLastState;
			HWND m_hwnd;
		};
	}
}