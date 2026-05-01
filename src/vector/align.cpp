#include "berialdraw_imp.hpp"

using namespace berialdraw;

void berialdraw::serialize(const char * name, JsonIterator & it, Align value)
{
	const char * text;
	switch(value)
	{
	case ALIGN_LEFT        :text = "left"         ;break;
	case ALIGN_RIGHT       :text = "right"        ;break;
	case ALIGN_TOP         :text = "top"          ;break;
	case ALIGN_BOTTOM      :text = "bottom"       ;break;
	case ALIGN_TOP_LEFT    :text = "top,left"     ;break;
	case ALIGN_TOP_RIGHT   :text = "top,right"    ;break;
	case ALIGN_BOTTOM_LEFT :text = "bottom,left"  ;break;
	case ALIGN_BOTTOM_RIGHT:text = "bottom,right" ;break;
	case ALIGN_HORIZONTAL  :text = "horizontal"   ;break;
	case ALIGN_VERTICAL    :text = "vertical"     ;break;
	case CENTER            :text = "center"       ;break;
	default                :text = "default"      ;break;      
	}
	it[name] = text;
}

void berialdraw::unserialize(const char * name, JsonIterator & it, Align & value)
{
	String text = it[name] | "default";

	if (text != "default")
	{
		value = (Align)0;

		if (text.find("left") != INT32_MAX)
		{
			value = (Align)(value | ALIGN_LEFT);
		}
		if (text.find("right") != INT32_MAX)
		{
			value = (Align)(value | ALIGN_RIGHT);
		}
		if (text.find("top") != INT32_MAX)
		{
			value = (Align)(value | ALIGN_TOP);
		}
		if (text.find("bottom") != INT32_MAX)
		{
			value = (Align)(value | ALIGN_BOTTOM);
		}
	}
	else
	{
		value = ALIGN_DEFAULT;
	}
}
