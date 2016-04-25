#include "Core.h"
#include "Common.h"
#include "Time\Timer.h"
#include "Time\Consts.h"
#include "Debug\Logging.h"
#include "Input\Keyboard.h"
#include "Graphics\GraphicsManager.h"
#include "GameLogic\GameStateService.h"
#include "Content\IO.h"

namespace Bitz
{
	bool Core::_Running;
	GameLogic::GameCore * Core::_CurrentGameCore = nullptr;

	Time::Timer Core::_RunningTimer = Time::Timer();

	double_t Core::_TargetFPS = 60;
	double_t Core::_TargetUPS = 60;

	double_t Core::_MSPerDraw = Time::MSINSEC / _TargetFPS;
	double_t Core::_MSPerUpdate = Time::MSINSEC / _TargetUPS;

	double_t Core::_LastDraw = 0;
	double_t  Core::_LastUpdate = 0;

	bool Core::_PauseRender = false;

	std::mutex Core::_InternalLock;

	void Core::Init()
	{
		if (!Debug::Logging::HasInit())Debug::Logging::Init();
		_LastDraw = 0;
		_LastUpdate = 0;
		_RunningTimer = Time::Timer();
	}

	void Core::Run(GameLogic::GameCore * game)
	{
		_InternalLock.lock();
		assert(!_Running && "Core cannot be run as it is already running");
		_Running = true;
		_CurrentGameCore = game;

		try {
			if (_CurrentGameCore->LoadContent() && _CurrentGameCore->Init())
			{
				_RunningTimer.Start();
				while (_Running)
				{
					_InternalLock.unlock();
					_InternalLock.lock();
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
						GameLogic::GameStateService::Update(_MSPerUpdate);
						if (!_CurrentGameCore->Update())
						{
							_Running = false;
							_InternalLock.unlock();
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
								_InternalLock.unlock();
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
		catch (std::exception e)
		{
			throw e;
		}
		_CurrentGameCore = nullptr;
		_InternalLock.unlock();
	}

	bool Core::IsRunning()
	{
		return _Running;
	}

	void Core::SetTargetUPS(const double_t newUPS)
	{
		assert(newUPS >= 1 && newUPS <= 1000 && "UPS can only be set to a value between 1 and 1000 inclusive");
		_TargetUPS = newUPS;
		_MSPerUpdate = Time::MSINSEC / _TargetUPS;
	}

	void Core::SetTargetFPS(const double_t newFPS)
	{
		assert(newFPS >= 1 && newFPS <= 1000 && "FPS can only be set to a value between 1 and 1000 inclusive");
		_TargetFPS = newFPS;
		_MSPerDraw = Time::MSINSEC / _TargetFPS;
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

	void Core::SetRenderPause(bool state)
	{
		_PauseRender = state;
	}

	bool Core::IsRenderPaused()
	{
		return _PauseRender;
	}

	void Core::Lock()
	{
		_InternalLock.lock();
	}

	void Core::Unlock()
	{
		_InternalLock.unlock();
	}
}