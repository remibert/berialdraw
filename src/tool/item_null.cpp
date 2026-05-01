#include "berialdraw_imp.hpp"

using namespace berialdraw;

ItemNull::ItemNull()
{
}

/** Destructor */
ItemNull::~ItemNull()
{
}

/** Serialize to json */
void ItemNull::serialize(TextStream & out, int32_t indent)
{
	out.write_string("null");
}

/** Unserialize from json */
bool ItemNull::unserialize(TextStream & content)
{
	String name;
	if (content.read_name(name) > 0)
	{
		if (name == "null" || name == "None")
		{
			return true;
		}
	}
	return false;
}

/** Convert to integer */
ItemNull::operator int() const
{
	return 0;
}

/** Convert to integer */
ItemNull::operator unsigned int() const
{
	return 0;
}

/** Convert to long */
ItemNull::operator long long() const
{
	return 0;
}

/** Convert to boolean */
ItemNull::operator bool() const
{
	return false;
}

/** Convert to string */
ItemNull::operator const String() const
{
	return String::empty;
}

// Compare if equal
bool ItemNull::operator==(const char * other) const
{
	return false;
}

bool ItemNull::operator==(const String & other) const
{
	return false;
}

bool ItemNull::operator==(int other) const
{
	return false;
}

bool ItemNull::operator==(unsigned int other) const
{
	return false;
}

bool ItemNull::operator==(long long other) const
{
	return false;
}

bool ItemNull::operator==(bool other) const
{
	return false;
}

// Compare if different
bool ItemNull::operator!=(const char * other) const
{
	return false;
}

bool ItemNull::operator!=(const String & other) const
{
	return false;
}

bool ItemNull::operator!=(int other) const
{
	return false;
}

bool ItemNull::operator!=(unsigned int other) const
{
	return false;
}

bool ItemNull::operator!=(long long other) const
{
	return false;
}

bool ItemNull::operator!=(bool other) const
{
	return false;
}

/** Indicates if the item is null */
bool ItemNull::is_null() const
{
	return true;
}

/** Indicates if the integer is accurate */
bool ItemNull::is_accurate() const
{
	return false;
}
