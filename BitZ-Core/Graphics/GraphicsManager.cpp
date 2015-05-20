#include "../Common.h"
#include "GraphicsManager.h"
#include "Window.h"
#include "Camera.h"
#include "GraphicsStates\BaseGS.h"
#include "../Time/Timer.h"

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
		Timer * GraphicsManager::_FrameTimer = new Timer();
		uint64_t GraphicsManager::_FrameNumber = 0;
		double_t GraphicsManager::_LastFrameTime = 0;
		GraphicsStates::BaseGS * GraphicsManager::_CurrentGraphicsState = nullptr;
		RenderEngine * GraphicsManager::_ActiveRenderEngine = nullptr;

		Camera * GraphicsManager::_LastActiveCamera = nullptr;

		Vector2I GraphicsManager::_CurrentScreenSize = Vector2I(512);

		void GraphicsManager::Init(Window * window)
		{
			assert(!_HasInit);

			_ActiveRenderEngine = new RenderEngine(window);

			_ActiveRenderEngine->Init();

			_HasInit = true;
		}

		void GraphicsManager::Update()
		{
			assert(_HasInit);
			_ActiveRenderEngine->Update();
		}

		void GraphicsManager::PreRender()
		{
			assert(_HasInit);

			_FrameTimer->Start();

			Clear();
		}

		void GraphicsManager::PostRender()
		{
			assert(_HasInit);
			assert(!_InRenderScope&& "End render must be called for every begin render");
			glFinish();
			_ActiveRenderEngine->Present();
			_FrameTimer->Stop();
			_LastFrameTime = _FrameTimer->GetElapsedMS();
			_FrameNumber++;
		}

		void GraphicsManager::BeginRender(GraphicsStates::BaseGS * graphicsState)
		{
			assert(_HasInit);

			_CurrentGraphicsState = graphicsState;

			_ActiveRenderEngine->Begin();

			assert(graphicsState != nullptr&& "Begin Render must be called with a graphics state");
			assert(_CurrentGraphicsState->GetActiveCamera() != nullptr&& "The current camera must be set before attempting to draw");
			assert(!_InRenderScope&& "Begin render has already been called");

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

			if (_CurrentGraphicsState->IsNormalsEnabled())
			{
				_ActiveRenderEngine->EnableNormals(true);
			}

			_ActiveRenderEngine->End();

			if (_CurrentGraphicsState->IsNormalsEnabled())
			{
				_ActiveRenderEngine->EnableNormals(false);
			}

			_CurrentGraphicsState->ExitState();
			_CurrentGraphicsState = nullptr;
			_InRenderScope = false;
		}

		void GraphicsManager::Clear()
		{
			_ActiveRenderEngine->Clear((!_Debug_EnableBackbufferColourAlt || _FrameNumber % 2) ? _BufferClearColour : _Debug_BackbufferColourAlt);
		}

		double_t GraphicsManager::GetLastFrameTimeMS()
		{
			assert(_HasInit);
			return _LastFrameTime;
		}

		void GraphicsManager::Render(Drawables::IDrawable* idrawable)
		{
			assert(_HasInit);
			assert(_InRenderScope&& "Begin render has not been called");
			_ActiveRenderEngine->Render(idrawable);
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
			return _CurrentScreenSize;
		}

		void GraphicsManager::StaticDispose()
		{
			assert(_HasInit);
			delete _FrameTimer;
			delete _ActiveRenderEngine;

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
			_CurrentScreenSize = newSize;
			_ActiveRenderEngine->SetSize(newSize);
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
	}
}