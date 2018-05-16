#pragma once
#include "../Common.h"
#include "../Math/Math.h"
#include "GLContext.h"


namespace Bitz
{
	namespace Input
	{
		class Mouse;
	}
	namespace GFX
	{
		class Window
		{
			friend class Input::Mouse;
		public:
			Window(const std::wstring& title, uint32_t width, uint32_t height, HINSTANCE hInstance);
			~Window();

			virtual void Update();
			virtual GLContext_Ptr GetGLContext() const;
			virtual void SetQuitFunction(void (function)(void));
			virtual Bitz::Math::Vector2I GetWindowSize() const;
			virtual void SetWindowSize(const Bitz::Math::Vector2I& newSize);
		private:
			WNDCLASS _WC;
			HWND _HWnd;
			HDC _HDC;
			HGLRC _HRC;
			GLContext_Ptr _GlContext = nullptr;

			void(*_OnQuitEvent)() = nullptr;

			Bitz::Math::Vector2I _WindowSize;
			virtual void ApplyWindowSizeChange();

			static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		};

		typedef std::shared_ptr<Window> Window_Ptr;
	}
}
