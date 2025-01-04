#include "berialdraw_imp.hpp"

using namespace berialdraw;

void berialdraw::serialize(const char * name, JsonIterator & it, Align value)
{
	const char * text;
	switch(value)
	{
	case ALIGN_LEFT        :text = "left"        ;break;
	case ALIGN_RIGHT       :text = "right"       ;break;
	case ALIGN_TOP         :text = "top"         ;break;
	case ALIGN_BOTTOM      :text = "bottom"      ;break;
	case ALIGN_TOP_LEFT    :text = "top_left"    ;break;
	case ALIGN_TOP_RIGHT   :text = "top_right"   ;break;
	case ALIGN_BOTTOM_LEFT :text = "bottom_left" ;break;
	case ALIGN_BOTTOM_RIGHT:text = "bottom_right";break;
	case ALIGN_HORIZONTAL  :text = "horizontal"  ;break;
	case ALIGN_VERTICAL    :text = "vertical"    ;break;
	case CENTER            :text = "center"      ;break;
	default                :text = "default"     ;break;      
	}
	it[name] = text;
}

void berialdraw::unserialize(const char * name, JsonIterator & it, Align & value)
{
	String text = it[name] | "unmodified";
	if      (text == "left"        ) value = ALIGN_LEFT        ;
	else if (text == "right"       ) value = ALIGN_RIGHT       ;
	else if (text == "top"         ) value = ALIGN_TOP         ;
	else if (text == "bottom"      ) value = ALIGN_BOTTOM      ;
	else if (text == "top_left"    ) value = ALIGN_TOP_LEFT    ;
	else if (text == "top_right"   ) value = ALIGN_TOP_RIGHT   ;
	else if (text == "bottom_left" ) value = ALIGN_BOTTOM_LEFT ;
	else if (text == "bottom_right") value = ALIGN_BOTTOM_RIGHT;
	else if (text == "horizontal"  ) value = ALIGN_HORIZONTAL  ;
	else if (text == "vertical"    ) value = ALIGN_VERTICAL    ;
	else if (text == "center"      ) value = CENTER            ;
	else if (text == "default"     ) value = ALIGN_DEFAULT     ;
}
