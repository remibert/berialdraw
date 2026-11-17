#include "berialdraw_imp.hpp"
using namespace berialdraw;

// Test intersections of diagonal lines
void Linear::test1()
{
	Point p1(0, 0);
	Point p2(100, 100);

	Point p3(100, 0);
	Point p4(0, 100);

	Linear l1(p1, p2);
	Linear l2(p3, p4);

	Point intersect1(0, 0);
	Point intersect2(0, 0);

	assert(l1.get_intersection(l2, intersect1));
	assert(l2.get_intersection(l1, intersect2));
	assert(intersect1.x_q6() == (50 << 6));
	assert(intersect1.y_q6() == (50 << 6));
	assert(intersect2.x_q6() == intersect1.x_q6());
	assert(intersect2.y_q6() == intersect1.y_q6());

	Linear fractional1(0, 0, 192, 64);
	Linear fractional2(0, 65, 192, 0);
	Point fractional_intersect1;
	Point fractional_intersect2;

	assert(fractional1.get_intersection(fractional2, fractional_intersect1));
	assert(fractional2.get_intersection(fractional1, fractional_intersect2));
	assert(fractional_intersect1.x_q6() == 96);
	assert(fractional_intersect1.y_q6() == 32);
	assert(fractional_intersect2.x_q6() == fractional_intersect1.x_q6());
	assert(fractional_intersect2.y_q6() == fractional_intersect1.y_q6());
}

// Test intersections involving a vertical line
void Linear::test2()
{
	Point p1(50, 0);
	Point p2(50, 100);

	Point p3(100, 0);
	Point p4(0, 100);

	Linear l1(p1, p2);
	Linear l2(p3, p4);

	Point intersect1(0, 0);
	Point intersect2(0, 0);

	assert(l1.get_intersection(l2, intersect1));
	assert(l2.get_intersection(l1, intersect2));
	assert(intersect1.x_q6() == (50 << 6));
	assert(intersect1.y_q6() == (50 << 6));
	assert(intersect2.x_q6() == intersect1.x_q6());
	assert(intersect2.y_q6() == intersect1.y_q6());
}

// Run linear geometry tests once
void Linear::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test2();
		test1();
	}
}
