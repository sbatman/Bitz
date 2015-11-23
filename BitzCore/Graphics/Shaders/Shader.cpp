#include "Shader.h"
#include "ShaderService.h"

namespace Bitz
{
	namespace GFX
	{
		namespace Shaders
		{
			static int32_t _LastAllocatedID = 0;
			Shader_Ptr _Myself = nullptr;

			Shader::~Shader()
			{
				AttemptDispose();
			}

			bool Shader::IsCompiled() const
			{
				return _Compiled;
			}

			int32_t Shader::GetID() const
			{
				return _ID;
			}

			Shader::Shader(const std::string vertexShader, const std::string fragmentShader)
			{
				_Myself = Shader_Ptr(this);
				ShaderService::RegisterShader(_Myself);
				SetGLSL(vertexShader, fragmentShader);
			}

			Shader::Shader()
			{
				_Myself = Shader_Ptr(this);
				ShaderService::RegisterShader(_Myself);
			}

			void Shader::SetGLSL(const std::string vertexShader, const std::string fragmentShader)
			{
				_ID = _LastAllocatedID++;
				_FragementShaderLength = fragmentShader.length() + 1;
				_VertexShaderLength = vertexShader.length() + 1;

				if (_FragementSource != nullptr)delete _FragementSource;
				if (_VertexSource != nullptr)delete _VertexSource;
#pragma warning( disable : 4996 ) 
				_VertexSource = new char[_VertexShaderLength];
				_FragementSource = new char[_FragementShaderLength];

				vertexShader.copy(_VertexSource, _VertexShaderLength, 0);
				_VertexSource[_VertexShaderLength - 1] = 0;
				fragmentShader.copy(_FragementSource, _FragementShaderLength, 0);
				_FragementSource[_FragementShaderLength - 1] = 0;
			}

			void Shader::Dispose()
			{
				if (_Myself == nullptr)return;
				_Myself = nullptr;
				ShaderService::UnRegisterShader(_Myself);
				if (_Program != -1)glDeleteProgram(_Program);
				if (_FragementSource != nullptr)delete _FragementSource;
				if (_VertexSource != nullptr)delete _VertexSource;				
			}

			void Shader::Enable()
			{
				if (_Enabled)return;

				_Enabled = true;
				glLinkProgram(_Program);
				glUseProgram(_Program);
			}

			void Shader::Disable()
			{
				if (!_Enabled)return;
				_Enabled = false;
				glUseProgram(NULL);
			}

			bool Shader::Compile()
			{
				assert(_VertexSource != nullptr&& "Vertex Shader is null");
				assert(_FragementSource != nullptr&& "Fragement Shader is null");

				_Program = glCreateProgram();
				_VertexShader = glCreateShader(GL_VERTEX_SHADER);
				_FragementShader = glCreateShader(GL_FRAGMENT_SHADER);

				glShaderSource(_VertexShader, 1, &_VertexSource, NULL);
				glShaderSource(_FragementShader, 1, &_FragementSource, NULL);
				glCompileShader(_VertexShader);
				glCompileShader(_FragementShader);
				glAttachShader(_Program, _VertexShader);
				glAttachShader(_Program, _FragementShader);

				bool vCompileOK = ShaderCompileSuccessful(_VertexShader);
				if (!vCompileOK)
				{
					GLsizei logLength = 0;
					glGetShaderiv(_VertexShader, GL_INFO_LOG_LENGTH, &logLength);
					GLchar* log_string = new GLchar[logLength + 1];
					glGetShaderInfoLog(_VertexShader, logLength, 0, log_string);
					printf("Vertex shader failure :%s", log_string);
				}
				bool fCompileOK = ShaderCompileSuccessful(_FragementShader);
				if (!fCompileOK)
				{
					GLsizei logLength = 0;
					glGetShaderiv(_FragementShader, GL_INFO_LOG_LENGTH, &logLength);
					GLchar* log_string = new char[logLength + 1];
					glGetShaderInfoLog(_FragementShader, logLength, 0, log_string);
					printf("Fragment shader failure :%s", log_string);
				}
				_Compiled = vCompileOK && fCompileOK;
				return _Compiled;
			}

			bool Shader::ShaderCompileSuccessful(const int32_t obj)
			{
				int status;
				glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
				return status == GL_TRUE;
			}

			bool Shader::ShaderLinkSuccessful(const int32_t obj)
			{
				int status;
				glGetProgramiv(obj, GL_LINK_STATUS, &status);
				return status == GL_TRUE;
			}

			int32_t Shader::GetAttributeLocation(std::string attributeName)const
			{
				return glGetAttribLocation(_Program, attributeName.c_str());
			}

			void Shader::SetVariable(std::string variableName, glm::mat4 matrix)
			{
				GLint loc = glGetUniformLocation(_Program, variableName.c_str());
				if (loc != -1)
				{
					glUniformMatrix4fv(loc, 1, 0, &matrix[0][0]);
				}
			}
		}
	}
}