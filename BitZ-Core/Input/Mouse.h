#pragma once
#include "../Common.h"
#include "../Math/Math.h"

namespace Bitz
{

	namespace GFX
	{
		class GraphicsManager;
		class Window;
	}

	namespace Input
	{
		class Mouse
		{
			friend class GFX::GraphicsManager;
			friend class GFX::Window;
		public:
			/// <summary>
			/// Enum on the queryable mouse buttons
			/// </summary>
			enum MouseButtons { LEFT, MIDDLE, RIGHT, FOUR, FIVE };
			/// <summary>
			/// Returns the current position of the mouse
			/// </summary>
			/// <returns>Vector2I containing the mouse position relative to the window top left</returns>
			static Math::Vector2I GetPosition();
			/// <summary>
			/// Returns the current x position of the mouse
			/// </summary>
			/// <returns>int32_t containing the mouse x position relative to the window left</returns>
			static int32_t GetX();
			/// <summary>
			/// Returns the current y position of the mouse
			/// </summary>
			/// <returns>int32_t containing the mouse y position relative to the window top</returns>
			static int32_t GetY();
			/// <summary>
			/// Returns whether the specified mouse button is down
			/// </summary>
			/// <param name='button'>The button to test</param>
			/// <returns>True if the specified button is currently down else false</returns>
			static bool IsButtonDown(MouseButtons button);
		private:
			static Math::Vector2I _LastMouse;
			static bool _ButtonDown[5];

			//static void HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		};
	}
}

