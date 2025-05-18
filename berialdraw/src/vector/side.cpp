#include "berialdraw_imp.hpp"

using namespace berialdraw;

namespace berialdraw
{
	/** Unserialize the content of extend from json */
	void unserialize(JsonIterator & it, Side & sides)
	{
		String str = it["sides"] | "nothing";
	
		if (str != "nothing")
		{
			sides = NO_SIDE;

			if (str.find("top") != INT32_MAX)
			{
				sides = sides | TOP_SIDE;
			}
			if (str.find("bottom") != INT32_MAX)
			{
				sides = sides | BOTTOM_SIDE;
			}
			if (str.find("left") != INT32_MAX)
			{
				sides = sides | LEFT_SIDE;
			}
			if (str.find("right") != INT32_MAX)
			{
				sides = sides | LEFT_SIDE;
			}
			if (str.find("all") != INT32_MAX)
			{
				sides = sides | ALL_SIDES;
			}
			if (str.find("inner") != INT32_MAX)
			{
				sides = sides | INNER_AREA;
			}
			if (str.find("rectangular") != INT32_MAX)
			{
				sides = sides | RECTANGULAR_EXTREMITY;
			}
		}
	}

	/** Serialize the content of extend into json */
	void serialize(JsonIterator & it, Side sides)
	{
		String value;
		switch(sides &ALL_SIDES)
		{
		case NO_SIDE               :value = "none";             break;
		case TOP_SIDE              :value = "top";              break;
		case RIGHT_SIDE            :value = "right";            break;
		case BOTTOM_SIDE           :value = "bottom";           break;
		case LEFT_SIDE             :value = "left";             break;
		case TOP_RIGHT_SIDE        :value = "top,right";        break;
		case TOP_LEFT_SIDE         :value = "top,left";         break;
		case BOTTOM_RIGHT_SIDE     :value = "bottom,right";     break;
		case BOTTOM_LEFT_SIDE      :value = "bottom,left";      break;
		case TOP_BOTTOM_SIDE       :value = "top,bottom";       break;
		case LEFT_RIGHT_SIDE       :value = "left,right";       break;
		case TOP_RIGHT_BOTTOM_SIDE :value = "top,right,bottom"; break;
		case RIGHT_BOTTOM_LEFT_SIDE:value = "right,bottom,left";break;
		case BOTTOM_LEFT_TOP_SIDE  :value = "bottom,left,top";  break;
		case LEFT_TOP_RIGHT_SIDE   :value = "left,top,right";   break;
		case ALL_SIDES             :value = "all";              break;
		}

		if (sides & INNER_AREA)
		{
			value = value + ",inner";
		}
		if (sides & RECTANGULAR_EXTREMITY)
		{
			value = value + ",rectangular";
		}
		it["sides"] = value;
	}
}

