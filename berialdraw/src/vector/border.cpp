#include "berialdraw_imp.hpp"

using namespace berialdraw;

namespace berialdraw
{
	/** Unserialize the content of extend from json */
	void unserialize(JsonIterator & it, Borders & borders)
	{
		String str = it["borders"] | "nothing";
	
		if (str != "nothing")
		{
			borders = NO_BORDER;

			if (str.find("top") != INT32_MAX)
			{
				borders = borders | TOP_BORDER;
			}
			if (str.find("bottom") != INT32_MAX)
			{
				borders = borders | BOTTOM_BORDER;
			}
			if (str.find("left") != INT32_MAX)
			{
				borders = borders | LEFT_BORDER;
			}
			if (str.find("right") != INT32_MAX)
			{
				borders = borders | LEFT_BORDER;
			}
			if (str.find("all") != INT32_MAX)
			{
				borders = borders | ALL_BORDERS;
			}
			if (str.find("inner") != INT32_MAX)
			{
				borders = borders | INNER_AREA;
			}
			if (str.find("right-angle-end") != INT32_MAX)
			{
				borders = borders | RIGHT_ANGLE_END;
			}
			if (str.find("rounded-end") != INT32_MAX)
			{
				borders = borders | ROUNDED_END;
			}
			if (str.find("right-angle-without-border") != INT32_MAX)
			{
				borders = borders | RIGHT_ANGLE_WITHOUT_BORDER;
			}
		}
	}

	/** Serialize the content of extend into json */
	void serialize(JsonIterator & it, Borders borders)
	{
		String value;
		switch(borders &ALL_BORDERS)
		{
		case NO_BORDER                              :value = "none";             break;
		case TOP_BORDER                             :value = "top";              break;
		case RIGHT_BORDER                           :value = "right";            break;
		case BOTTOM_BORDER                          :value = "bottom";           break;
		case LEFT_BORDER                            :value = "left";             break;
		case TOP_BORDER|RIGHT_BORDER                :value = "top,right";        break;
		case TOP_BORDER|LEFT_BORDER                 :value = "top,left";         break;
		case BOTTOM_BORDER|RIGHT_BORDER             :value = "bottom,right";     break;
		case BOTTOM_BORDER|LEFT_BORDER              :value = "bottom,left";      break;
		case TOP_BORDER|BOTTOM_BORDER               :value = "top,bottom";       break;
		case LEFT_BORDER|RIGHT_BORDER               :value = "left,right";       break;
		case TOP_BORDER|RIGHT_BORDER|BOTTOM_BORDER  :value = "top,right,bottom"; break;
		case RIGHT_BORDER|BOTTOM_BORDER|LEFT_BORDER :value = "right,bottom,left";break;
		case BOTTOM_BORDER|LEFT_BORDER|TOP_BORDER   :value = "bottom,left,top";  break;
		case LEFT_BORDER|TOP_BORDER|RIGHT_BORDER    :value = "left,top,right";   break;
		case ALL_BORDERS                            :value = "all";              break;
		}

		if (borders & INNER_AREA)
		{
			value = value + ",inner";
		}
		if (borders & RIGHT_ANGLE_END)
		{
			value = value + ",right-angle-end";
		}
		if (borders & ROUNDED_END)
		{
			value = value + ",rounded-end";
		}
		if (borders & RIGHT_ANGLE_WITHOUT_BORDER)
		{
			value = value + ",right-angle-without-border";
		}
		it["borders"] = value;
	}
}

