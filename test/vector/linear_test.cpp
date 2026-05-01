#include "berialdraw_imp.hpp"
using namespace berialdraw;

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

	l1.get_intersection(l2, intersect1);
	l2.get_intersection(l1, intersect2);
}

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

	l1.get_intersection(l2, intersect1);
	l2.get_intersection(l1, intersect2);
}
