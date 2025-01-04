#ifndef Dim_hpp_INCLUDED
#define Dim_hpp_INCLUDED

namespace berialdraw
{
	typedef uint32_t Dim;

	Dim substract(Dim a, Dim b);
	Dim multiply(Dim a, Dim b);
	Dim multiply_(Dim a, Dim b);
	bool unserialize(const char * name, JsonIterator & it, Dim & value);
}
#endif