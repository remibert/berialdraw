#include "berialdraw_imp.hpp"
//#define JSON_TRACE
using namespace berialdraw;

const int32_t Json::LAST  = 0x7FFFFFFF;
const int32_t Json::FIRST = 0x80000000;

/** Constructor */
Json::Json(const char * buffer) : 
	m_iterator(*this)
{
	if(buffer)
	{
		String content(buffer);
		unserialize(content);
	}
}

/** Destructor */
Json::~Json()
{
	delete m_json;
}

/** Move constructor - transfer ownership of data */
Json::Json(Json&& other) noexcept
	: m_json(nullptr), m_iterator(*this)
{
	// Transfer ownership of the ItemCollection pointer
	m_json = other.m_json;
	other.m_json = nullptr;
	
	// Note: m_iterator is already initialized to point to this Json object
}

/** Move assignment operator - transfer ownership of data */
Json& Json::operator=(Json&& other) noexcept
{
	if (this != &other)
	{
		// Clean up existing data
		delete m_json;
		
		// Transfer ownership of the ItemCollection pointer
		m_json = other.m_json;
		other.m_json = nullptr;
		
		// m_iterator already references this object (no need to rebuild)
	}
	return *this;
}

/** Provides read access to element at the given index in the vector. */
JsonIterator Json::operator[](int index)
{
	return m_iterator[index];
}

/** Provides read access to element at the given index in the vector. */
JsonIterator Json::operator[](const char * key)
{
	return m_iterator[key];
}

/** Clear the content of json */
void Json::clear()
{
	delete m_json;
	m_json = 0;
}

/** Skip blank character */
wchar_t Json::skip_blank(TextStream & content)
{
	wchar_t result = '\0';
 	wchar_t character = ' ';
	do
	{
		// Skip white space
		while(character)
		{
			character = content.read_char();
			if(character > ' ')
			{
				uint32_t of = content.offset();
				content.offset(content.offset()-1);
				break;
			}
		}

		// If comment
		if(character == '#' || character == ';')
		{
			while(character)
			{
				character = content.read_char();
				if(character == '\n')
				{
					break;
				}
			}
		}
		// If comment
		else if(character == '/')
		{
			content.read_char();
			character = content.read_char();

			// Single line comment
			if(character == '/')
			{
				while(character)
				{
					character = content.read_char();
					if(character == '\n')
					{
					//	content.offset(content.offset()-1);
						break;
					}
				}
			}
			// Multiline comment
			else if(character == '*')
			{
				while(character)
				{
					character = content.read_char();
					if(character == '*')
					{
						character = content.read_char();
						if(character == '/')
						{
							character = ' ';
							//content.offset(content.offset()-1);
							break;
						}
					}
				}
			}
			else
			{
				// Bad comment detected
				character = Utf8::not_a_char;
				break;
			}
		}
		else
		{
			// Not a blank character can stop parsing
			break;
		}
	}
	while (character <= ' ' && character);

	result = character;

	return result;
}

/** Choose item type */
Item * Json::choose_type(TextStream & content)
{
	Item * result = 0;
	wchar_t character = skip_blank(content);

	switch(character)
	{
	case 'n':case 'N':
		result = new ItemNull;
		break;

	case 'f':case 'F': case 't':case 'T':
		result = new ItemBool;
		break;

	case '\"':case '\'':
		result = new ItemString;
		break;

	case '[':
		result = new ItemArray;
		break;

	case '{':
		result = new ItemObject;
		break;

	case '\0':
		break;

	default:
		if (character == '-' || (character >= '0' && character <= '9'))
		{
			result = new ItemInt;
		}
		break;
	}

	if (result)
	{
		if(result->unserialize(content) == false)
		{
			delete result;
			result = 0;
		}
	}
	return result;
}

/** Unserialize from json */
void Json::unserialize(const char *content)
{
	if (content)
	{
		String data(content);
		unserialize(data);
	}
}

/** Unserialize from json */
void Json::unserialize(TextStream & content)
{
	Item * item = 0;
	bool result = true;

	// If json already existing
	if(m_json)
	{
		// remove existing
		delete m_json;
		m_json = 0;
	}
	content.offset(0);

	// Create type
	item = choose_type(content);
	if(item)
	{
		// Ignore all blank
		wchar_t character = Json::skip_blank(content);

		// If the end of content reached
		if(character == 0)
		{
			m_json = dynamic_cast<ItemCollection*>(item);
		}
		else
		{
			// Problem another data in file
			delete item;
			result = false;
		}
	}
	else
	{
		// The file is empty
		result = false;
	}

	if (result == false)
	{
		throw_error(content);
	}
}

/** Serialize to json */
void Json::serialize(TextStream & out, int32_t indent)
{
	out.clear();
	if(m_json)
	{
		m_json->serialize(out, indent);
	}
}

/** Update the line and column for where the error is located */
void Json::throw_error(TextStream & content)
{
	JsonUnserializeError err;
	uint32_t offset_parse = 0;
	wchar_t character;
	err.m_line = 1;
	err.m_column = 1;
	uint32_t offset = content.offset();
	content.offset(0);
	do
	{
		character = content.read_char();
		if (character == '\n')
		{
			err.m_line ++;
			err.m_column = 1;
		}
		else if (character)
		{
			err.m_column ++;
		}
	}
	while (character && content.offset() < offset);
	throw err;
}

/** Get the count of items in the current node */
int32_t Json::count()
{
	if (m_json)
	{
		return m_json->count();
	}
	return 0;
}