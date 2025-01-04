#ifndef Coord_hpp_INCLUDED
#define Coord_hpp_INCLUDED

namespace berialdraw
{
	typedef int32_t Coord;

	Coord multiply(Coord a, Coord b);
	Coord multiply_(Coord a, Coord b);
	bool unserialize(const char * name, JsonIterator & it, Coord & value);
}
#endif