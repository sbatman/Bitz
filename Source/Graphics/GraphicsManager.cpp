#include "../Common.h"
#include "GraphicsManager.h"

namespace Bitz
{
	using namespace std;
	using namespace Math;
	using namespace Time;

	namespace GFX
	{
		bool GraphicsManager::_HasInit = false;
		bool GraphicsManager::_InRenderScope = false;
		bool GraphicsManager::_Debug_EnableBackbufferColourAlt = false;

		Vector3F GraphicsManager::_Debug_BackbufferColourAlt = Vector3F(0.5f, 0.3f, 0.5f);
		Vector3F GraphicsManager::_BufferClearColour = Vector3F(0);
		Timer * GraphicsManager::_FrameTimer = nullptr;
		uint64_t GraphicsManager::_FrameNumber = 0;
		double_t GraphicsManager::_LastFrameTime = 0;
		GraphicsStates::BaseGS * GraphicsManager::_CurrentGraphicsState = nullptr;
		RenderEngine * GraphicsManager::_ActiveRenderEngine = nullptr;
		Window * GraphicsManager::_ActiveWindow = nullptr;

		Camera * GraphicsManager::_LastActiveCamera = nullptr;

		void GraphicsManager::Init(Window * window)
		{
#ifdef WIN32
			SetActiveWindow(window);
#endif
			_HasInit = true;
			if (_FrameTimer == nullptr)_FrameTimer = new Timer();
		}

		void GraphicsManager::SetActiveWindow(Window * window)
		{
			if (window == _ActiveWindow)return;
			if (_ActiveWindow != nullptr)
			{
				if (_ActiveRenderEngine != nullptr)
				{
					delete _ActiveRenderEngine;
					_ActiveRenderEngine = nullptr;
				}
				delete window;
				window = nullptr;
			}

			_ActiveWindow = window;

			if (_ActiveWindow != nullptr)
			{
				_ActiveRenderEngine = new RenderEngine(window);

				_ActiveRenderEngine->Init();
			}
		}

		void GraphicsManager::Update()
		{
			assert(_HasInit);
			if (_ActiveRenderEngine != nullptr)_ActiveRenderEngine->Update();
		}

		void GraphicsManager::PreRender()
		{
			assert(_HasInit);
			if (_ActiveRenderEngine == nullptr) return;
			_FrameTimer->Start();

			Clear();
		}

		void GraphicsManager::PostRender()
		{
			assert(_HasInit);
			assert(!_InRenderScope&& "End render must be called for every begin render");

			if (_ActiveRenderEngine == nullptr) return;

			glFinish();
			_ActiveRenderEngine->Present();
			_FrameTimer->Stop();
			_LastFrameTime = _FrameTimer->GetElapsedMS();
			_FrameNumber++;
		}

		void GraphicsManager::BeginRender(GraphicsStates::BaseGS * graphicsState)
		{
			assert(_HasInit);

			if (_ActiveRenderEngine == nullptr) return;

			assert(graphicsState != nullptr&& "Begin Render must be called with a graphics state");
			assert(graphicsState->GetActiveCamera() != nullptr&& "The current camera must be set before attempting to draw");
			assert(!_InRenderScope&& "Begin render has already been called");

			_CurrentGraphicsState = graphicsState;

			_ActiveRenderEngine->Begin(graphicsState->GetStockShader());

			_InRenderScope = true;

			_CurrentGraphicsState->EnterState();

			Camera * camera = _CurrentGraphicsState->GetActiveCamera();

			if (_LastActiveCamera != camera)
			{
				camera->MakeActive();
				_LastActiveCamera = camera;
			}

			camera->Apply();
		}

		void GraphicsManager::EndRender()
		{
			assert(_HasInit);
			assert(_InRenderScope&& "Begin render has not been called");

			if (_ActiveRenderEngine == nullptr) return;

			_ActiveRenderEngine->End();

			_CurrentGraphicsState->ExitState();
			_CurrentGraphicsState = nullptr;
			_InRenderScope = false;
		}

		void GraphicsManager::Clear()
		{
			if (_ActiveRenderEngine == nullptr) return;
			_ActiveRenderEngine->Clear((!_Debug_EnableBackbufferColourAlt || _FrameNumber % 2) ? _BufferClearColour : _Debug_BackbufferColourAlt);
		}

		double_t GraphicsManager::GetLastFrameTimeMS()
		{
			assert(_HasInit);
			return _LastFrameTime;
		}

		void GraphicsManager::Render(std::shared_ptr<Drawables::IDrawable> idrawable)
		{
			assert(_HasInit);
			assert(_InRenderScope&& "Begin render has not been called");

			if (_ActiveRenderEngine == nullptr) return;

			_ActiveRenderEngine->Render(idrawable.get());
		}

		Camera* GraphicsManager::GetCurrentCamera()
		{
			if (_CurrentGraphicsState == nullptr) return nullptr;
			return _CurrentGraphicsState->GetActiveCamera();
		}

		uint64_t GraphicsManager::GetFrameCount()
		{
			return _FrameNumber;
		}

		Vector2I GraphicsManager::GetScreenSize()
		{
			return _ActiveRenderEngine != nullptr ? _ActiveRenderEngine->GetSize() : Vector2I(0);
		}

		void GraphicsManager::StaticDispose()
		{
			assert(_HasInit);
			if (_FrameTimer != nullptr)
			{
				delete _FrameTimer;
				_FrameTimer = nullptr;
			}
			if (_ActiveRenderEngine != nullptr)
			{
				delete _ActiveRenderEngine;
				_ActiveRenderEngine = nullptr;
			}

			_ActiveWindow = nullptr;

			if (_CurrentGraphicsState != nullptr)
			{
				_CurrentGraphicsState->ExitState();
				delete _CurrentGraphicsState;
			}

			_LastActiveCamera = nullptr;
			_HasInit = false;
		}

		void GraphicsManager::SetBufferClearColour(const Vector3F colour)
		{
			assert(_HasInit);
			_BufferClearColour = colour;
		}

		void GraphicsManager::SetScreenSize(Vector2I newSize)
		{
			if (_ActiveRenderEngine != nullptr)_ActiveRenderEngine->SetSize(newSize);
			if (_LastActiveCamera != nullptr)
			{
				_LastActiveCamera->_ForceMakeActiveOnApply = true;
			}
		}

		Vector3F GraphicsManager::GetBufferClearColour()
		{
			assert(_HasInit);
			return _BufferClearColour;
		}

		RenderEngine * GraphicsManager::GetActiveRenderEngine()
		{
			return _ActiveRenderEngine;
		}
	}
}