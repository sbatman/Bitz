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

			_ActiveTexture = nullptr;
			_TexturingEnabled = false;
			_NormalsEnabled = false;

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
			_ActiveTexture = nullptr;
			Content::TextureData::ClearAllOpenGLIDs();
			if (Settings::DEBUG_LOGGING_GRAPHICS) Debug::Logging::Log(Debug::Logging::ErrorType::Notice, "Destroyed Render Engine");
		}

		void RenderEngine::Init()
		{
			assert(glGetError() == GL_NO_ERROR);

			glewInit();

			SetSize(_CurrentWindow->GetWindowSize());

			assert(glGetError() == GL_NO_ERROR);

			glEnable(GL_MULTISAMPLE);
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
			glm::mat4 matrix = is3D ? (static_cast<Drawables::Model *>(idrawable))->GetTransformation() : glm::mat4();

			if (_DrawIntervals.empty())
			{
				DrawInterval interval = { uint32_t(0), uint32_t(-1), data, idrawable->_RenderMode,nullptr ,matrix };
				_DrawIntervals.push_back(interval);
			}
			else	if (_DrawIntervals.back().Texture != data
				|| _DrawIntervals.back().Mode != idrawable->_RenderMode
				|| is3D)
			{
				_DrawIntervals.back().VertCountEnd = _RenderedVertCount;
				DrawInterval interval = { uint32_t(_RenderedVertCount), uint32_t(-1), data, idrawable->_RenderMode,nullptr , matrix };

				_DrawIntervals.push_back(interval);
			}
			_RenderedVertCount += idrawable->GetVertCount();

			assert(glGetError() == GL_NO_ERROR);
		}

		void RenderEngine::SetSize(Vector2I newSize)
		{
			_CurrentWindow->SetWindowSize(newSize);
			glViewport(0, 0, newSize.X, newSize.Y);
			assert(glGetError() == GL_NO_ERROR);
		}

		void RenderEngine::EnableNormals(bool enabled)
		{
			if (enabled)
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, 0, _NormCache);
			}
			else
			{
				glDisableClientState(GL_NORMAL_ARRAY);
			}
			assert(glGetError() == GL_NO_ERROR);
		}

		bool RenderEngine::IsEnabledNormals() const
		{
			return _NormalsEnabled;
		}

		bool RenderEngine::IsEnabledTexturing() const
		{
			return _TexturingEnabled;
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

			if (_DrawIntervals.size() == 0)return;

			if (Settings::DEBUG_LOGGING_GRAPHICS)Debug::Logging::Log(Debug::Logging::ErrorType::Notice, fmt::format("Rendering {0} Verts in {1} Intervals", _RenderedVertCount, _DrawIntervals.size()));

			_DrawIntervals.back().VertCountEnd = _RenderedVertCount;

			for (uint32_t i = 0; i < _DrawIntervals.size(); i++)
			{
				SetActiveTexture(_DrawIntervals[i].Texture);
				if (_DrawIntervals[i].Mode == Drawables::IDrawable::RenderMode::ThreeD)
				{
					_ActiveShader->SetVariable("ModelMatrix", _DrawIntervals[i].Matrix);
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

		void RenderEngine::SetActiveTexture(Content::TextureData_Ptr activeTexture)
		{
			if (_ActiveTexture == activeTexture)return;

			if (activeTexture == nullptr)
			{
				_ActiveTexture = nullptr;
				glBindTexture(GL_TEXTURE_2D, 0);
				_TexturingEnabled = false;
				if (_TexturingEnabled)
				{
					_TexturingEnabled = false;
					glDisable(GL_TEXTURE_2D);
				}
				assert(glGetError() == GL_NO_ERROR);
				return;
			}

			if (activeTexture->GetOpenglTextureID() != -1)
			{
				_ActiveTexture = activeTexture;
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

			if (!_TexturingEnabled)
			{
				_TexturingEnabled = true;
				glEnable(GL_TEXTURE_2D);
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