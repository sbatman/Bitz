#include "../Common.h"
#include "GameStateService.h"
#include "GameState.h"

namespace Bitz
{
	namespace GameLogic
	{
		std::vector<GameState_Ptr> GameStateService::_ActiveGameStates;

		void GameStateService::StaticDispose()
		{
			_ActiveGameStates.clear();
		}

		void GameStateService::Update()
		{
			for (GameState_Ptr s : _ActiveGameStates) s->Update();

			for (GameState_Ptr s : _ActiveGameStates)
			{
				if (s->IsComplete())
				{
					GameState_Ptr nextState = s->GetNextGameState();
					EndState(s);
					StartState(nextState, s);
				}
			}
			_ActiveGameStates.erase(std::remove_if(_ActiveGameStates.begin(), _ActiveGameStates.end(), [](GameState_Ptr s) {return s->IsComplete(); }), _ActiveGameStates.end());
		}

		void GameStateService::Draw()
		{
			for (GameState_Ptr s : _ActiveGameStates) s->Draw();
		}

		bool GameStateService::IsGameStateActive(GameState_Ptr state)
		{
			for (GameState_Ptr s : _ActiveGameStates) if (s == state) return true;

			return false;
		}

		void GameStateService::StartState(GameState_Ptr state, GameState_Ptr preceedingState)
		{
			assert(state != nullptr);
			_ActiveGameStates.push_back(state);
			state->Enter(preceedingState);
		}

		void GameStateService::EndState(GameState_Ptr state)
		{
			assert(state != nullptr);
			_ActiveGameStates.erase(std::remove(_ActiveGameStates.begin(), _ActiveGameStates.end(), state), _ActiveGameStates.end());
			state->Exit();
		}
	}
}