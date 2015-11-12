#pragma once
#include "../Common.h"
#include "GameState.h"

namespace Bitz
{
	class Core;

	namespace GameLogic
	{
		/// <summary>
		/// The GameStateSerive is responsible for managing the progression, swithing and exiting of game states
		/// for the whole project. Multiple states can be started and ran in parallel
		/// </summary>
		class GameStateService
		{
			friend Core;
		public:
			static void StartState(GameState_Ptr state, GameState_Ptr preceedingState = nullptr);
			static void EndState(GameState_Ptr state);
			static bool IsGameStateActive(GameState_Ptr state);
		private:
			static void StaticDispose();
			static void Update();
			static void Draw();
			static std::vector<GameState_Ptr> _ActiveGameStates;
		};
	}
}
