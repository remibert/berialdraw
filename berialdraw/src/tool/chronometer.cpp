#include "berialdraw_imp.hpp"

using namespace berialdraw;

extern "C" void sleep_us(int64_t usec)
{
#ifdef WIN32
	HANDLE timer; 
	LARGE_INTEGER ft; 

	ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	if (timer)
	{
		SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
		WaitForSingleObject(timer, INFINITE); 
		CloseHandle(timer); 
	}
#endif

#ifdef __APPLE__
	struct timespec ts;
	ts.tv_sec = usec / 1000000;
	ts.tv_nsec = (usec % 1000000) * 1000;
	nanosleep(&ts, NULL);
#endif
}

extern "C" long long clock_ns()
{
#ifdef WIN32
	LARGE_INTEGER freq , count ;

	QueryPerformanceFrequency(&freq);
	if(QueryPerformanceCounter(&count) && freq.QuadPart != 0)
	{
		return (long long)((count.QuadPart)* (1000000000/freq.QuadPart)) ;
	}
	else 
	{
		return 0;
	}
#endif

#ifdef LINUX
	struct timespec elapsed_time;
	clock_gettime(CLOCK_MONOTONIC, &elapsed_time);
	return (elapsed_time.tv_sec) * 1000000000LL + (elapsed_time.tv_nsec);
#endif

#ifdef __APPLE__
	static mach_timebase_info_data_t timebase_info = {0, 0};
	
	// Initialize timebase info on first call
	if (timebase_info.denom == 0)
	{
		mach_timebase_info(&timebase_info);
	}
	
	uint64_t mach_time = mach_absolute_time();

	// Convert from Mach absolute time to nanoseconds
	return (mach_time * timebase_info.numer) / timebase_info.denom;
#endif
}

// Chronometer class implementation
Chronometer::Chronometer()
{
	start_time_ns = clock_ns();
	last_method_call_ns = 0;
	accumulated_method_ns = 0;
}

void Chronometer::start()
{
	start_time_ns = clock_ns();
}

int64_t Chronometer::elapsed_ms() const
{
	if (start_time_ns == 0)
		return 0;
	long long current_time_ns = clock_ns();
	return (current_time_ns - start_time_ns) / 1000000;
}

int64_t Chronometer::elapsed_us() const
{
	if (start_time_ns == 0)
		return 0;
	long long current_time_ns = clock_ns();
	return (current_time_ns - start_time_ns) / 1000;
}

int64_t Chronometer::elapsed_ns() const
{
	if (start_time_ns == 0)
	{
		return 0;
	}
	return clock_ns() - start_time_ns;
}

/** Format nanoseconds as string "S.MMM,UUUs" */
String Chronometer::format_time_ns(int64_t ns) const
{
	String result;
	int64_t s = ns / 1000000000LL;
	int64_t ms = (ns / 1000000) % 1000;
	int64_t us = (ns / 1000) % 1000;
	result.write_format("%lld.%03lld,%03llds", s, ms, us);
	return result;
}

/** Get the elapsed time as a formatted String */
String Chronometer::elapsed() const
{
	int64_t ns = elapsed_ns();
	return format_time_ns(ns);
}

/** Get elapsed time with delta since last call (excludes method processing time) */
String Chronometer::elapsed_with_delta()
{
	long long method_start = clock_ns();
	
	// Calculate total elapsed time minus accumulated method processing time
	long long total_ns = elapsed_ns() - accumulated_method_ns;
	
	// Calculate delta since last call
	long long delta_ns;
	if (last_method_call_ns == 0)
	{
		// First call: delta equals total
		delta_ns = total_ns;
	}
	else
	{
		// Time between last call and now
		delta_ns = method_start - last_method_call_ns;
	}
	
	// Format both times using the dedicated formatting method
	String result;
	result.print("T=%s D=%s", (const char*)format_time_ns(total_ns), (const char*)format_time_ns(delta_ns));
	
	// Record method end time and accumulate processing time
	long long method_end = clock_ns();
	accumulated_method_ns += (method_end - method_start);
	
	// Save end time for next delta calculation
	last_method_call_ns = method_end;
	
	return result;
}

/** Print the elapsed time */
void Chronometer::print(const String & name) const
{
	bd_printf("%s:%s", name.c_str(), elapsed().c_str());
}

/** Print the elapsed time with newline */
void Chronometer::println(const String & name) const
{
	print(name);
	bd_printf("\n");
}
