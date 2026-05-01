#include "berialdraw_imp.hpp"

namespace berialdraw
{
	/** Unserialize the content of extend from json */
	void unserialize(JsonIterator & it, Extend & extend)
	{
		if (it["extend"] == "none")   extend = EXTEND_NONE  ;
		if (it["extend"] == "width")  extend = EXTEND_WIDTH ;
		if (it["extend"] == "height") extend = EXTEND_HEIGHT;
		if (it["extend"] == "all")    extend = EXTEND_ALL   ;
	}

	/** Serialize the content of extend into json */
	void serialize(JsonIterator & it, Extend extend)
	{
		switch(extend)
		{
		case EXTEND_NONE  : it["extend"] = "none" ; break;
		case EXTEND_WIDTH : it["extend"] = "width" ; break;
		case EXTEND_HEIGHT: it["extend"] = "height"; break;
		case EXTEND_ALL   : it["extend"] = "all"; break;
		}
	}
}