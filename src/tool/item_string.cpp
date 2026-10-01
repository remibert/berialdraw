#include "berialdraw_imp.hpp"

using namespace berialdraw;

ItemString::ItemString(String & string) : m_value(string)
{
}

ItemString::ItemString(const char * string) : m_value(string)
{
}

/** Destructor */
ItemString::~ItemString()
{
}

// Write a json string, optionally dropping the precision suffix of a key
static void write_json_string(String & value, TextStream & out, bool strip_q6_suffix)
{
	uint32_t length = value.count();
	const size_t suffix_len = sizeof(Q6_SUFFIX) - 1;
	const char * raw = value.c_str();
	size_t raw_len = raw ? strlen(raw) : 0;

	// Suffix characters are not written
	if (strip_q6_suffix && raw_len >= suffix_len && strcmp(raw + raw_len - suffix_len, Q6_SUFFIX) == 0)
	{
		length -= (uint32_t)suffix_len;
	}

	out.write_string("\"");

	value.offset(0);
	wchar_t character = value.read_char();

	for (uint32_t i = 0; i < length; i++)
	{
		if ((unsigned char) character >= ' ' && character != '\"' && character != '\\')
		{
			out.write_char(character);
		}
		else
		{
			switch (character)
			{
			case '\\' : out.write_string("\\");   break;
			case '\"' : out.write_string("\\\""); break;
			case '\b' : out.write_string("\\b" ); break;
			case '\f' : out.write_string("\\f" ); break;
			case '\n' : out.write_string("\\n" ); break;
			case '\r' : out.write_string("\\r" ); break;
			case '\t' : out.write_string("\\t" ); break;
			default:
				{
					unsigned char code = (unsigned char)character;
					static const char tohex[] = "0123456789ABCDEF";

					out.write_string("\\x");
					out.write_char(tohex[(code >> 4)]);
					out.write_char(tohex[(code & 0x0F)]);
				}
			}
		}
		character = value.read_char();
	}
	out.write_string("\"");
}

// Serialize to json
void ItemString::serialize(TextStream & out, int32_t indent)
{
	write_json_string(m_value, out, false);
}

// Serialize as a json key, without the precision suffix
void ItemString::serialize_key(TextStream & out)
{
	write_json_string(m_value, out, true);
}

/** Unserialize from json */
bool ItemString::unserialize(TextStream & content)
{
	bool result = true;
	wchar_t quote = content.read_char();

	if (quote == '\'' || quote == '\"')
	{
		wchar_t character = content.read_char();
		bool docstring = false;
		
		// If python comment perhaps detected
		if (character == quote)
		{
			uint32_t offset = content.offset();
			character = content.read_char();

			// If python comment detected
			if (character == quote)
			{
				docstring = true;
				character = content.read_char();
			}
			else
			{
				content.offset(offset);
				// String closed
				character = quote;
			}
		}

		while(character)
		{
			if (docstring)
			{
				if (character == quote)
				{
					uint32_t offset2 = content.offset();

					// If python comment detected
					if (content.read_char() == quote && content.read_char() == quote)
					{
						break;
					}
					else
					{
						content.offset(offset2);
					}
				}
			}
			else if (character == quote)
			{
				break;
			}

			if(character != '\\')
			{
				m_value += character;
			}
			else
			{
				wchar_t backvalue = content.read_char();
				switch (backvalue)
				{
				case 'b': m_value+= '\b'; break;
				case 'f': m_value+= '\f'; break;
				case 'n': m_value+= '\n'; break;
				case 'r': m_value+= '\r'; break;
				case 't': m_value+= '\t'; break;
				case '\\': m_value+= '\\'; break;
				case 'x':
				case 'u':
					{
						String hexastring;
						//uint32_t offsethexa = content.offset();
						wchar_t hexa;
						do
						{
							hexa = content.read_char();
							if ((hexa >= '0' && hexa <= '9') || (hexa >= 'A' && hexa <= 'F') || (hexa >= 'a' && hexa <= 'f'))
							{
								//offset = offsethexa;
								hexastring += hexa;
							}
							else
							{
								content.offset(content.offset()-1);
								break;
							}
						}
						while (hexastring.size() <= 4 && backvalue == 'u' ||
								hexastring.size() <= 2 && backvalue == 'x' );
						
						if (hexastring.size() >= 1)
						{
							character = (wchar_t)strtolong(hexastring.c_str(), 0, 16);
							m_value += character;
						}
						else
						{
							result = false;
						}
					}
					break;
				default:
					m_value += backvalue;
					break;
				}
			}
			character = content.read_char();
		}

		if(character != quote)
		{
			result = false;
		}
	}
	else
	{
		result = false;
	}
	return result;
}

/** Convert to integer */
ItemString::operator int() const
{
	return (int)ItemString::to_int(m_value.c_str());
}

/** Convert to integer */
ItemString::operator unsigned int() const
{
	return (unsigned int)ItemString::to_int(m_value.c_str());
}

/** Convert to long long */
ItemString::operator long long() const
{
	return ItemString::to_int(m_value.c_str());
}

/** Convert to boolean */
ItemString::operator bool() const
{
	bool result = false;
	if (ItemBool::is_true(m_value.c_str()))
	{
		result = true;
	}
	return result;
}

/** Convert to string */
ItemString::operator const String() const
{
	return m_value;
}

/** Get the internal string */
const String & ItemString::get() const
{
	return m_value;
}

// Compare if equal
bool ItemString::operator==(const char * other) const
{
	if(m_value == other)
	{
		return true;
	}
	return false;
}

bool ItemString::operator==(const String & other) const
{
	return m_value == other;
}

bool ItemString::operator==(int other) const
{
	return other == int(*this);
}

bool ItemString::operator==(unsigned int other) const
{
	return other == (unsigned int)(*this);
}

bool ItemString::operator==(long long other) const
{
	return other == (long long)(*this);
}

bool ItemString::operator==(bool other) const
{
	return ItemBool::is_true(m_value) == other;
}

// Compare if different
bool ItemString::operator!=(const char * other) const
{
	return m_value != other;
}

bool ItemString::operator!=(const String & other) const
{
	return m_value != other;
}

bool ItemString::operator!=(int other) const
{
	return other != int(*this);
}

bool ItemString::operator!=(unsigned int other) const
{
	return other != (unsigned int)(*this);
}

bool ItemString::operator!=(long long other) const
{
	return other == (long long)(*this);
}

bool ItemString::operator!=(bool other) const
{
	return ItemBool::is_true(m_value) != other;
}

long long ItemString::to_int(const char * str)
{
	long long result = 0;
	if(strcmp(str,"false") == 0 || strcmp(str,"False") == 0 || strcmp(str,"None") == 0)
	{
		result = 0;
	}
	else if(strcmp(str,"true") == 0 || strcmp(str,"True") == 0)
	{
		result = 1;
	}
	else
	{
		String::to_int(str, result);
	}
	return result;
}

/** Indicates if the item is null */
bool ItemString::is_null() const
{
	return false;
}

// Compare keys, ignoring a trailing suffix and the difference between '-' and '_'
bool ItemString::compare(const char * other, const char * ignored_suffix, bool & accurate) const
{
	bool result = false;
	const char * string = m_value.c_str();

	if(string && other)
	{
		const size_t suffix_len = strlen(ignored_suffix);
		size_t string_len = strlen(string);
		size_t other_len = strlen(other);
		bool string_suffix = string_len >= suffix_len && strcmp(string + string_len - suffix_len, ignored_suffix) == 0;
		bool other_suffix = other_len >= suffix_len && strcmp(other + other_len - suffix_len, ignored_suffix) == 0;

		if (string_suffix)
		{
			string_len -= suffix_len;
		}
		if (other_suffix)
		{
			other_len -= suffix_len;
		}

		if (string_len == other_len)
		{
			size_t i = 0;
			while (i < string_len &&
				((string[i] == other[i]) ||
				 (string[i] == '-' && other[i] == '_') ||
				 (string[i] == '_' && other[i] == '-')))
			{
				i++;
			}
			if (i == string_len)
			{
				accurate = other_suffix && !string_suffix;
				result = true;
			}
		}
	}
	else
	{
		if(string == 0 && other == 0)
		{
			accurate = true;
			result = true;
		}
	}
	return result;
}

/** Indicates if the integer is accurate */
bool ItemString::is_accurate() const
{
	return false;
}
