#ifndef Extend_hpp_INCLUDED
#define Extend_hpp_INCLUDED

namespace berialdraw
{
	enum Extend
	{
		EXTEND_NONE   = 0,
		EXTEND_WIDTH  = 1,
		EXTEND_HEIGHT = 2,
		EXTEND_ALL    = 3,
	};

	/** Unserialize the content of extend from json */
	void unserialize(JsonIterator & it, Extend & extend);
	
	/** Serialize the content of extend into json */
	void serialize(JsonIterator & it, Extend extend);
}
#endif