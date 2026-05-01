#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Marker::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		Circle::test();
		Triangle::test();
		Square::test();
		Cross::test();
	}
}
