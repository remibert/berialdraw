#pragma once
namespace berialdraw
{
	typedef uint32_t Dim;

	Dim substract(Dim a, Dim b);
	Dim multiply(Dim a, Dim b);
	Dim multiply_q6(Dim a, Dim b);
	bool unserialize(const char * name, JsonIterator & it, Dim & value);
	bool unserialize(const char * name, JsonIterator & it, uint16_t & value);
}
