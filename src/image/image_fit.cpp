#include "berialdraw_imp.hpp"

using namespace berialdraw;

void berialdraw::serialize(const char * name, JsonIterator & it, ImageFitMode value)
{
	const char * text;
	switch(value)
	{
	case IMAGE_FIT_STRETCH      :text = "stretch"       ;break;
	case IMAGE_FIT_ASPECT_WIDTH :text = "aspect-width"  ;break;
	case IMAGE_FIT_ASPECT_HEIGHT:text = "aspect-height" ;break;
	case IMAGE_FIT_ASPECT_FIT   :text = "aspect-fit"    ;break;
	case IMAGE_FIT_ASPECT_FILL  :text = "aspect-fill"   ;break;
	case IMAGE_FIT_NONE         :text = "none"          ;break;
	default                     :text = "stretch"       ;break;      
	}
	it[name] = text;
}

void berialdraw::unserialize(const char * name, JsonIterator & it, ImageFitMode & value)
{
	String text = it[name] | "stretch";

	if (text == "stretch")
	{
		value = IMAGE_FIT_STRETCH;
	}
	else if (text == "aspect-width")
	{
		value = IMAGE_FIT_ASPECT_WIDTH;
	}
	else if (text == "aspect-height")
	{
		value = IMAGE_FIT_ASPECT_HEIGHT;
	}
	else if (text == "aspect-fit")
	{
		value = IMAGE_FIT_ASPECT_FIT;
	}
	else if (text == "aspect-fill")
	{
		value = IMAGE_FIT_ASPECT_FILL;
	}
	else if (text == "none")
	{
		value = IMAGE_FIT_NONE;
	}
	else
	{
		value = IMAGE_FIT_STRETCH;
	}
}
