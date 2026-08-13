#pragma once
namespace berialdraw
{
	/** System time utilities for cross-platform timer operations
	Provides platform-independent time measurement functions used by timer managers */
	class SystemTime
	{
	public:
		/** Get current time in milliseconds
		Uses platform-specific high-resolution timers (GetTickCount64, mach_absolute_time, clock_gettime)
		@return Current time in milliseconds */
		static uint64_t get_time_ms();
	};
}
