#include "ShaderService.h"

namespace Bitz
{
	namespace GFX
	{
		namespace Shaders
		{
			std::vector<Shader_Ptr> ShaderService::_KnownShaders = std::vector<Shader_Ptr>();
			Shader_Ptr ShaderService::_StandardShader = nullptr;

			void ShaderService::RegisterShader(Shader_Ptr shaderInstance)
			{
				_KnownShaders.push_back(shaderInstance);
			}

			void ShaderService::UnRegisterShader(Shader_Ptr shaderInstance)
			{
				_KnownShaders.erase(std::remove(_KnownShaders.begin(), _KnownShaders.end(), shaderInstance), _KnownShaders.end());
			}

			std::vector<Shader_Ptr> ShaderService::GetCurrentShaders()
			{
				return _KnownShaders;
			}

			Shader_Ptr ShaderService::GetStandardShader()
			{
				if (_StandardShader == nullptr)
				{

				}
				return _StandardShader;
			}
		}
	}
}