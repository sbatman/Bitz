#include "../Common.h"
#include "Debug\Logging.h"
#include "GameState.h"

namespace Bitz
{
	namespace GameLogic
	{
		GameState::GameState(std::string name)
		{
			assert(name.length() > 0 && "Not providing a name for a GameState is bad practice");
			_Name = name;
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

		void GameState::Enter(const GameState_Ptr preceedingState)
		{
			_EntryTimer = new Time::Timer();
			_EntryTimer->Start();
			_Current = true;
			Debug::Logging::Log(Debug::Logging::ErrorType::Notice, fmt::format("Entering GameState {0}", _Name));
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
			Debug::Logging::Log(Debug::Logging::ErrorType::Notice, fmt::format("Exiting GameState {0}", _Name));
			OnExit();
		}

		void GameState::InternalUpdate(double ms)
		{
			OnUpdate(ms);
		}

		void GameState::Draw()
		{
			OnDraw();
		}

		bool GameState::IsComplete() const
		{
			return _NextGameState != nullptr;
		}

		GameState_Ptr GameState::GetNextGameState() const
		{
			return _NextGameState;
		}
	}
}