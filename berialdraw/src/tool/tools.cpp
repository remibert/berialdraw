#include "berialdraw_imp.hpp"

namespace berialdraw
{
static const unsigned long  crc32Poly[] = 
{
	0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
	0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
	0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,	0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
	0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,	0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
	0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
	0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,	0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
	0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
	0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
	0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
	0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
	0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
	0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
	0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
	0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
	0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
	0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
	0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
	0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
	0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
	0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
	0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
	0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
	0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
	0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
	0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
	0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
	0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
	0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
	0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
	0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
	0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
	0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

/** Generate a crc on 32 bits buffer
@param buffer pointer to the buffer
@param size size of buffer
@return crc generated */
uint32_t compute_crc32(const void * buffer, uint32_t size)
{
	uint32_t initial = 0;
	return compute_crc32(buffer, size, initial);
}


/** Generate a crc on 32 bits buffer
@param buffer pointer to the buffer
@param size size of buffer
@param initial initial crc value
@return crc generated */
uint32_t compute_crc32(const void * buffer, uint32_t size, uint32_t & initial)
{
	const unsigned char *current;
	uint32_t result = 0;

	result = initial;

	current = (const unsigned char *)buffer;
	result = (result ^ ~0U);

	while (size--)
	{
		result = crc32Poly [(result ^ *current++) & 0xFF] ^ (result >> 8);
	}

	result ^= ~0U;

	initial = result;
	return result;
}


/** Converts the initial part of the string in stringptr to a long integer value according to the given base, which must be between 2, 8, 10, 16.
@param stringptr String to be converted
@param endptr Character that stops the convertion
@param base Base of return value
@return returns the value converted */
int64_t strtolong(const char* stringptr, char** endptr, int base)
{
	const char* str = stringptr;
	char sign = '+';
	int64_t result = 0;
	int64_t temp = 0;
	int64_t val = 0;
	int refbase = base;
	unsigned char current;
	if (stringptr)
	{
		if (endptr)
		{
			*endptr = (char*)stringptr;
		}
		while (*str == ' ' || *str == '\t' || *str == '\v' || *str == '\f' || *str == '\r' || *str == '\n')
		{
			str++;
		}

		if (*str == '-' || *str == '+')
		{
			sign = *str++;
		}

		if (*str == '0')
		{
			str++;
			current = *str;

			// hexadecimal
			if (current == 'x' || current == 'X')
			{
				str++;
				base = 16;
			}
			// octal
			else if (current == 'o' || current == 'O')
			{
				str++;
				base = 8;
			}
			// binary
			else if (current == 'b' || current == 'B')
			{
				str++;
				base = 2;
			}
			// decimal
			else
			{
				str--;
				base = 10;
			}
		}
		else if (*str == '\\')
		{
			str++;
			current = *str;

			// unicode (hexadecimal)
			if (current == 'u' || current == 'U')
			{
				str++;
				base = 16;
			}
			// octal
			else if (current == 'o' || current == 'O')
			{
				str++;
				base = 8;
			}
			// hexadecimal
			else if (current == 'x' || current == 'X')
			{
				str++;
				base = 16;
			}
			// binary
			else if (current == 'b' || current == 'B')
			{
				str++;
				base = 2;
			}
			else
			{
				base = 8;
			}
		}
		else if (base == 0)
		{
			// decimal
			base = 10;
		}

		if (refbase != base && refbase != 0)
		{
			base = refbase;
		}

		while (*str)
		{
			unsigned char current = *str;
			if (current >= '0' && current <= '9')
			{
				temp = temp * base;
				val = (current - 0x30);
				if (val >= base)
				{
					if (endptr)
					{
						*endptr = (char*)str;
					}
					break;
				}
				result = val + temp;
				temp = result;
			}
			else if (current >= 'A' && current <= 'Z')
			{
				temp = temp * base;
				val = (current - 55);
				if (val >= base)
				{
					if (endptr)
					{
						*endptr = (char*)str;
					}
					break;
				}
				result = val + temp;
				temp = result;
			}
			else if (current >= 'a' && current <= 'z')
			{
				temp = temp * base;
				val = (current - 87);
				if (val >= base)
				{
					if (endptr)
					{
						*endptr = (char*)str;
					}
					break;
				}
				result = val + temp;
				temp = result;
			}
			else
			{
				if (endptr)
				{
					*endptr = (char*)str;
				}
				break;
			}
			str++;
			if (endptr)
			{
				*endptr = (char*)str;
			}
		}

		if (result)
		{
			if (sign == '-')
			{
				result = -result;
			}
		}
	}
	return result;
}

/** The result max of square root must be lower than 4096 */
uint64_t sqr(uint64_t value)
{
	uint64_t result;
	if (value <= 1)
	{
		result = value;
	}
	else
	{
		static uint64_t previous_value = 0xCAFEDEAD;
		static uint64_t previous_result = 0xDEADCAFE;

		result = 0;
		if (value == previous_value)
		{
			result = previous_result;
			if (value != previous_value)
			{
				result = 0;
			}
		}
		
		if (result == 0)
		{
			// Initialization with an initial guess
			result = value;

			// Newton-Raphson iteration to compute sqrt(value)
			// (Note: Stop when the approximation no longer changes)
			uint64_t x = value;
			uint64_t approx = (x + 1) >> 1;  // initial guess: value / 2

			while (approx < result)
			{
				result = approx;
				approx = (result + value / result) >> 1;  // Newton-Raphson update
			}

			previous_value = value;
			previous_result = result;
		}
	}
	return result;
}

FT_Matrix vector_matrix(int32_t angle)
{
	static int32_t previous_angle = 0xCAFEFADE;
	static FT_Matrix previous_matrix;

	if(angle == previous_angle)
	{
		return previous_matrix;
	}
	else if(angle == -previous_angle)
	{
		FT_Matrix matrix;

		matrix.xx = previous_matrix.xx;
		matrix.yy = previous_matrix.yy;
		matrix.xy = previous_matrix.yx;
		matrix.yx = previous_matrix.xy;
	
		previous_matrix = matrix;
		previous_angle = angle;
		return matrix;
	}
	else
	{
		FT_Vector sincosvec;
		FT_Vector_Unit(&sincosvec, FT_ANGLE_PI2 + angle);
		FT_Matrix matrix;
		matrix.yy = sincosvec.y;
		matrix.xx = sincosvec.y;
		matrix.yx = sincosvec.x;
		matrix.xy = -sincosvec.x;
		previous_matrix = matrix;
		previous_angle = angle;
		return matrix;
	}
}

void calc_point(Point & p, Dim radius, FT_Vector & sincos)
{
	p.x_(p.x_() + (Coord)((((int64_t)(sincos.x)*(int64_t)(radius)) + (1 << 15))>>16));
	p.y_(p.y_() + (Coord)((((int64_t)(sincos.y)*(int64_t)(radius)) + (1 << 15))>>16));
}

void calc_cubic(Point & p, Dim radius, FT_Vector & sincos)
{
	p.x_(p.x_()+ (Coord)((((int64_t)(sincos.y)*(int64_t)(radius)) + (1 << 15)) >>16));
	p.y_(p.y_()+ (Coord)((((int64_t)(sincos.x)*(int64_t)(radius)) + (1 << 15)) >>16));
}

bool match_pattern(const char *pattern, const char *string, bool ignore_case)
{
	size_t pattern_len = strlen(pattern);
	size_t string_len = strlen(string);

	size_t i = 0;
	size_t j = 0;

	while (i < pattern_len && j < string_len)
	{
		if (pattern[i] == '*')
		{
			// Match zero or more characters
			while (i < pattern_len && pattern[i] == '*')
			{
				i++;
			}
			if (i == pattern_len)
			{
				return true; // Pattern matches if it ends with *
			}

			if (ignore_case)
			{
				while (j < string_len && Strnicmp(pattern + i, string + j, 1) != 0)
				{
					j++;
				}
			}
			else
			{
				while (j < string_len && strncmp(pattern + i, string + j, 1) != 0)
				{
					j++;
				}
			}
		}
		else if (pattern[i] == '?')
		{
			// Match a single character
			i++;
			j++;
		}
		else if (pattern[i] == string[j])
		{
			// Match a single character
			i++;
			j++;
		}
		else
		{
			return false; // Pattern does not match
		}
	}
	return i == pattern_len && j == string_len;
}

#define A 1664525
#define C 1013904223
#define M 4294967296 // 2^32

static uint32_t seed = 1; 
void set_rand(uint32_t new_seed)
{
	seed = new_seed;
}

int32_t get_rand(int32_t mini, int32_t maxi)
{
	int32_t result;
	seed = (1664525 * seed + 1013904223) % 4294967296;

	result = (seed % (maxi-mini)) + mini;
	if (result < mini || result > maxi)
	{
		result = result;
	}
	return result;
}


Dim compute_zoom(Dim value, Dim zoom)
{
	return (Dim)((((uint64_t)value * (uint64_t)zoom)) >> 6);
}

}


#ifdef WIN32
	#include <windows.h>
	void usleep(int64_t usec) 
	{ 
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
	}
	
	long long clockns()
	{
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
	}
#endif

#ifdef LINUX
#include <time.h>
	long long clockns()
	{
		long long elapsed_time;
		clock_gettime(CLOCK_MONOTONIC, &elapsed_time);
		return (elapsed_time.tv_sec) * 1000000000LL + (elapsed_time.tv_nsec);
	}
#endif


#ifdef _DEBUG
#include <math.h>
#include <stdio.h>
void tools_test()
{
	uint32_t i;
	uint32_t res;
	uint32_t shift = 6;
	double r1, r2;

	for (i = 0; i < 0x01000000; i += 1)
	{
		res = (uint32_t)berialdraw::sqr((uint64_t)(i << shift));

		r1 = ((float)res) / (1 << (shift / 2));
		r2 = sqrt((int32_t)i);
		if (abs(int32_t(r1 - r2)) >= 1)
		{
			bd_printf("%d %.4lf %.4lf %d\n", i, r1, r2, abs(int32_t(r1 - r2)));
		}
	}
}
#endif
