#include "../Common.h"
#include "GameStateService.h"
#include "GameState.h"

namespace Bitz
{
	namespace GameLogic
	{
		std::vector<GameState *> GameStateService::_ActiveGameStates;

		void GameStateService::StaticDispose()
		{
			_ActiveGameStates.clear();
		}

		void GameStateService::Update()
		{
			for (GameState * s : _ActiveGameStates)
			{
				s->Update();
			}
			for (GameState * s : _ActiveGameStates)
			{
				if (s->IsComplete())
				{
					GameState * nextState = s->GetNextGameState();
					EndState(s);
					StartState(nextState, s);
				}
			}
			_ActiveGameStates.erase(std::remove_if(_ActiveGameStates.begin(), _ActiveGameStates.end(), [](GameState * s) {return s->IsComplete(); }), _ActiveGameStates.end());
		}

		void GameStateService::Draw()
		{
			for (GameState * s : _ActiveGameStates)
			{
				s->Draw();
			}
		}

		bool GameStateService::IsGameStateActive(GameState * state)
		{
			for (GameState * s : _ActiveGameStates)
			{
				if (s == state) return true;
			}
			return false;
		}

		void GameStateService::StartState(GameState * state, GameState * preceedingState)
		{
			assert(state != nullptr);
			_ActiveGameStates.push_back(state);
			state->Enter(preceedingState);
		}

		void GameStateService::EndState(GameState * state)
		{
			assert(state != nullptr);
			_ActiveGameStates.erase(std::remove(_ActiveGameStates.begin(), _ActiveGameStates.end(), state), _ActiveGameStates.end());
			state->Exit();
		}
	}
}