#include "GLContext.h"

namespace Bitz
{
	namespace PlatformSpecific
	{
		namespace Android
		{
			namespace GFX
			{
				GLContext::GLContext()
				{
				}

				GLContext::~GLContext()
				{
				}

				void GFX::GLContext::Init(EGLDisplay display, EGLSurface surface, EGLContext context)
				{
					_Display = display;
					_Surface = surface;
					_Context = context;
				}

				void GFX::GLContext::MakeCurrent()
				{
					eglMakeCurrent(_Display, _Surface, _Surface, _Context);
				}

				void GFX::GLContext::Clear(Vector3F colour)
				{
					glClear(GL_COLOR_BUFFER_BIT);
					glClearColor(colour.X, colour.Y, colour.Z, 255);
				}

				void GFX::GLContext::FinishRender()
				{
					eglSwapBuffers(_Display, _Surface);
				}
			}
		}
	}
}