#ifndef tools_h_INCLUDED
#define tools_h_INCLUDED

namespace berialdraw
{
	/** Converts the initial part of the string in stringptr to a long integer value according to the given base, which must be between 2, 8, 10, 16.
	@param stringptr String to be converted
	@param endptr Character that stops the convertion
	@param base Base of return value
	@return returns the value converted */
	int64_t strtolong(const char* stringptr, char** endptr, int base);


	/** Fast square root. The result max of square root must be lower than 4096 */
	uint64_t sqr(uint64_t value);

	FT_Matrix vector_matrix(int32_t angle);
	void calc_point(Point & p, Dim radius, FT_Vector & sincos);
	void calc_cubic(Point & p, Dim radius, FT_Vector & sincos);
	void set_rand(uint32_t new_seed);
	int32_t get_rand(int32_t mini, int32_t maxi);

	Dim compute_zoom(Dim value, Dim zoom);

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

	bool match_pattern(const char *pattern, const char *string, bool ignore_case=false);
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

#endif // tools_h_INCLUDED

