#pragma once
#include "../../Bitz-Core/Common.h"
#include "../../Bitz-Core/Graphics/Window.h"
#include "GLContext.h"
#include "../android_native_app_glue.h"

namespace Bitz
{
	namespace PlatformSpecific
	{
		namespace Android
		{
			namespace GFX
			{
				class Window : public Bitz::GFX::Window
				{
				public:
					Window(android_app * app);
					~Window();
					virtual void Update() override;
					virtual GLContext * GetGLContext() const override;
					virtual Vector2I GetWindowSize() const override;
					virtual void SetWindowSize(const Vector2I newSize) override;
					virtual void SetQuitFunction(void(function)(void)) override;
					virtual	void ApplyWindowSizeChange()override;
				private:
					GLContext * _GlContext = nullptr;
					android_app * _NativeApp = nullptr;
					EGLSurface surface;
					EGLContext context;
					EGLDisplay display;
					EGLConfig config;
				};
			}
		}
	}
}
