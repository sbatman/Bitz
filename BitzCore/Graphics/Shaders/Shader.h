#pragma once
#include "../../Common.h"
#include "../../Interfaces/IDisposable.h"

namespace Bitz
{
	namespace GFX
	{
		namespace Shaders
		{
			class Shader : public Interfaces::IDisposable
			{
			public:
				Shader(const std::string vertexShader, const std::string fragmentShader);
				~Shader();
				bool virtual Compile();
				bool virtual IsCompiled() const;
				int virtual GetID() const;
				void virtual Enable();
				void virtual Disable();
				void virtual Dispose() override;

			protected:
				Shader();
				void SetGLSL(const std::string vertexShader, const std::string fragmentShader);
			private:
				char * _FragementSource = nullptr;
				char * _VertexSource = nullptr;
				int32_t _FragementShaderLength = 0;
				int32_t _VertexShaderLength = 0;
				GLenum _Program = -1;
				GLenum _VertexShader = -1;
				GLenum _FragementShader = -1;
				bool _Compiled = false;
				int32_t _ID = -1;
				bool _Enabled = false;

				bool ShaderCompileSuccessful(const int32_t obj);
				bool ShaderLinkSuccessful(const int32_t obj);
			};
			typedef std::shared_ptr<Shader> Shader_Ptr;
		}
	}
}


