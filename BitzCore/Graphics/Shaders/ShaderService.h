#pragma once
#include "../../Common.h"
#include "Shader.h"

namespace Bitz
{
	class Core;

	namespace GFX
	{
		namespace Shaders
		{
			class ShaderService
			{
				friend Core;
			public:

			private:
				static	std::vector<Shader> _KnownShaders;
			};
		}
	}
}