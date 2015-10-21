#include "../Common.h"
#include "../Math/Math.h"
#include "../Graphics/GraphicsManager.h"
#include "Mouse.h"

namespace Bitz
{
	namespace Input
	{
		Math::Vector2I Mouse::_LastMouse(0);
		bool Mouse::_ButtonDown[5]{ 0 };

		Math::Vector2I Mouse::GetPosition()
		{
			return Math::Vector2I();
		}

		int32_t Mouse::GetX()
		{
			return _LastMouse.X;
		}

		int32_t Mouse::GetY()
		{
			return _LastMouse.Y;
		}

		bool Mouse::IsButtonDown(MouseButtons button)
		{
			return _ButtonDown[button];
		}

#ifdef WIN32

		void Mouse::HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			switch (message)
			{
			case WM_MOUSEMOVE:
			{
				int32_t screenHeight = Bitz::GFX::GraphicsManager::GetScreenSize().Y;

				int32_t xPos = LOWORD(lParam);
				int32_t yPos = HIWORD(lParam);

				if (xPos != _LastMouse.X || yPos != _LastMouse.Y)
				{
					_LastMouse.X = xPos;
					_LastMouse.Y = yPos;
				}
			}
			break;
			case WM_LBUTTONDOWN:
				_ButtonDown[MouseButtons::LEFT] = true;
				break;
			case WM_LBUTTONUP:
				_ButtonDown[MouseButtons::LEFT] = false;
				break;
			case WM_MBUTTONDOWN:
				_ButtonDown[MouseButtons::MIDDLE] = true;
				break;
			case WM_MBUTTONUP:
				_ButtonDown[MouseButtons::MIDDLE] = false;
				break;
			case WM_RBUTTONDOWN:
				_ButtonDown[MouseButtons::RIGHT] = true;
				break;
			case WM_RBUTTONUP:
				_ButtonDown[MouseButtons::RIGHT] = false;
				break;
			case WM_XBUTTONDOWN:
				if (wParam & 0x10000)_ButtonDown[MouseButtons::FOUR] = true;
				if (wParam & 0x20000)_ButtonDown[MouseButtons::FIVE] = true;
				break;
			case WM_XBUTTONUP:
				if (wParam & 0x10000)_ButtonDown[MouseButtons::FOUR] = false;
				if (wParam & 0x20000)_ButtonDown[MouseButtons::FIVE] = false;
				break;
			}
		}
#endif
	}
}