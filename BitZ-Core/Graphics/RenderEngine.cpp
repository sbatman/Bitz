#include "../Common.h"
#include "RenderEngine.h"
#include "Drawables\Model.h"
#include "Drawables\Sprite.h"
#include "../Content/TextureData.h"
#include "../Debug/Logging.h"

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
		}

		RenderEngine::~RenderEngine()
		{
			if(_VertCache != nullptr)
			{
				delete[] _VertCache;
				_VertCache = nullptr;
			}
			if(_ColCache != nullptr)
			{
				delete[] _ColCache;
				_ColCache = nullptr;
			}
			if(_TexCache != nullptr)
			{
				delete[] _TexCache;
				_TexCache = nullptr;
			}
			_CurrentRenderingContext = nullptr;
			delete _CurrentWindow;
			_CurrentWindow = nullptr;
			_ActiveTexture = nullptr;
			Content::TextureData::ClearAllOpenGLIDs();
		}

		void RenderEngine::Init()
		{
			assert(glGetError() == GL_NO_ERROR);

			glewInit();

			SetSize(_CurrentWindow->GetWindowSize());

			assert(glGetError() == GL_NO_ERROR);

			glMatrixMode(GL_MODELVIEW);

			assert(glGetError() == GL_NO_ERROR);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			assert(glGetError() == GL_NO_ERROR);

			glEnable(GL_MULTISAMPLE);

			assert(glGetError() == GL_NO_ERROR);
		}

		void RenderEngine::Update()
		{
			_CurrentWindow->Update();
		}

		void RenderEngine::Begin()
		{
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
			if(!idrawable->ShouldDraw())return;
			if(_RenderedVertCount + idrawable->GetVertCount() > BUFFERVERTCOUNT)
			{
				assert(false && "Render Buffer Overrun");
				Debug::Logging::Log(Debug::Logging::ErrorType::Warning, "Render Buffer Overrun");
				return;
			}

			idrawable->PopulateVertArray(_VertCache, &_VertCachePos);
			idrawable->PopulateColArray(_ColCache, &_ColCachePos);
			idrawable->PopulateTexArray(_TexCache, &_TexCachePos);
			idrawable->PopulateNormArray(_NormCache, &_NormCachePos);

			Content::TextureData * data = idrawable->GetTexture() != nullptr ? idrawable->GetTexture()->_Data : nullptr;

			if(_DrawIntervals.empty())
			{
				DrawInterval interval = { uint32_t(0), uint32_t(-1), data, idrawable->_RenderMode, idrawable };
				_DrawIntervals.push_back(interval);
			}
			else	if(_DrawIntervals.back().Texture != data
				|| _DrawIntervals.back().Mode != idrawable->_RenderMode
				|| idrawable->_RenderMode == Drawables::IDrawable::RenderMode::ThreeD)
			{
				_DrawIntervals.back().VertCountEnd = _RenderedVertCount;

				DrawInterval interval = { uint32_t(_RenderedVertCount), uint32_t(-1), data, idrawable->_RenderMode, idrawable };
				_DrawIntervals.push_back(interval);
			}
			_RenderedVertCount += idrawable->GetVertCount();

			assert(glGetError() == GL_NO_ERROR);
		}

		void RenderEngine::SetSize(Vector2I newSize)
		{
			_CurrentWindow->SetWindowSize(newSize);
			glViewport(0, 0, _CurrentWindow->GetWindowSize().X, _CurrentWindow->GetWindowSize().Y);
			assert(glGetError() == GL_NO_ERROR);
		}

		void RenderEngine::EnableNormals(bool enabled)
		{
			if(enabled)
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

		bool RenderEngine::IsEnabledNormals()
		{
			return _NormalsEnabled;
		}

		bool RenderEngine::IsEnabledTexturing()
		{
			return _TexturingEnabled;
		}

		void RenderEngine::End()
		{
			glVertexPointer(3, GL_FLOAT, 0, _VertCache);
			glColorPointer(4, GL_FLOAT, 0, _ColCache);
			glTexCoordPointer(2, GL_FLOAT, 0, _TexCache);

			_DrawIntervals.back().VertCountEnd = _RenderedVertCount;
			for(uint32_t i = 0; i < _DrawIntervals.size(); i++)
			{
				SetActiveTexture(_DrawIntervals[i].Texture);
				if(_DrawIntervals[i].Mode == Drawables::IDrawable::RenderMode::ThreeD)
				{
					Drawables::Model * model = static_cast<Drawables::Model *>(_DrawIntervals[i].InitialDrawable);
					glPushMatrix();
					model->ApplyTransformation();
				}
				glDrawArrays(GL_TRIANGLES, _DrawIntervals[i].VertCountStart, _DrawIntervals[i].VertCountEnd - _DrawIntervals[i].VertCountStart);
				if(_DrawIntervals[i].Mode == Drawables::IDrawable::RenderMode::ThreeD)
				{
					glPopMatrix();
				}
			}
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

		void RenderEngine::SetActiveTexture(Content::TextureData * activeTexture)
		{
			if(_ActiveTexture == activeTexture)return;

			if(activeTexture == nullptr)
			{
				_ActiveTexture = nullptr;
				glBindTexture(GL_TEXTURE_2D, 0);
				_TexturingEnabled = false;
				if(_TexturingEnabled)
				{
					_TexturingEnabled = false;
					glDisable(GL_TEXTURE_2D);
				}
				assert(glGetError() == GL_NO_ERROR);
				return;
			}

			if(activeTexture->GetOpenglTextureID() != -1)
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

				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, activeTexture->GetWidth(), activeTexture->GetHeight(),
					0, GL_RGBA, GL_UNSIGNED_BYTE, activeTexture->_PixelData);
			}
			if(!_TexturingEnabled)
			{
				_TexturingEnabled = true;
				glEnable(GL_TEXTURE_2D);
			}
			assert(glGetError() == GL_NO_ERROR);
		}

		Vector2I RenderEngine::GetSize()
		{
			return _CurrentWindow->GetWindowSize();
		}
	}
}