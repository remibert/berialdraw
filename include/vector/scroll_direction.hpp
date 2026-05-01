#pragma once

namespace berialdraw
{
	/** Enumeration for scroll direction */
	enum ScrollDirection
	{
		SCROLL_HORIZONTAL = 0,
		SCROLL_VERTICAL = 1,
		SCROLL_ALL_DIRECTIONS = 2
	};

	class JsonIterator;

	/** Serialize ScrollDirection to JSON */
	void serialize(const char * name, JsonIterator & it, ScrollDirection value);

	/** Unserialize ScrollDirection from JSON */
	void unserialize(const char * name, JsonIterator & it, ScrollDirection & value);
}
