#pragma once
#include "../../BitzCore/Common.h"
#include "../../BitzCore/Graphics/Window.h"
#include "GLContext.h"

namespace Bitz
{
	namespace PlatformSpecific
	{
		namespace Windows
		{
			namespace GFX
			{
				class Window : public Bitz::GFX::Window
				{
				public:
					Window(std::wstring title, uint32_t width, uint32_t height, HINSTANCE hInstance);
					~Window();

					virtual void Update() override;
					virtual GLContext * GetGLContext() const override;
					virtual void SetQuitFunction(void (function)(void)) override;
					virtual Vector2I GetWindowSize() const override;
					virtual void SetWindowSize(const Vector2I newSize) override;
				private:
					WNDCLASS _WC;
					HWND _HWnd;
					HDC _HDC;
					HGLRC _HRC;
					GLContext * _GlContext = nullptr;

					static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
					virtual void ApplyWindowSizeChange() override;
				};
			}
		}
	}
}
