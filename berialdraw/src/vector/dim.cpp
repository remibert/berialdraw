#include "berialdraw_imp.hpp"

using namespace berialdraw;


Dim berialdraw::substract(Dim a, Dim b)
{
	if (a > b)
	{
		return a-b;
	}
	return 0;
}

Dim berialdraw::multiply(Dim a, Dim b)
{
	return a*b;
}

Dim berialdraw::multiply_(Dim a, Dim b)
{
	Dim result;
	result = (Dim)(((uint64_t) a * (uint64_t) b)>>6);
	return result;
}

bool berialdraw::unserialize(const char * name, JsonIterator & it, Dim & value)
{
	int v     = it[name]    | Size::MAX_SIZE;
	value     = (v    == Size::MAX_SIZE) ? value    : v;
	return v == Size::MAX_SIZE;
}
