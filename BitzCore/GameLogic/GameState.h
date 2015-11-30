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
		class GameState : public Interfaces::INameable, public Interfaces::IUpdateable
		{
			friend GameStateService;
		public:
			GameState(std::string name);
			~GameState();

			Time::Timer * GetTimeSinceEnter() const;
			bool IsCurrent() const;
			GameState_Ptr GetNextGameState() const;

		protected:
			virtual bool IsComplete() const;
			virtual void OnEnter(const GameState_Ptr preceedingState) = 0;
			virtual void OnExit() = 0;
			virtual void OnUpdate(double ms) = 0;
			virtual void OnDraw() = 0;

		private:
			Time::Timer * _EntryTimer;
			bool _Current;
			GameState_Ptr _NextGameState = nullptr;

			void Enter(GameState_Ptr preceedingState);
			void Exit();
			void InternalUpdate(double ms) override;
			void Draw();
		};
	}
}