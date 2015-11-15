#pragma once
#include "../../Common.h"

namespace Bitz
{
	namespace GFX
	{
		namespace Shaders
		{
			class Shader
			{
			public:
				Shader(std::string vertexShader, std::string fragmentShader);
				~Shader();

				bool virtual Compile();
				bool virtual IsCompiled() const;
				int virtual GetID() const;
				void virtual Enable();
				void virtual Disable();

			private:
				char * _FragementSource;
				char * _VertexSource;
				int32_t _FragementShaderLength;
				int32_t _VertexShaderLength;
				GLenum _Program;
				GLenum _VertexShader;
				GLenum _FragementShader;
				bool _Compiled = false;
				int32_t _ID;
				bool _Enabled;

				bool ShaderCompileSuccessful(int32_t obj);
				bool ShaderLinkSuccessful(int32_t obj);
			};
			typedef std::shared_ptr<Shader> Shader_Ptr;
		}
	}
}


