#pragma once
#include "../Time/Timer.h"
#include "../Interfaces.h"

namespace Bitz
{
	namespace GameLogic
	{
		class GameStateService;

		class GameState : public Interfaces::INameable, public Interfaces::IUpdateable
		{
			friend GameStateService;
		public:
			GameState();
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