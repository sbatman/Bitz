#pragma once
#include "Common.h"
#include "GameLogic/GameCore.h"

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
		/// Stops the engine
		/// </summary>
		static void Stop();
		/// <summary>
		/// Sets the target update rate per second
		/// </summary>
		static void SetTargetUPS(double_t newUPS);
		/// <summary>
		/// Sets the target frame draws per second
		/// </summary>
		static void SetTargetFPS(double_t newFPS);

		/// <summary>
		/// Gets the current target update rate per second
		/// </summary>
		static double_t GetTargetUPS();
		/// <summary>
		/// Gets the current target frame draws per second
		/// </summary>
		static double_t GetTargetFPS();
	};
}