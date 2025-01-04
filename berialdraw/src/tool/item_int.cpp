#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Create new item string */
ItemInt::ItemInt(int64_t value,  bool accurate)
{
	m_value = (value << 1) | (accurate ? 1 : 0);
}

/** Destructor */
ItemInt::~ItemInt()
{
}

/** Serialize to json */
void ItemInt::serialize(TextStream & out, int32_t indent)
{
	to_string(out);
}

/** Unserialize from json */
bool ItemInt::unserialize(TextStream & content)
{
	String integer;
	bool result = false;
	bool accurate = false;
	if (content.read_integer(m_value, accurate) > 0)
	{
		m_value <<= 1;
		if (accurate)
		{
			m_value |= 1;
		}
		result = true;
	}
	return result;
}

void ItemInt::to_string(TextStream & out) const
{
	if ((m_value & 1) == 1)
	{
		uint64_t integer =  m_value < 0 ? -(m_value>>1) : (m_value>>1);

		if (integer == Size::MAX_SIZE)
		{
			out.write_format("0x%08X",integer);
		}
		else
		{
			uint32_t decimal = ((((((uint32_t)integer %64))*1000)/64)+5)/10;
			integer >>= 6;

			if (m_value < 0)
			{
				out.write_string("-");
			}
			if (decimal)
			{
				out.write_format("%lld.%02d",integer, decimal);
			}
			else
			{
				out.write_format("%lld",integer);
			}
		}
	}
	else
	{
		out.write_format("%lld", m_value>>1);
	}
}

/** Convert to integer */
ItemInt::operator int() const
{
	return (int)(m_value>>1);
}

/** Convert to integer */
ItemInt::operator unsigned int() const
{
	return (unsigned int)(m_value>>1);
}

/** Convert to long long */
ItemInt::operator long long() const
{
	return (m_value>>1);
}

/** Convert to boolean */
ItemInt::operator bool() const
{
	if (m_value>>1)
	{
		return true;
	}
	return false;
}

/** Convert to string */
ItemInt::operator const String() const
{
	String result;
	to_string(result);
	return result;
}

// Compare if equal
bool ItemInt::operator==(const char * other) const
{
	bool result = false;
	int value;
	if (String::to_int(other, value))
	{
		result = (m_value>>1 == value);
	}
	return result;
}

bool ItemInt::operator==(const String & other) const
{
	bool result = false;
	int value;
	if (String::to_int(other, value))
	{
		result = (m_value>>1 == value);
	}
	return result;
}

bool ItemInt::operator==(int other) const
{
	return other == m_value>>1;
}

bool ItemInt::operator==(unsigned int other) const
{
	return other == (unsigned int)m_value>>1;
}

bool ItemInt::operator==(long long other) const
{
	return other == m_value>>1;
}

bool ItemInt::operator==(bool other) const
{
	return other == (m_value>>1 != 0);
}

// Compare if different
bool ItemInt::operator!=(const char * other) const
{
	bool result = true;
	int value;
	if (String::to_int(other, value))
	{
		result = (m_value>>1 != value);
	}
	return result;
}

bool ItemInt::operator!=(const String & other) const
{
	bool result = true;
	int value;
	if (String::to_int(other, value))
	{
		result = (m_value>>1 != value);
	}
	return result;
}

bool ItemInt::operator!=(int other) const
{
	return other != m_value>>1;
}

bool ItemInt::operator!=(unsigned int other) const
{
	return other != (unsigned int)m_value>>1;
}

bool ItemInt::operator!=(long long other) const
{
	return other != m_value>>1;
}

bool ItemInt::operator!=(bool other) const
{
	return other != (m_value>>1 != 0);
}

/** Indicates if the item is null */
bool ItemInt::is_null() const
{
	return false;
}

/** Indicates if the integer is accurate */
bool ItemInt::is_accurate() const
{
	return (m_value & 1);
}

