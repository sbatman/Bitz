#pragma once
#include "../Common.h"
#include "../Math/Math.h"
#include "GLContext.h"


namespace Bitz
{

	namespace GFX
	{
		class Window
		{
		public:
			Window(std::wstring title, uint32_t width, uint32_t height, HINSTANCE hInstance);
			~Window();

			virtual void Update();
			virtual GLContext * GetGLContext() const;
			virtual void SetQuitFunction(void (function)(void));
			virtual Bitz::Math::Vector2I GetWindowSize() const;
			virtual void SetWindowSize(const Bitz::Math::Vector2I newSize);
		private:
			WNDCLASS _WC;
			HWND _HWnd;
			HDC _HDC;
			HGLRC _HRC;
			GLContext * _GlContext = nullptr;

			void(*OnQuitEvent)() = nullptr;

			Bitz::Math::Vector2I _WindowSize;
			virtual void ApplyWindowSizeChange();

			static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		};


	}
}
