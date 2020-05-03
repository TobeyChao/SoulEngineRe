#include "WindowsUtils.h"

namespace Soul
{
	namespace Platform
	{
		HWND WindowsUtils::HWnd = nullptr;
		bool WindowsUtils::MsgProcess()
		{
			MSG msg;
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				if (msg.message == WM_QUIT)
				{
					return false;
				}
			}
			return true;
		}

		LRESULT WindowsUtils::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			switch (uMsg)
			{
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			default:
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			return 0;
		}
	}
}