#include "Input.h"
#define BUTTONDOWN(name, key) ( name.rgbButtons[key] & 0x80 )
#define KEYDOWN( name, key ) ( name[key] & 0x80 )

namespace Soul
{
	namespace Input
	{
		DXInput& DXInput::GetInstance()
		{
			static DXInput instance;
			return instance;
		}
		DXInput::DXInput()
		{
			m_directInput = NULL;
			m_keyboard = NULL;
			m_mouse = NULL;
		}
		bool DXInput::Initialize(HWND hWnd)
		{
			HINSTANCE hInstance = GetModuleHandle(NULL);
			HRESULT hr;
			m_hwnd = hWnd;
			hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
			if (FAILED(hr))
			{
				return false;
			}
			hr = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
			if (FAILED(hr))
			{
				return false;
			}
			hr = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
			if (FAILED(hr))
			{
				return false;
			}
			hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
			if (FAILED(hr))
			{
				return false;
			}
			hr = m_keyboard->SetCooperativeLevel(m_hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
			if (FAILED(hr))
			{
				return false;
			}
			hr = m_mouse->SetDataFormat(&c_dfDIMouse);
			if (FAILED(hr))
			{
				return false;
			}
			hr = m_mouse->SetCooperativeLevel(m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			if (FAILED(hr))
			{
				return false;
			}
			return true;
		}

		void DXInput::Shutdown()
		{
			if (m_mouse)
			{
				m_mouse->Unacquire();
				m_mouse->Release();
				m_mouse = 0;
			}

			// Release the keyboard.
			if (m_keyboard)
			{
				m_keyboard->Unacquire();
				m_keyboard->Release();
				m_keyboard = 0;
			}

			// Release the main interface to direct input.
			if (m_directInput)
			{
				m_directInput->Release();
				m_directInput = 0;
			}
		}

		int DXInput::Update()
		{
			m_keyboard->Acquire();
			m_mouse->Acquire();
			ReadKeyboard();
			ReadMouse();
			int res = 0;
			if (memcmp(m_keyboardState, m_lastKeyboardState, sizeof(m_keyboardState)) != 0)
			{
				res = 1;
			}
			if (memcmp(&m_mouseState, &m_mouseLastState, sizeof(m_mouseLastState)) != 0)
			{
				if (res == 1)
				{
					res = 3;
				}
				else
				{
					res = 2;
				}
			}
			return res;
		}

		const Core::SVector2 DXInput::GetMouseLocation()
		{
			POINT point;
			GetCursorPos(&point);
			ScreenToClient(m_hwnd, &point);
			return Core::SVector2{ (float)(point.x), (float)(point.y) };
		}

		bool DXInput::ReadKeyboard()
		{
			memcpy(m_lastKeyboardState, m_keyboardState, sizeof(m_keyboardState));
			m_keyboard->GetDeviceState(sizeof(m_keyboardState), &m_keyboardState);
			return true;
		}

		bool DXInput::ReadMouse()
		{
			m_mouseLastState = m_mouseState;
			m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState);
			return true;
		}

		bool DXInput::IsPressed(int num)
		{
			if (KEYDOWN(m_keyboardState, num))
			{
				return true;
			}
			return false;
		}

		bool DXInput::IsLastPressed(int num)
		{
			if (KEYDOWN(m_lastKeyboardState, num))
			{
				return true;
			}
			return false;
		}

		bool DXInput::IsButtonDown(int mouse)
		{
			if (BUTTONDOWN(m_mouseState, mouse))
			{
				return true;
			}
			return false;
		}

		bool DXInput::IsLastButtonDown(int mouse)
		{
			if (BUTTONDOWN(m_mouseLastState, mouse))
			{
				return true;
			}
			return false;
		}
	}
}