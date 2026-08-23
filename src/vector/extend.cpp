#include "berialdraw_imp.hpp"

namespace berialdraw
{
	/** Unserialize the content of extend from json */
	void unserialize(JsonIterator & it, Extend & extend)
	{
		if (it[StyleNames::EXTEND] == "none")   extend = EXTEND_NONE  ;
		if (it[StyleNames::EXTEND] == "width")  extend = EXTEND_WIDTH ;
		if (it[StyleNames::EXTEND] == "height") extend = EXTEND_HEIGHT;
		if (it[StyleNames::EXTEND] == "all")    extend = EXTEND_ALL   ;
	}

	/** Serialize the content of extend into json */
	void serialize(JsonIterator & it, Extend extend)
	{
		switch(extend)
		{
		case EXTEND_NONE  : it[StyleNames::EXTEND] = "none" ; break;
		case EXTEND_WIDTH : it[StyleNames::EXTEND] = "width" ; break;
		case EXTEND_HEIGHT: it[StyleNames::EXTEND] = "height"; break;
		case EXTEND_ALL   : it[StyleNames::EXTEND] = "all"; break;
		}
	}
}