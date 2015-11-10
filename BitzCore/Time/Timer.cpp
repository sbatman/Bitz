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

#ifdef WIN32
		int64_t Timer::SystemCurrentTicks() const
		{
			LARGE_INTEGER now;
			QueryPerformanceCounter(&now);
			return now.QuadPart;
		}

		double_t Timer::SystemFrequency() const
		{
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			return  static_cast<double_t>(frequency.QuadPart);
		}
#endif

#ifdef __ANDROID__
		int64_t Timer::SystemCurrentTicks() const
		{
			struct timespec now;
			clock_gettime(CLOCK_MONOTONIC, &now);
			return now.tv_sec * 1000000000LL + now.tv_nsec;
		}

		double_t Timer::SystemFrequency() const
		{
			return  1000000000;
		}
#endif
	}
}