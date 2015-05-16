#include "../Common.h"
#include "Timer.h"
#include "Consts.h"

namespace Bitz
{
	namespace Time
	{
		Timer::Timer()
		{
			_Running = false;
			_Start = 0;
			_Stop = 0;
			_Frequency = SystemFrequency();
		}

		Timer::~Timer()
		{
			_Running = false;
			_Start = 0;
			_Stop = 0;
			_Frequency = 0;
		}

		void Timer::Start()
		{
			_Start = SystemCurrentTicks();
			_Running = true;
		}

		void Timer::Stop()
		{
			_Stop = SystemCurrentTicks();
			_Running = false;
		}

		void Timer::Restart()
		{
			Stop();
			Start();
		}

		double_t Timer::GetElapsedMS() const
		{
			return  (GetElapsedTicks() / _Frequency) * MSINSEC;
		}

		double_t Timer::GetElapsedSeconds() const
		{
			return  (GetElapsedTicks() / _Frequency);
		}

		int64_t Timer::GetElapsedTicks() const
		{
			return _Running ? (SystemCurrentTicks() - _Start) : (_Stop - _Start);
		}

		bool Timer::IsRunning() const
		{
			return _Running;
		}
	}
}