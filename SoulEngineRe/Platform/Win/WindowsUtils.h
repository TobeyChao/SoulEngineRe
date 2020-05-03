#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
namespace Soul
{
	namespace Platform
	{
		class WindowsUtils
		{
		public:
			static bool MsgProcess();
			static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			static HWND HWnd;
		};
	}
}