#pragma once
namespace berialdraw
{
	typedef int32_t Coord;

	Coord multiply(Coord a, Coord b);
	Coord multiply_q6(Coord a, Coord b);
	bool unserialize(const char * name, JsonIterator & it, Coord & value);
}
