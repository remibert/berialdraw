#include "berialdraw_imp.hpp"

using namespace berialdraw;

Coord berialdraw::multiply(Coord a, Coord b)
{
	return a*b;
}

Coord berialdraw::multiply_(Coord a, Coord b)
{
	Coord result;
	result = (Coord)(((uint64_t) a * (uint64_t) b)>>6);
	return result;
}


bool berialdraw::unserialize(const char * name, JsonIterator & it, Coord & value)
{
	int v     = it[name]    | Size::MAX_SIZE;
	value     = (v    == Size::MAX_SIZE) ? value    : v ;
	return v == Size::MAX_SIZE;
}