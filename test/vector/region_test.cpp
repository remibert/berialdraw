#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Region::test1()
{
	Region r1;
	Area a1(50,50,50,50,true);

	r1.unite(a1);
	assert(r1.is_inside(0,0,51,50)  == Region::OUT);
	assert(r1.is_inside(0,0,50,51)  == Region::OUT);
	assert(r1.is_inside(0,0,51,51)  == Region::PART);
	assert(r1.is_inside(0,0,150,150) == Region::PART);
	assert(r1.is_inside(50,50,50,50) == Region::IN);
	assert(r1.is_inside(99,99,50,50) == Region::PART);
	assert(r1.is_inside(99,100,50,50) == Region::OUT);
	assert(r1.is_inside(100,99,50,50) == Region::OUT);
	assert(r1.is_inside(100,100,50,50) == Region::OUT);

	Region r2(r1);
	Area a2(25,25,50,50, true);
	r2.intersect(a2);
	assert(r2.is_inside(0,0,51,50) == Region::OUT);
	assert(r2.is_inside(0,0,50,51) == Region::OUT);
	assert(r2.is_inside(0,0,51,51) == Region::PART);
	assert(r2.is_inside(0,0,150,150) == Region::PART);
	assert(r2.is_inside(25,25,50,50) == Region::PART);
	assert(r2.is_inside(74,74,50,50) == Region::PART);
	assert(r2.is_inside(74,75,50,50) == Region::OUT);
	assert(r2.is_inside(75,74,50,50) == Region::OUT);
	assert(r2.is_inside(75,75,50,50) == Region::OUT);
}


void Region::test2()
{
	Region r1;
	Area a1(50,50,50,50);

	r1.unite(a1);
	assert(r1.is_inside(10,10,10,10) == Region::OUT);
	assert(r1.is_inside(40,40,20,20) == Region::PART);
	assert(r1.is_inside(60,60,10,10) == Region::IN);
}

void Region::test()
{
	MemoryLeakLog
	test2();
	test1();
}
