#pragma once
#include "../../Common.h"

namespace Bitz
{
	class Core;

	namespace GFX
	{
		namespace Shaders
		{
			class Shader;
			typedef std::shared_ptr<Shader> Shader_Ptr;

			class ShaderService
			{
				friend Core;
			private:
				static	std::vector<Shader_Ptr> _KnownShaders;

				static void RegisterShader(Shader_Ptr shaderInstance);
				static void UnRegisterShader(Shader_Ptr shaderInstance);
				static std::vector<Shader_Ptr> GetCurrentShaders();
			};
		}
	}
}