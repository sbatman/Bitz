#pragma once
#include "../Common.h"

namespace Bitz
{
	namespace GFX
	{
		class GraphicsManager;
		class Window;
	}

	namespace PlatformSpecific { namespace Windows { namespace GFX { class Window; } } }

	class Core;

	namespace Input
	{
		/// <summary>
		/// The keyboard class provides keyboard quering functionaly, the class interacts directly with
		/// the windows message loop to handle incoming WM_KEYDOWN and WM_KEYUP events.
		/// </summary>
		class Keyboard
		{
			typedef unsigned KEY;
			friend class Bitz::GFX::GraphicsManager;
			friend class Bitz::GFX::Window;
			friend class Bitz::PlatformSpecific::Windows::GFX::Window;
			friend class Bitz::Core;
		public:
			/// <summary>
			/// Returns whether the specified key was released since the last update
			/// </summary>
			/// <param name='key'>The virtual key id of the key to test</param>
			/// <returns>True if released else false</returns>
			static bool WasKeyReleased(KEY key);
			/// <summary>
			/// Returns whether the specified key was pressed since the last update
			/// </summary>
			/// <param name='key'>The virtual key id of the key to test</param>
			/// <returns>True if pressed else false</returns>
			static bool WasKeyPressed(KEY key);
			/// <summary>
			/// Returns whether the specified key is currently down
			/// </summary>
			/// <param name='key'>The virtual key id of the key to test</param>
			/// <returns>True if down else false</returns>
			static bool IsKeyDown(KEY key);
		private:
			static void Init();
			static void StaticDispose();
			static void Update();

#ifdef WIN32
			static void HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

			static bool _CurrentKeyStates[];
			static bool _PreviousKeyStates[];
		};
	}
}
