#pragma once
#include "../../BitzCore/Common.h"
#include "../../BitzCore/Math/Math.h"
#include "../../BitzCore/Graphics/GLContext.h"

using namespace Bitz::Math;

namespace Bitz
{
	namespace PlatformSpecific
	{
		namespace Android
		{
			namespace GFX
			{
				class GLContext : public Bitz::GFX::GLContext
				{
				public:
					GLContext();
					~GLContext();
					void Init(EGLDisplay display, EGLSurface  surface, EGLContext context);
					void MakeCurrent();
					void Clear(Vector3F colour);
					void FinishRender();

				private:
					EGLDisplay  _Display;
					EGLSurface  _Surface;
					EGLContext  _Context;
				};
			}
		}
	}
}
