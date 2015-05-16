#include "../Common.h"
#include "Debug\Logging.h"
#include "GameState.h"

namespace Bitz
{
	namespace GameLogic
	{
		GameState::GameState()
		{
		}

		GameState::~GameState()
		{
		}

		Time::Timer * GameState::GetTimeSinceEnter() const
		{
			return _EntryTimer;
		}

		bool GameState::IsCurrent() const
		{
			return _Current;
		}

		void GameState::Enter(GameState * preceedingState)
		{
			_EntryTimer = new Time::Timer();
			_EntryTimer->Start();
			_Current = true;
			Debug::Logging::Log(Debug::Logging::ErrorType::Notice, fmt::format(L"Entering GameState {0}", _Name));
			OnEnter(preceedingState);
		}

		void GameState::Exit()
		{
			if (_EntryTimer != nullptr)
			{
				delete _EntryTimer;
				_EntryTimer = nullptr;
			}
			_Current = false;
			Debug::Logging::Log(Debug::Logging::ErrorType::Notice, fmt::format(L"Exiting GameState {0}", _Name));
			OnExit();
		}

		void GameState::InternalUpdate()
		{
			OnUpdate();
		}

		void GameState::Draw()
		{
			OnDraw();
		}

		bool GameState::IsComplete() const
		{
			return _NextGameState != nullptr;
		}

		GameState * GameState::GetNextGameState() const
		{
			return _NextGameState;
		}
	}

}