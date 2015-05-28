#include "Core.h"
#include "Common.h"
#include "Time\Timer.h"
#include "Debug\Logging.h"
#include "Input\Keyboard.h"
#include "Input\Mouse.h"
#include "Graphics\GraphicsManager.h"
#include "GameLogic\GameStateService.h"
#include "Content\IO.h"

namespace Bitz
{
	bool Core::_Running;
	GameLogic::GameCore * Core::_CurrentGameCore;

	Time::Timer Core::_RunningTimer;

	double_t Core::_TargetFPS = 60;
	double_t Core::_TargetUPS = 120;

	double_t Core::_MSPerDraw = 1000 / _TargetFPS;
	double_t Core::_MSPerUpdate = 1000 / _TargetUPS;

	double_t Core::_LastDraw;
	double_t  Core::_LastUpdate = 0;

	bool Core::_PauseRender = false;

	void Core::Init()
	{
		if (!Debug::Logging::HasInit())Debug::Logging::Init();
		_LastDraw = 0;
		_LastUpdate = 0;
		_RunningTimer = Time::Timer();
	}

	void Core::Run(GameLogic::GameCore * game)
	{
		assert(!_Running && "Core cannot be run as it is already running");
		_Running = true;
		_CurrentGameCore = game;

		if (_CurrentGameCore->LoadContent() && _CurrentGameCore->Init())
		{
			_RunningTimer.Start();
			while (_Running)
			{
				Input::Keyboard::Update();
				GFX::GraphicsManager::Update();
				double_t elapsedMS = _RunningTimer.GetElapsedMS();

				if (_LastDraw > elapsedMS)
				{
					_LastDraw = elapsedMS;
				}
				if (_LastUpdate > elapsedMS)
				{
					_LastUpdate = elapsedMS;
				}

				if ((elapsedMS - _LastUpdate) > _MSPerUpdate)
				{
					_LastUpdate += _MSPerUpdate;
					GameLogic::GameStateService::Update();
					if (!_CurrentGameCore->Update())
					{
						_Running = false;
						break;
					}
				}
				else if ((elapsedMS - _LastDraw) > _MSPerDraw) //The else is to ensure the update always gets preference, only doing the draw if the update wasnt required
				{
					_LastDraw += _MSPerDraw;
					if (!_PauseRender)
					{
						GFX::GraphicsManager::PreRender();
						GameLogic::GameStateService::Draw();
						if (!_CurrentGameCore->Draw())
						{
							_Running = false;
							break;
						}
						GFX::GraphicsManager::PostRender();
						Debug::Logging::Log(Debug::Logging::ErrorType::Notice, fmt::format("Draw Took {0}", GFX::GraphicsManager::GetLastFrameTimeMS()));
					}
				}
				else
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(0));
				}
			}
		}

		bool unloadSuccessful = _CurrentGameCore->UnloadContent();
		assert(unloadSuccessful && "Unload Content Failed");
		bool exitSuccessful = _CurrentGameCore->OnExit();
		assert(exitSuccessful && "Exit Failed");

		GFX::GraphicsManager::StaticDispose();
		Content::IO::CloseAllOpen();
		Debug::Logging::StaticDispose();
	}

	bool Core::IsRunning()
	{
		return _Running;
	}

	void Core::SetTargetUPS(double_t newUPS)
	{
		assert(newUPS >= 1 && newUPS <= 1000 && "UPS can only be set to a value between 1 and 1000 inclusive");
		_TargetUPS = newUPS;
		_MSPerUpdate = 1000.0f / _TargetUPS;
	}

	void Core::SetTargetFPS(double_t newFPS)
	{
		assert(newFPS >= 1 && newFPS <= 1000 && "FPS can only be set to a value between 1 and 1000 inclusive");
		_TargetFPS = newFPS;
		_MSPerDraw = 1000.0f / _TargetFPS;
	}

	double_t Core::GetTargetUPS()
	{
		return _TargetUPS;
	}

	double_t Core::GetTargetFPS()
	{
		return _TargetFPS;
	}

	void Core::Stop()
	{
		_Running = false;
	}
}