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
					EGLConfig config;
					EGLSurface surface;
					EGLContext context;

					EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

					eglInitialize(display, 0, 0);

					eglChooseConfig(display, attribs, &config, 1, &numConfigs);

					eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

					ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

					surface = eglCreateWindowSurface(display, config, app->window, NULL);
					context = eglCreateContext(display, config, NULL, NULL);

					_GlContext = new Bitz::PlatformSpecific::Android::GFX::GLContext();
					_GlContext->Init(display, surface, context);

					_GlContext->MakeCurrent();

					eglQuerySurface(display, surface, EGL_WIDTH, &w);
					eglQuerySurface(display, surface, EGL_HEIGHT, &h);

					_WindowSize.X = w;
					_WindowSize.Y = h;	

					Bitz::GFX::GraphicsManager::Init(this);
				}

				Window::~Window()
				{
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

				}

				void Window::SetWindowSize(const Vector2I newSize)
				{
					//Fail silently as this is not possible on Android
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