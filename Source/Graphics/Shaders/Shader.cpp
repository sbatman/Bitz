#include "Shader.h"
#include "ShaderService.h"


namespace Bitz
{
	namespace GFX
	{
		namespace Shaders
		{
			static int32_t _LastAllocatedID = 0;

			bool Shader::IsCompiled() const
			{
				return _Compiled;
			}

			int32_t Shader::GetID() const
			{
				return _ID;
			}

			Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
			{
				SetGLSL(vertexShader, fragmentShader);
			}

			Shader::Shader()
			{

			}

			void Shader::SetGLSL(const std::string& vertexShader, const std::string& fragmentShader)
			{
				_ID = _LastAllocatedID++;
				_FragementShaderLength = fragmentShader.length() + 1;
				_VertexShaderLength = vertexShader.length() + 1;

#pragma warning( disable : 4996 ) 
				_VertexSource = vertexShader;
				_FragementSource = fragmentShader;
			}

			void Shader::Delete()
			{
				if (_Program != -1)glDeleteProgram(_Program);
				_Program = -1;
				_Compiled = false;
				_Enabled = false;
				ShaderService::UnRegisterShader(GetPtr());
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
				assert(!_VertexSource.empty() && "Vertex Shader is null");
				assert(!_VertexSource.empty() && "Fragement Shader is null");

				_Program = glCreateProgram();
				_VertexShader = glCreateShader(GL_VERTEX_SHADER);
				_FragementShader = glCreateShader(GL_FRAGMENT_SHADER);
				auto vertexSource = (_VertexSource.c_str());
				auto fragmentSource = (_FragementSource.c_str());
				glShaderSource(_VertexShader, 1, &vertexSource, nullptr);
				glShaderSource(_FragementShader, 1, &fragmentSource, nullptr);
				glCompileShader(_VertexShader);
				glCompileShader(_FragementShader);
				glAttachShader(_Program, _VertexShader);
				glAttachShader(_Program, _FragementShader);

				const bool vCompileOk = ShaderCompileSuccessful(_VertexShader);
				if (!vCompileOk)
				{
					GLsizei logLength = 0;
					glGetShaderiv(_VertexShader, GL_INFO_LOG_LENGTH, &logLength);
					GLchar* log_string = new GLchar[logLength + 1];
					glGetShaderInfoLog(_VertexShader, logLength, nullptr, log_string);
					printf("Vertex shader failure :%s", log_string);
				}
				const bool fCompileOk = ShaderCompileSuccessful(_FragementShader);
				if (!fCompileOk)
				{
					GLsizei logLength = 0;
					glGetShaderiv(_FragementShader, GL_INFO_LOG_LENGTH, &logLength);
					GLchar* log_string = new char[logLength + 1];
					glGetShaderInfoLog(_FragementShader, logLength, nullptr, log_string);
					printf("Fragment shader failure :%s", log_string);
				}
				_Compiled = vCompileOk && fCompileOk;
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

			int32_t Shader::GetAttributeLocation(const std::string& attributeName)const
			{
				return glGetAttribLocation(_Program, attributeName.c_str());
			}

			void Shader::SetVariable(const std::string& variableName, glm::mat4 matrix)
			{
				const GLint loc = glGetUniformLocation(_Program, variableName.c_str());
				if (loc != -1)
				{
					glUniformMatrix4fv(loc, 1, 0, &matrix[0][0]);
					assert(glGetError() == GL_NO_ERROR);
				}
			}
			void Shader::SetVariable(const std::string& variableName, int32_t value)
			{
				const GLint loc = glGetUniformLocation(_Program, variableName.c_str());
				if (loc != -1)
				{
					glUniform1i(loc, value);
					assert(glGetError() == GL_NO_ERROR);
				}
			}
			void Shader::SetVariable(const std::string& variableName, float_t value)
			{
				const GLint loc = glGetUniformLocation(_Program, variableName.c_str());
				if (loc != -1)
				{
					glUniform1f(loc, value);
					assert(glGetError() == GL_NO_ERROR);
				}
			}
			void Shader::SetVariable(const std::string& variableName, Bitz::Math::Vector3F value)
			{
				const GLint loc = glGetUniformLocation(_Program, variableName.c_str());
				if (loc != -1)
				{
					glUniform3f(loc, value.X, value.Y, value.Z);
					assert(glGetError() == GL_NO_ERROR);
				}
			}
			void Shader::SetVariable(const std::string& variableName, glm::vec3 value)
			{
				const GLint loc = glGetUniformLocation(_Program, variableName.c_str());
				if (loc != -1)
				{
					glUniform3fv(loc, 1, glm::value_ptr(value));
					assert(glGetError() == GL_NO_ERROR);
				}
			}
			void Shader::SetVariable(const std::string& variableName, Bitz::Math::Vector4F value)
			{
				const GLint loc = glGetUniformLocation(_Program, variableName.c_str());
				if (loc != -1)
				{
					glUniform4f(loc, value.X, value.Y, value.Z, value.W);
					assert(glGetError() == GL_NO_ERROR);
				}
			}
			void Shader::SetVariable(const std::string& variableName, glm::vec4 value)
			{
				const GLint loc = glGetUniformLocation(_Program, variableName.c_str());
				if (loc != -1)
				{
					glUniform4fv(loc, 1, glm::value_ptr(value));
					assert(glGetError() == GL_NO_ERROR);
				}
			}

			std::shared_ptr<Shader> Shader::GetPtr()
			{
				return shared_from_this();
			}
		}
	}
}