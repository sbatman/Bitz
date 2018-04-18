#pragma once
#include "Common.h"
#include "GameLogic/GameCore.h"
#include "Time\Timer.h"

namespace Bitz
{
	namespace GameLogic
	{
		class GameCore;
	}
	/// <summary>
	/// The core class is the central control path of the engine, this triggers all required
	/// sub logic paths such as rendering or update logic. It is also responsible for triggering
	/// the engines services
	/// </summary>
	class Core
	{
		friend class GameLogic::GameCore;
	protected:
		/// <summary>
		/// Initialises the engine core with the provided window
		/// </summary>
		static void Init();
		/// <summary>
		/// Starts the GameCore running, taking control of the control path of this thread
		/// </summary>
		/// <param name='game'>The GameCore to initalise and run for this session</param>
		static void Run(GameLogic::GameCore * game);
		/// <summary>
		/// Returns whether the engine is running
		/// </summary>
		/// <returns>True if the engine is running else false</returns>
		static bool IsRunning();
		/// <summary>
		/// Sets the target update rate per second
		/// </summary>
		static void SetTargetUPS(const double_t newUPS);
		/// <summary>
		/// Sets the target frame draws per second
		/// </summary>
		static void SetTargetFPS(const double_t newFPS);

		/// <summary>
		/// Gets the current target update rate per second
		/// </summary>
		static double_t GetTargetUPS();
		/// <summary>
		/// Gets the current target frame draws per second
		/// </summary>
		static double_t GetTargetFPS();

	public:
		/// <summary>
		/// Pause or unpause the renderloop, this effectivly causes the engine to skip calling draw logic while paused
		/// This is mostly used for periods of lost focus
		/// </summary>
		/// <param name='state'>True to prevent further draw calls, else false</param>
		static void SetRenderPause(bool state);

		/// <summary>
		/// Returns whether the render loop is paused
		/// </summary>
		/// <returns>True if paused else false</returns>
		static bool IsRenderPaused();

		/// <summary>
		/// Stops the engine
		/// </summary>
		static void Stop();

		/// <summary>
		/// Locks the games internal mutex preventing it from performing a draw or update
		/// </summary>
		static void Lock();
		/// <summary>
		/// Unlocks the games internal mutex allowing it to continue normal operation after a lock
		/// </summary>
		static void Unlock();

	private:
		static bool _PauseRender;
		static double_t _LastUpdate;
		static double_t _LastDraw;
		static double_t _TargetFPS;
		static double_t _TargetUPS;
		static double_t _MSPerDraw;
		static double_t _MSPerUpdate;
		static bool _Running;
		static GameLogic::GameCore * _CurrentGameCore;
		static Time::Timer _RunningTimer;

		static void PlatformSpecificUpdate();

		static std::mutex _InternalLock;
	};
}