#include "berialdraw_imp.hpp"
using namespace berialdraw;

#include <math.h>
#include <stdio.h>
void tools_test1()
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

void tools_test2()
{
	FT_Matrix m = berialdraw::vector_matrix(0);

	assert(m.xx == 65536 && m.yy == 65536 && m.xy == 0 && m.yx == 0);
	m = berialdraw::vector_matrix(FT_ANGLE_PI2);
	assert(m.xx == 0 && m.yy == 0 && m.xy == 65536 && m.yx == -65536);
	m = berialdraw::vector_matrix(FT_ANGLE_PI);
	assert(m.xx == -65536 && m.yy == -65536 && m.xy == 0 && m.yx == 0);
	m = berialdraw::vector_matrix(FT_ANGLE_PI + FT_ANGLE_PI2);
	assert(m.xx == 0 && m.yy == 0 && m.xy == -65536 && m.yx == 65536);
}
