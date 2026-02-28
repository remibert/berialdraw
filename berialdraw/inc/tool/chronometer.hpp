#pragma once

namespace berialdraw
{
	class String;

	/** Chronometer class to measure elapsed time.
	
	Usage:
		Chronometer chrono;
		chrono.start();
		// ... some work ...
		int64_t elapsed_ms = chrono.elapsed_ms();
	*/
	class Chronometer
	{
	private:
		long long start_time_ns;

	public:
		/** Constructor - initializes the chronometer but does not start it */
		Chronometer();

		/** Start or restart the chronometer */
		void start();

		/** Get the elapsed time in milliseconds since start() was called
		@return Elapsed time in milliseconds */
		int64_t elapsed_ms() const;

		/** Get the elapsed time in microseconds since start() was called
		@return Elapsed time in microseconds */
		int64_t elapsed_us() const;

		/** Get the elapsed time in nanoseconds since start() was called
		@return Elapsed time in nanoseconds */
		int64_t elapsed_ns() const;

		/** Get the elapsed time as a formatted String
		@return String with formatted time (e.g. "1h 2m 3s 45ms 678us 912ns") */
		String elapsed() const;

		/** Print the elapsed time */
		void print(const String & name = "time") const;

		/** Print the elapsed time with newline */
		void println(const String & name = "time") const;
	};
}

#ifndef WIN32
	#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

void sleep_us(int64_t usec);
long long clock_ns();

#ifdef __cplusplus
}
#endif
