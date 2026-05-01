#include "berialdraw_imp.hpp"

using namespace berialdraw;

void berialdraw::serialize(const char * name, JsonIterator & it, ScrollDirection value)
{
	const char * text;
	switch(value)
	{
	case SCROLL_HORIZONTAL      :text = "horizontal"      ;break;
	case SCROLL_VERTICAL        :text = "vertical"        ;break;
	case SCROLL_ALL_DIRECTIONS  :text = "all-directions"  ;break;
	default                     :text = "default"         ;break;      
	}
	it[name] = text;
}

void berialdraw::unserialize(const char * name, JsonIterator & it, ScrollDirection & value)
{
	String text = it[name] | "unmodified";
	if      (text == "horizontal"      ) value = SCROLL_HORIZONTAL      ;
	else if (text == "vertical"        ) value = SCROLL_VERTICAL        ;
	else if (text == "all-directions"  ) value = SCROLL_ALL_DIRECTIONS  ;
	else if (text == "default"         ) value = SCROLL_VERTICAL        ;
}
