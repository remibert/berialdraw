#include "berialdraw_imp.hpp"

using namespace berialdraw;

ItemBool::ItemBool(bool value) : m_value(value)
{
}

/** Destructor */
ItemBool::~ItemBool()
{
}

/** Serialize to json */
void ItemBool::serialize(TextStream & out, int32_t indent)
{
	if(m_value)
	{
		out.write_string("true");
	}
	else
	{
		out.write_string("false");
	}
}

/** Unserialize from json */
bool ItemBool::unserialize(TextStream & content)
{
	bool result = false;
	String name;
	if (content.read_name(name) > 0)
	{
		if (name == "false" || name == "False")
		{
			m_value = false;
			result = true;
		}
		else if (name == "true" || name == "True")
		{
			m_value = true;
			result = true;
		}
	}
	return result;
}

/** Convert to integer */
ItemBool::operator int() const
{
	int result = 0;
	if(m_value)
	{
		result = 1;
	}
	return result;
}

/** Convert to integer */
ItemBool::operator unsigned int() const
{
	unsigned int result = 0;
	if(m_value)
	{
		result = 1;
	}
	return result;
}

/** Convert to long */
ItemBool::operator long long() const
{
	long long result = 0;
	if(m_value)
	{
		result = 1;
	}
	return result;
}

/** Convert to boolean */
ItemBool::operator bool() const
{
	return m_value;
}

/** Convert to string */
ItemBool::operator const String() const
{
	if (m_value)
	{
		String result("true");
		return result;
	}
	else
	{
		String result("false");
		return result;
	}
}

bool ItemBool::is_true(const char * other)
{
	return ItemString::to_int(other) != 0LL;
}

// Compare if equal
bool ItemBool::operator==(const char * other) const
{
	return is_true(other) == m_value;
}

bool ItemBool::operator==(const String & other) const
{
	return is_true(other.c_str()) == m_value;
}

bool ItemBool::operator==(int other) const
{
	return (other !=0) == m_value;
}

bool ItemBool::operator==(unsigned int other) const
{
	return (other !=0) == m_value;
}

bool ItemBool::operator==(long long other) const
{
	return (other !=0) == m_value;
}

bool ItemBool::operator==(bool other) const
{
	return other == m_value;
}

// Compare if different
bool ItemBool::operator!=(const char * other) const
{
	return is_true(other) != m_value;
}

bool ItemBool::operator!=(const String & other) const
{
	return is_true(other) != m_value;
}

bool ItemBool::operator!=(int other) const
{
	return (other !=0) != m_value;
}

bool ItemBool::operator!=(unsigned int other) const
{
	return (other !=0) != m_value;
}

bool ItemBool::operator!=(long long other) const
{
	return (other !=0) != m_value;
}

bool ItemBool::operator!=(bool other) const
{
	return other != m_value;
}

/** Indicates if the item is null */
bool ItemBool::is_null() const
{
	return false;
}

/** Indicates if the integer is accurate */
bool ItemBool::is_accurate() const
{
	return false;
}

