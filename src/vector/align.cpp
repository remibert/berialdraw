#include "berialdraw_imp.hpp"

using namespace berialdraw;

void berialdraw::serialize(const char * name, JsonIterator & it, Align value)
{
	const char * text;
	switch(value)
	{
	case Align::ALIGN_LEFT        :text = "left"         ;break;
	case Align::ALIGN_RIGHT       :text = "right"        ;break;
	case Align::ALIGN_TOP         :text = "top"          ;break;
	case Align::ALIGN_BOTTOM      :text = "bottom"       ;break;
	case Align::ALIGN_TOP_LEFT    :text = "top,left"     ;break;
	case Align::ALIGN_TOP_RIGHT   :text = "top,right"    ;break;
	case Align::ALIGN_BOTTOM_LEFT :text = "bottom,left"  ;break;
	case Align::ALIGN_BOTTOM_RIGHT:text = "bottom,right" ;break;
	case Align::ALIGN_HORIZONTAL  :text = "horizontal"   ;break;
	case Align::ALIGN_VERTICAL    :text = "vertical"     ;break;
	case Align::CENTER            :text = "center"       ;break;
	default                :text = "default"      ;break;      
	}
	it[name] = text;
}

void berialdraw::unserialize(const char * name, JsonIterator & it, Align & value)
{
	String text = it[name] | "default";

	if (text != "default")
	{
		value = Align::CENTER;

		if (text.find("left") != INT32_MAX)
		{
			value = (value | Align::ALIGN_LEFT);
		}
		if (text.find("right") != INT32_MAX)
		{
			value = (value | Align::ALIGN_RIGHT);
		}
		if (text.find("top") != INT32_MAX)
		{
			value = (value | Align::ALIGN_TOP);
		}
		if (text.find("bottom") != INT32_MAX)
		{
			value = (value | Align::ALIGN_BOTTOM);
		}
	}
	else
	{
		value = Align::ALIGN_DEFAULT;
	}
}
