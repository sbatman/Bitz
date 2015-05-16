#include "../BitZ-Core/Time/Timer.h"
#include <Windows.h>

namespace Bitz
{
	namespace Time
	{
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
	}
}