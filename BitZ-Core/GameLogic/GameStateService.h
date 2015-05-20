#pragma once
#include "../Common.h"
#include "GameState.h"

namespace Bitz
{
	class Core;

	namespace GameLogic
	{
		class GameStateService
		{
			friend Core;
		public:
			static void StartState(GameState * state, GameState * preceedingState = nullptr);
			static void EndState(GameState * state);
			static bool IsGameStateActive(GameState * state);
		private:
			static void StaticDispose();
			static void Update();
			static void Draw();
			static std::vector<GameState *> _ActiveGameStates;
		};
	}
}
