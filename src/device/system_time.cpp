#include "berialdraw_imp.hpp"
#include "device/system_time.hpp"

#ifdef WIN32
	#include <windows.h>
#elif __APPLE__
	#include <mach/mach_time.h>
#else
	#include <time.h>
#endif

using namespace berialdraw;

// Get current time in milliseconds - platform specific implementation
uint64_t SystemTime::get_time_ms()
{
#ifdef WIN32
	return (uint64_t)GetTickCount64();
#elif __APPLE__
	// macOS/Cocoa: use mach_absolute_time
	static mach_timebase_info_data_t timebase = {0, 0};
	if (timebase.denom == 0)
	{
		mach_timebase_info(&timebase);
	}
	uint64_t mach_time = mach_absolute_time();
	uint64_t nanos = mach_time * timebase.numer / timebase.denom;
	return nanos / 1000000;  // Convert nanoseconds to milliseconds
#else
	// Linux/Unix: use clock_gettime with CLOCK_MONOTONIC
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
}
