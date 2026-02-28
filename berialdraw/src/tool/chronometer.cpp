#include "berialdraw_imp.hpp"

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

namespace berialdraw
{

// Chronometer class implementation
Chronometer::Chronometer()
{
	start_time_ns = clock_ns();
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
		return 0;
	return clock_ns() - start_time_ns;
}

/** Get the elapsed time as a formatted String */
String Chronometer::elapsed() const
{
	int64_t ns = elapsed_ns();
	String result;
	
	if (ns == 0)
	{
		result.write_format("0ns");
	}
	else
	{
		int64_t h = ns / 3600000000000LL;
		int64_t m = (ns / 60000000000LL) % 60;
		int64_t s = (ns / 1000000000LL) % 60;
		int64_t ms = (ns / 1000000) % 1000;
		int64_t us = (ns / 1000) % 1000;
		int64_t ns_rem = ns % 1000;
		
		if (h > 0) result.write_format("%lldh", h);
		if (m > 0) result.write_format(" %lldm", m);
		if (s > 0) result.write_format(" %llds", s);
		if (ms > 0) result.write_format(" %lldms", ms);
		if (us > 0) result.write_format(" %lldus", us);
		if (ns_rem > 0) result.write_format(" %lldns", ns_rem);
	}
	
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

} // namespace berialdraw
