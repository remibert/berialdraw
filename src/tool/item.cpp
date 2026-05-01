#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Return the type of node found in path selected */
enum JsonType Item::type(Item * item)
{
	enum JsonType result = JsonType::UNDEFINED;
	if(item)
	{
		if(dynamic_cast<ItemInt*>(item))
		{
			result = JsonType::INT;
		}
		else if(dynamic_cast<ItemString*>(item))
		{
			result = JsonType::STRING;
		}
		else if(dynamic_cast<ItemBool*>(item))
		{
			result = JsonType::BOOL;
		}
		else if(dynamic_cast<ItemArray*>(item))
		{
			result = JsonType::ARRAY;
		}
		else if(dynamic_cast<ItemObject*>(item))
		{
			result = JsonType::OBJECT;
		}
		else if(dynamic_cast<ItemNull*>(item))
		{
			result = JsonType::NONE;
		}
	}
	return result;	
}

const char * Item::typestr(Item *item)
{
	if(dynamic_cast<ItemInt*>(item))
	{
		return "int";
	}
	else if(dynamic_cast<ItemString*>(item))
	{
		return "string";
	}
	else if(dynamic_cast<ItemBool*>(item))
	{
		return "bool";
	}
	else if(dynamic_cast<ItemArray*>(item))
	{
		return "array";
	}
	else if(dynamic_cast<ItemObject*>(item))
	{
		return "object";
	}
	else if (dynamic_cast<ItemNull*>(item))
	{
		return "null";
	}
	else
	{
		return "undefined";
	}
}
