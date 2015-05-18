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
			virtual ~GLContext() {};
			virtual void MakeCurrent() = 0;
			virtual void Clear(Vector3F colour) = 0;
			virtual void FinishRender() = 0;
		};
	}
}
