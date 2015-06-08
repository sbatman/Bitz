#include "../Bitz-Core/Common.h"
#include "GLContext.h"
#include "../Bitz-Core/Graphics/Window.h"
#include "../Bitz-Core/Graphics/GraphicsManager.h"
#include "Window.h"
#include "../Bitz-Core/Input/Keyboard.h"
#include "../Bitz-Core/Input/Mouse.h"

namespace Bitz
{
	namespace PlatformSpecific
	{
		namespace Android
		{
			namespace GFX
			{
				Window::Window(android_app * app)
				{
					const EGLint attribs[] = {
						EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
						EGL_BLUE_SIZE, 8,
						EGL_GREEN_SIZE, 8,
						EGL_RED_SIZE, 8,
						EGL_NONE
					};
					EGLint w, h, format;
					EGLint numConfigs;


					_NativeApp = app;

					display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

					eglInitialize(display, 0, 0);

					eglChooseConfig(display, attribs, &config, 1, &numConfigs);

					eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

					ANativeWindow_setBuffersGeometry(_NativeApp->window, 0, 0, format);

					surface = eglCreateWindowSurface(display, config, _NativeApp->window, NULL);
					context = eglCreateContext(display, config, NULL, NULL);


					_GlContext = new Bitz::PlatformSpecific::Android::GFX::GLContext();
					_GlContext->Init(display, surface, context);

					_GlContext->MakeCurrent();

					eglQuerySurface(display, surface, EGL_WIDTH, &w);
					eglQuerySurface(display, surface, EGL_HEIGHT, &h);

					_WindowSize.X = w;
					_WindowSize.Y = h;


					Bitz::GFX::GraphicsManager::Init(this);

					Bitz::GFX::GraphicsManager::SetScreenSize(Vector2I(w, h));
				}

				Window::~Window()
				{
					delete _GlContext;
					_GlContext = nullptr;
					eglDestroyContext(display, context);
					eglDestroySurface(display, surface);

				}

				GLContext * Window::GetGLContext() const
				{
					return _GlContext;
				}

				Vector2I Window::GetWindowSize() const
				{
					return _WindowSize;
				}

				void Window::Update()
				{
					int ident;
					int events;
					struct android_poll_source* source;
					while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
					{
						if (source != NULL && _NativeApp !=nullptr)
						{
							source->process(_NativeApp, source);
						}
					}

					EGLint w, h;
					eglQuerySurface(display, surface, EGL_WIDTH, &w);
					eglQuerySurface(display, surface, EGL_HEIGHT, &h);
					if ((_WindowSize.X != w || _WindowSize.Y != h) && w != 0 && h != 0)
					{
						Bitz::GFX::GraphicsManager::SetScreenSize(Vector2I(w, h));
					}

				}

				void Window::SetWindowSize(const Vector2I newSize)
				{
					_WindowSize = newSize;
				}

				void Window::SetQuitFunction(void(function)(void))
				{
					OnQuitEvent = function;
				}

				void Window::ApplyWindowSizeChange()
				{
				}
			}
		}
	}
}