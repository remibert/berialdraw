#include "berialdraw_imp.hpp"

namespace berialdraw
{
	/** Unserialize the content of list selection mode from json */
	void unserialize(JsonIterator & it, ListSelectionMode & mode)
	{
		if (it[StyleNames::LISTSTYLE_SELECTION_MODE] == "no-selection")     mode = LIST_NO_SELECTION     ;
		if (it[StyleNames::LISTSTYLE_SELECTION_MODE] == "single-selection") mode = LIST_SINGLE_SELECTION ;
		if (it[StyleNames::LISTSTYLE_SELECTION_MODE] == "multi-selection")  mode = LIST_MULTI_SELECTION  ;
	}

	/** Serialize the content of list selection mode into json */
	void serialize(JsonIterator & it, ListSelectionMode mode)
	{
		switch(mode)
		{
		case LIST_NO_SELECTION     : it[StyleNames::LISTSTYLE_SELECTION_MODE] = "no-selection" ; break;
		case LIST_SINGLE_SELECTION : it[StyleNames::LISTSTYLE_SELECTION_MODE] = "single-selection" ; break;
		case LIST_MULTI_SELECTION  : it[StyleNames::LISTSTYLE_SELECTION_MODE] = "multi-selection"; break;
		}
	}
}
