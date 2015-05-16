#pragma once

#include "../Math/Math.h"

namespace Bitz
{
	using namespace Math;

	namespace GFX
	{
		class GLContext
		{
		public:
			GLContext();
			~GLContext();
			virtual void Init() = 0;
			virtual void MakeCurrent() = 0;
			virtual void Clear(Vector3F colour);
			virtual void FinishRender() = 0;
		};
	}
}
