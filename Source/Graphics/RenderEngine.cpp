#include "..\Common.h"
#include "RenderEngine.h"
#include "Drawables\Model.h"
#include "Drawables\Sprite.h"
#include "..\Content\TextureData.h"
#include "..\Debug\Logging.h"
#include "Shaders\ShaderService.h"
#include "GraphicsManager.h"

namespace Bitz
{
	namespace GFX
	{
		RenderEngine::RenderEngine(Window * window)
		{
			assert(window != nullptr);

			_VertCachePos = 0;
			_ColCachePos = 0;
			_TexCachePos = 0;
			_NormCachePos = 0;

			_CurrentWindow = window;
			_CurrentRenderingContext = _CurrentWindow->GetGLContext();
			_CurrentRenderingContext->MakeCurrent();

			_VertCache = new float_t[BUFFERVERTCOUNT * 3];
			_ColCache = new float_t[BUFFERVERTCOUNT * 4];
			_TexCache = new float_t[BUFFERVERTCOUNT * 2];
			_NormCache = new float_t[BUFFERVERTCOUNT * 3];

			for (int i = 0;i < MAXTEXTUREUNITS;i++)_ActiveTexture[i] = nullptr;

			if (Settings::DEBUG_LOGGING_GRAPHICS) Debug::Logging::Log(Debug::Logging::ErrorType::Notice, "Created Render Engine");

		}

		RenderEngine::~RenderEngine()
		{
			if (_VertCache != nullptr)
			{
				delete[] _VertCache;
				_VertCache = nullptr;
			}
			if (_ColCache != nullptr)
			{
				delete[] _ColCache;
				_ColCache = nullptr;
			}
			if (_TexCache != nullptr)
			{
				delete[] _TexCache;
				_TexCache = nullptr;
			}
			_CurrentRenderingContext = nullptr;
			delete _CurrentWindow;
			_CurrentWindow = nullptr;
			for (int i = 0;i < MAXTEXTUREUNITS;i++)_ActiveTexture[i] = nullptr;
			Content::TextureData::ClearAllOpenGLIDs();
			if (Settings::DEBUG_LOGGING_GRAPHICS) Debug::Logging::Log(Debug::Logging::ErrorType::Notice, "Destroyed Render Engine");
		}

		void RenderEngine::Init()
		{
			assert(glGetError() == GL_NO_ERROR);

			glewInit();

			SetSize(_CurrentWindow->GetWindowSize());

			assert(glGetError() == GL_NO_ERROR);

			//glEnable(GL_MULTISAMPLE);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);

			assert(glGetError() == GL_NO_ERROR);
		}

		void RenderEngine::Update()
		{
			_CurrentWindow->Update();
		}

		void RenderEngine::Begin(Shaders::Shader_Ptr activeShader)
		{
			if (_ActiveShader != nullptr)_ActiveShader->Disable();
			_ActiveShader = activeShader != nullptr ? activeShader : Shaders::ShaderService::GetStandardShader();

			if (!_ActiveShader->IsCompiled())_ActiveShader->Compile();
			_ActiveShader->Enable();

			_VertGLCacheLoc = _ActiveShader->GetAttributeLocation("in_Position");
			_ColGLCacheLoc = _ActiveShader->GetAttributeLocation("in_Color");
			_NormGLCacheLoc = _ActiveShader->GetAttributeLocation("in_Normal");
			_TexGLCacheLoc = _ActiveShader->GetAttributeLocation("in_TexCoordinate");

			_ActiveShader->SetVariable("ProjectionMatrix", GraphicsManager::GetCurrentCamera()->GetProjectionMatrix());
			_ActiveShader->SetVariable("ViewMatrix", GraphicsManager::GetCurrentCamera()->GetViewMatrix());

			_VertCachePos = 0;
			_ColCachePos = 0;
			_TexCachePos = 0;
			_NormCachePos = 0;
			_RenderedVertCount = 0;
			_DrawIntervals.clear();
		}

		Window * RenderEngine::GetWindow()
		{
			return _CurrentWindow;
		}

		void RenderEngine::Render(Drawables::IDrawable* idrawable)
		{
			if (!idrawable->ShouldDraw())return;
			if (_RenderedVertCount + idrawable->GetVertCount() > BUFFERVERTCOUNT)
			{
				assert(false && "Render Buffer Overrun");
				Debug::Logging::Log(Debug::Logging::ErrorType::Warning, "Render Buffer Overrun");
				return;
			}

			if (_VertGLCacheLoc != -1)idrawable->PopulateVertArray(_VertCache, &_VertCachePos);
			if (_ColGLCacheLoc != -1)idrawable->PopulateColArray(_ColCache, &_ColCachePos);
			if (_NormGLCacheLoc != -1)idrawable->PopulateNormArray(_NormCache, &_NormCachePos);
			if (_TexGLCacheLoc != -1)idrawable->PopulateTexArray(_TexCache, &_TexCachePos);

			Content::TextureData_Ptr data = idrawable->GetTexture() != nullptr ? idrawable->GetTexture()->_Data : nullptr;

			bool is3D = idrawable->_RenderMode == Drawables::IDrawable::RenderMode::ThreeD;

			DrawInterval interval;
			bool intervalGenerated = false;

			if (_DrawIntervals.empty())
			{
				if (is3D)
				{
					Drawables::Model * theModel = static_cast<Drawables::Model *>(idrawable);
					interval = DrawInterval(uint32_t(0), uint32_t(-1), data, idrawable->_RenderMode, nullptr, (theModel)->GetTransformation(), theModel->GetSpecularTexture()->_Data);
					intervalGenerated = true;
				}
				else
				{
					interval = DrawInterval( uint32_t(0), uint32_t(-1), data, idrawable->_RenderMode,nullptr , glm::mat4() );
					intervalGenerated = true;
				}
			}
			else if (_DrawIntervals.back().Texture[0] != data || _DrawIntervals.back().Mode != idrawable->_RenderMode || is3D)
			{
				_DrawIntervals.back().VertCountEnd = _RenderedVertCount;
				if (is3D)
				{
					Drawables::Model * theModel = static_cast<Drawables::Model *>(idrawable);
					interval = DrawInterval(uint32_t(_RenderedVertCount), uint32_t(-1), data, idrawable->_RenderMode, nullptr, (theModel)->GetTransformation(), theModel->GetSpecularTexture()->_Data);
					intervalGenerated = true;
				}
				else
				{
					interval = DrawInterval( uint32_t(_RenderedVertCount), uint32_t(-1), data, idrawable->_RenderMode,nullptr , glm::mat4() );
					intervalGenerated = true;
				}

			}
			if(intervalGenerated)_DrawIntervals.push_back(interval);
			_RenderedVertCount += idrawable->GetVertCount();
			assert(glGetError() == GL_NO_ERROR);
		}

		void RenderEngine::SetSize(Vector2I newSize)
		{
			_CurrentWindow->SetWindowSize(newSize);
			glViewport(0, 0, newSize.X, newSize.Y);
			assert(glGetError() == GL_NO_ERROR);
		}

		void RenderEngine::End()
		{
			if (_VertGLCacheLoc != -1)
			{
				glVertexAttribPointer(_VertGLCacheLoc, 3, GL_FLOAT, false, 0, _VertCache);
				glEnableVertexAttribArray(_VertGLCacheLoc);
			}
			if (_ColGLCacheLoc != -1)
			{
				glVertexAttribPointer(_ColGLCacheLoc, 4, GL_FLOAT, false, 0, _ColCache);
				glEnableVertexAttribArray(_ColGLCacheLoc);
			}
			if (_NormGLCacheLoc != -1)
			{
				glVertexAttribPointer(_NormGLCacheLoc, 3, GL_FLOAT, false, 0, _NormCache);
				glEnableVertexAttribArray(_NormGLCacheLoc);
			}
			if (_TexGLCacheLoc != -1)
			{
				glVertexAttribPointer(_TexGLCacheLoc, 2, GL_FLOAT, false, 0, _TexCache);
				glEnableVertexAttribArray(_TexGLCacheLoc);
			}

			for (int i = 0;i < MAXTEXTUREUNITS;i++)	_ActiveShader->SetVariable(fmt::format("Texture{0}", i), i);

			if (_DrawIntervals.size() == 0)return;

			if (Settings::DEBUG_LOGGING_GRAPHICS_VERBOSE)Debug::Logging::Log(Debug::Logging::ErrorType::Notice, fmt::format("Rendering {0} Verts in {1} Intervals", _RenderedVertCount, _DrawIntervals.size()));

			_DrawIntervals.back().VertCountEnd = _RenderedVertCount;

			for (uint32_t i = 0; i < _DrawIntervals.size(); i++)
			{
				int texUnit = 0;
				for (auto tex : _DrawIntervals[i].Texture)
				{
					SetActiveTexture(tex, texUnit++);
				}

				if (_DrawIntervals[i].Mode == Drawables::IDrawable::RenderMode::ThreeD)
				{

					_ActiveShader->SetVariable("ModelMatrix", _DrawIntervals[i].Matrix);
					
					glm::vec4 lightDirection = glm::vec4(0.2f,0.0f,1.0f, 1.0f);

					_ActiveShader->SetVariable("LightDirection", glm::vec3(lightDirection.x, lightDirection.y, lightDirection.z));

				}
				glDrawArrays(GL_TRIANGLES, _DrawIntervals[i].VertCountStart, _DrawIntervals[i].VertCountEnd - _DrawIntervals[i].VertCountStart);

			}
			_ActiveShader->Disable();
			_ActiveShader = nullptr;
			assert(glGetError() == GL_NO_ERROR);
		}

		void RenderEngine::Present()
		{
			_CurrentRenderingContext->FinishRender();
			assert(glGetError() == GL_NO_ERROR);
		}

		void RenderEngine::Clear(Vector3F colour)
		{
			_CurrentRenderingContext->Clear(colour);
			assert(glGetError() == GL_NO_ERROR);
		}

		void RenderEngine::SetActiveTexture(Content::TextureData_Ptr activeTexture, int textureUnitID)
		{
			if (_ActiveTexture[textureUnitID] == activeTexture)return;

			glActiveTexture(GL_TEXTURE0 + textureUnitID);

			if (activeTexture == nullptr)
			{
				_ActiveTexture[textureUnitID] = nullptr;
				glBindTexture(GL_TEXTURE_2D, 0);

				assert(glGetError() == GL_NO_ERROR);
				return;
			}

			if (activeTexture->GetOpenglTextureID() != -1)
			{
				_ActiveTexture[textureUnitID] = activeTexture;
				glBindTexture(GL_TEXTURE_2D, activeTexture->GetOpenglTextureID());
			}
			else
			{
				GLuint texID;
				glGenTextures(1, &texID);
				activeTexture->SetOpenglTextureID(texID);
				glBindTexture(GL_TEXTURE_2D, texID);

				//do something sensible with this
				GLfloat largest_supported_anisotropy;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_supported_anisotropy);

				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, activeTexture->GetWidth(), activeTexture->GetHeight(),
					0, GL_BGRA, GL_UNSIGNED_BYTE, activeTexture->_PixelData);
			}

			int err = glGetError();
			assert(err == GL_NO_ERROR);
		}

		Vector2I RenderEngine::GetSize() const
		{
			return _CurrentWindow->GetWindowSize();
		}
	}
}