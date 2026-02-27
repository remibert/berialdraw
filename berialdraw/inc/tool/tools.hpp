#pragma once
namespace berialdraw
{
	/** Converts the initial part of the string in stringptr to a long integer value according to the given base, which must be between 2, 8, 10, 16.
	@param stringptr String to be converted
	@param endptr Character that stops the convertion
	@param base Base of return value
	@return returns the value converted */
	int64_t strtolong(const char* stringptr, char** endptr, int base);

	/** Fast square root. The result max of square root must be lower than 4096.
	
	Compute the integer square root (floor) of `value` using a fast iterative
	method (Newton-Raphson-like). The implementation caches the previous input
	and result for small repeated-call optimizations.
	@param value Input value.
	@return Integer square root (floor of sqrt(value)). */
	uint64_t sqr(uint64_t value);

	/** Create a FreeType `FT_Matrix` representing a rotation for the given angle.
	
	The `angle` parameter uses FreeType's fixed-angle units (16.16 fixed-point
	representation, with constants such as `FT_ANGLE_PI`, `FT_ANGLE_PI2`, and
	`FT_ANGLE_2PI`). The returned matrix is suitable for FreeType transforms
	and uses 1<<16 scaling (e.g. 65536 == 1.0).
	Special-cases exist for exact multiples of 90 degrees.
	@param angle Rotation angle in FreeType fixed-point angle units.
	@return FT_Matrix describing the rotation. */
	FT_Matrix vector_matrix(int32_t angle);

	/** Seed the module-local pseudo-random number generator.
	@param new_seed New 32-bit seed value. */
	void set_rand(uint32_t new_seed);

	/** Generate a pseudo-random integer in the interval [mini, maxi).
	
	The implementation uses a simple linear congruential generator (LCG).
	@param mini Inclusive lower bound.
	@param maxi Exclusive upper bound.
	@return Pseudo-random integer between mini (inclusive) and maxi (exclusive). */
	int32_t get_rand(int32_t mini, int32_t maxi);

	/** Generate a crc on 32 bits buffer
	@param buffer pointer to the buffer
	@param size size of buffer
	@return crc generated */
	uint32_t compute_crc32(const void * buffer, uint32_t size);

	/** Generate a crc on 32 bits buffer
	@param buffer pointer to the buffer
	@param size size of buffer
	@param initial initial crc value
	@return crc generated */
	uint32_t compute_crc32(const void * buffer, uint32_t size, uint32_t & initial);

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

		/** Reset the chronometer (same as start) */
		void reset();
	};
}

#ifdef WIN32
	void usleep(int64_t usec);
#else
	#include <unistd.h>
#endif

#ifdef _DEBUG
void tools_test1();
void tools_test2();
#endif

	long long clockns();

