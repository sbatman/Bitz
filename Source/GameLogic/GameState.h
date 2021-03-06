#pragma once
#include "../Time/Timer.h"
#include "../Interfaces/Core.h"

namespace Bitz
{
	namespace GameLogic
	{
		class GameStateService;
		class GameState;

		typedef std::shared_ptr<GameState> GameState_Ptr;

		/// <summary>
		/// The GameState class acts as a backbone of the games logic paths, each of the possible states
		/// the game could be in should have its logic compartmentalized into a derived copy of this class
		/// Optimal use of this structure will offer an objectorientation of not just classes and objects
		/// within the code, but an oject orientation of the logic pathways themselves.
		/// </summary>
		class GameState : public Interfaces::INameable, public Interfaces::IGameObject
		{
			friend GameStateService;
		public:
			GameState(std::string name);
			~GameState() = default;

			Time::Timer * GetTimeSinceEnter() const;
			bool IsCurrent() const;
			GameState_Ptr GetNextGameState() const;

		protected:
			virtual bool IsComplete() const;
			virtual void OnEnter(const GameState_Ptr& preceedingState) = 0;
			virtual void OnExit() = 0;
			void OnUpdate(double ms) override = 0;
			void OnDraw() override = 0;

		private:
			Time::Timer * _EntryTimer;
			bool _Current;
			GameState_Ptr _NextGameState = nullptr;

			void Enter(const GameState_Ptr& preceedingState);
			void Exit();
		};
	}
}