#pragma once
#include "../Time/Timer.h"
#include "../Interfaces/Core.h"

namespace Bitz
{
	namespace GameLogic
	{
		class GameStateService;

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
			GameState * GetNextGameState() const;

		protected:
			virtual bool IsComplete() const;
			virtual void OnEnter(GameState * preceedingState) = 0;
			virtual void OnExit() = 0;
			virtual void OnUpdate() = 0;
			virtual void OnDraw() = 0;

		private:
			Time::Timer * _EntryTimer;
			bool _Current;
			GameState * _NextGameState = nullptr;

			void Enter(GameState * preceedingState);
			void Exit();
			void InternalUpdate() override;
			void Draw();
		};
	}
}