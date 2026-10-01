#pragma once
namespace berialdraw
{
	enum class ListSelectionMode
	{
		LIST_NO_SELECTION      = 0,
		LIST_SINGLE_SELECTION  = 1,
		LIST_MULTI_SELECTION   = 2,
	};

	/** Unserialize the content of list selection mode from json */
	void unserialize(JsonIterator & it, ListSelectionMode & mode);
	
	/** Serialize the content of list selection mode into json */
	void serialize(JsonIterator & it, ListSelectionMode mode);
}
