#pragma once
#include "../Bitz-Core/Common.h"
#include "../Bitz-Core/Math/Math.h"
#include "../Bitz-Core/Graphics/GLContext.h"

namespace Bitz
{
	namespace PlatformSpecific
	{
		namespace Windows
		{
			namespace GFX
			{
				class GLContext : public Bitz::GFX::GLContext
				{
				public:
					GLContext();
					~GLContext();
					void Init(HWND hwnd);
					void MakeCurrent();
					void Clear(Vector3F colour);
					void FinishRender();

					HDC GetHDC() const;
				private:
					HGLRC _HRC;
					HWND _HWND;
					HDC _HDC;
				};

			}
		}
	}
}