#pragma once
#include "../Common.h"
#include "../Math/Math.h"

namespace Bitz
{

	namespace GFX
	{
		class GLContext
		{
		public:
			GLContext();
			~GLContext();
			void Init(HWND hwnd);
			void MakeCurrent() const;
			static void Clear(Bitz::Math::Vector3F colour);
			void FinishRender() const;

			HDC GetHDC() const;
		private:
			HGLRC _HRC;
			HWND _HWND;
			HDC _HDC;
		};

		typedef std::shared_ptr<GLContext> GLContext_Ptr;
	}
}

