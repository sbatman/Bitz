#include "GLContext.h"

namespace Bitz
{
	namespace PlatformSpecific
	{
		namespace Windows
		{
			namespace GFX
			{
				GLContext::GLContext()
				{
				}

				GLContext::~GLContext()
				{
					wglMakeCurrent(nullptr, nullptr);
					wglDeleteContext(_HRC);
					ReleaseDC(_HWND, _HDC);
				}

				void GFX::GLContext::Init(HWND hwnd)
				{
					_HWND = hwnd;
					_HDC = GetDC(_HWND);
					PIXELFORMATDESCRIPTOR pfd;
					int32_t iFormat;

					// set the pixel format for the DC
					ZeroMemory(&pfd, sizeof(pfd));
					pfd.nSize = sizeof(pfd);
					pfd.nVersion = 1;
					pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
					pfd.iPixelType = PFD_TYPE_RGBA;
					pfd.cColorBits = 24;
					pfd.cDepthBits = 16;
					pfd.iLayerType = PFD_MAIN_PLANE;
					iFormat = ChoosePixelFormat(_HDC, &pfd);
					SetPixelFormat(_HDC, iFormat, &pfd);

					// create and enable the render context (RC)
					_HRC = wglCreateContext(_HDC);
				}

				void GFX::GLContext::MakeCurrent()
				{
					wglMakeCurrent(_HDC, _HRC);
				}

				HDC GFX::GLContext::GetHDC() const
				{
					return _HDC;
				}

				void GFX::GLContext::Clear(Vector3F colour)
				{
					glClear(GL_COLOR_BUFFER_BIT);
					glClearColor(colour.X, colour.Y, colour.Z, 255);
				}

				void GFX::GLContext::FinishRender()
				{
					SwapBuffers(_HDC);
				}
			}
		}
	}
}