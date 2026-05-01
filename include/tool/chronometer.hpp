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
		long long last_method_call_ns;
		long long accumulated_method_ns;

		/** Format nanoseconds as string "S.MMM,UUUs" */
		String format_time_ns(int64_t ns) const;

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
		@return String with formatted time (e.g. "1.234 567s") */
		String elapsed() const;

		/** Get elapsed time with delta since last call (excludes method processing time)
		@return String with format "Total: X.XXX XXXs | Delta: X.XXX XXXs" */
		String elapsed_with_delta();

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


#if defined(_DEBUG)
	#define __ Chronometer chrono;
	#define _  bd_printf("%s:%d: %s\n",__FILE__,__LINE__, chrono.elapsed_with_delta().c_str());
#else
	#define __ 
	#define _
#endif


#ifdef __cplusplus
}
#endif
