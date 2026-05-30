#include "berialdraw_imp.hpp"

using namespace berialdraw;

void berialdraw::serialize(const char * name, JsonIterator & it, ImageFitMode value)
{
	const char * text;
	switch(value)
	{
	case STRETCH :text = "stretch" ;break;
	case FIT     :text = "fit"     ;break;
	default                :text = "fit"     ;break;      
	}
	it[name] = text;
}

void berialdraw::unserialize(const char * name, JsonIterator & it, ImageFitMode & value)
{
	String text = it[name] | "fit";

	if (text == "stretch")
	{
		value = STRETCH;
	}
	else
	{
		value = FIT;
	}
}
