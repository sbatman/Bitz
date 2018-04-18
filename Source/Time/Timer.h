#pragma once
#include "../Common.h"

namespace Bitz
{
	namespace Time
	{
		/// <summary>
		/// Used for tracking time within the engine in a uniform context
		/// </summary>
		class Timer
		{
		public:
			Timer();
			virtual ~Timer();

			/// <summary>
			/// Starts the timer from 0
			/// </summary>
			void Start();
			/// <summary>
			/// Stops the timer
			/// </summary>
			void Stop();
			/// <summary>
			/// Restarts the timer from 0
			/// </summary>
			void Restart();

			/// <summary>
			/// Returns the number of milliseconds since the timer started
			/// </summary>
			/// <returns>Time since timer start in ms</returns>
			double_t GetElapsedMS() const;
			/// <summary>
			/// Returns the number of seconds since the timer started
			/// </summary>
			/// <returns>Time since timer start in seconds</returns>
			double_t GetElapsedSeconds() const;
			/// <summary>
			/// Returns the number of ticks since the timer started
			/// </summary>
			/// <returns>Time since timer start in ticks</returns>
			int64_t GetElapsedTicks() const;
			/// <summary>
			/// Returns whether the timer is running
			/// </summary>
			/// <returns>True if the timer is running else false </returns>
			bool IsRunning() const;

		private:
			/// <summary>
			/// The point from which the timer started
			/// </summary>
			int64_t _Start;
			/// <summary>
			/// The point at which the timer stopped
			/// </summary>
			int64_t _Stop;
			/// <summary>
			/// Whether or not the timer is currently running
			/// </summary>
			bool _Running;
			/// <summary>
			/// The frequency used for underlying clock calculations
			/// </summary>
			double_t _Frequency;

			virtual int64_t SystemCurrentTicks() const;

			virtual double_t SystemFrequency() const;
		};
	}
}