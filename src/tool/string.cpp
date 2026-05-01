#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Empty string */
const String String::empty("");

/** Compute the reserved size */
void String::alloc(uint32_t change_size)
{
	uint32_t size = change_size+1;
	uint32_t capacity = 0;

	// Compute the capacity length
	if (size > 16)
	{
		for(uint32_t i = 4; i < 32 && capacity < size; i++)
		{
			capacity = (uint32_t)(1) << i;
		}
	}
	else
	{
		capacity = 16;
	}

	// If the capacity size must be changed
	if (capacity != m_capacity)
	{
		char * string = new char[capacity]();

		// Copy existing string
		if (m_string)
		{
			memcpy(string, m_string, m_size);
			delete[] m_string;
		}
		m_string = string;
		m_capacity = capacity;

		// Add string terminator
		m_string[m_capacity-1]   = '\0';
	}
}

String::String()
{
	append("");
}

String::String(const String & string)
{
	append(string.m_string);
}

String::String(String&& string) noexcept
	: m_string(string.m_string), m_capacity(string.m_capacity),
	  m_size(string.m_size), m_offset(string.m_offset)
{
	string.m_string = nullptr;
	string.m_capacity = 0;
	string.m_size = 0;
	string.m_offset = 0;
}

String::String(const char * string)
{
	if(string)
	{
		append(string);
	}
	else
	{
		append("");
	}
}

String::String(const char * string, size_t length)
{
	if(string && length > 0)
	{
		// Create a temporary null-terminated string
		char* temp = new char[length + 1];
		memcpy(temp, string, length);
		temp[length] = '\0';
		append(temp);
		delete[] temp;
	}
	else
	{
		append("");
	}
}

String& String::operator=(const String& other)
{
	if (this != &other)
	{
		if(m_string)
		{
			delete m_string;
		}
		m_string = 0;
		m_capacity = 0;
		m_size = 0;
		m_offset = 0;
		append(other);
	}
	return *this;
}

String& String::operator=(const char * other)
{
	if(m_string)
	{
		delete m_string;
	}
	m_string = 0;
	m_capacity = 0;
	m_size = 0;
	m_offset = 0;
	append(other);
	return *this;
}

// Assign operator redefinition
String& String::operator=(wchar_t value)
{
	char buffer[7]={0,0,0,0,0,0,0};
	Utf8::write(value, buffer, sizeof(buffer));
	return operator=(buffer);
}

String& String::operator=(String&& other) noexcept
{
	if (this != &other)
	{
		if(m_string)
		{
			delete m_string;
		}
		m_string = other.m_string;
		m_capacity = other.m_capacity;
		m_size = other.m_size;
		m_offset = other.m_offset;
		
		other.m_string = nullptr;
		other.m_capacity = 0;
		other.m_size = 0;
		other.m_offset = 0;
	}
	return *this;
}

String::~String()
{
	if(m_string)
	{
		delete m_string;
		m_string = 0;
	}
}

void String::append(const char * string)
{
	if(Utf8::is_correct(string))
	{
		alloc(m_size + (uint32_t)strlen(string));
		strcat(m_string, string);
		m_size = (uint32_t)strlen(m_string);
	}
}

/** Allocate temporary string
@param length string length to allocate
@return allocated temporary pointer */
char * String::tmp_alloc(uint32_t length)
{
	alloc(length + m_size);
	return &m_string[m_size];
}

/** Deallocate temporary string
@param tmp allocated temporary pointer 
@param length string length to allocate */
void String::tmp_dealloc(char * tmp, uint32_t length)
{
	m_size = length+m_size;
}

void String::vprint(const char *format, va_list args)
{
	clear();
	write_format(format, args);
}

void String::print(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vprint(format, args);
	va_end(args);
}


uint32_t String::write_string(const char * buffer)
{
	append(buffer);
	return (uint32_t)strlen(buffer);
}

// Write character into string with append
uint32_t String::write_char(wchar_t character)
{
	int result = -1;
	char buffer[10];
	uint32_t length = Utf8::write(character, buffer, sizeof(buffer));
	if (length > 0)
	{
		append(buffer);
		result = (int)length;
	}
	else
	{
		result = 0;
	}
	return result;
}

// Get the wide character at the index
wchar_t String::get(int32_t index) const
{
	uint32_t pos = 0;
	return offset(index, pos);
}


/** Get a slice of string 
@param start start position
@param end  end position
@param part slice extracted */
void String::slice(int32_t start, int32_t end, String & part) const
{
	uint32_t first = 0;
	uint32_t last  = 0;

	if (end == 0 && start < 0)
	{
		end = m_size;
	}

	offset(start, first);
	offset(end,   last);

	part = "";

	if (first < last)
	{
		part.alloc(last-first);
		memcpy(part.m_string, &m_string[first], last-first);
		part.m_size = last-first;
	}
}


/** Remove a slice of string 
@param start start position
@param end  end position */
void String::remove(int32_t start, int32_t end)
{
	if (m_size > 0)
	{
		uint32_t first = 0;
		uint32_t last  = 0;

		if (end == 0 && start != 0)
		{
			end = m_size;
		}

		offset(start, first);
		offset(end,   last);

		if (first < last)
		{
			uint32_t length = (uint32_t)strlen(m_string);
			if (last > length)
			{
				last = length;
			}
			memmove(&m_string[first], &m_string[last], length-last+1);
			m_size = m_size - (last-first);
		}
	}
}


/** Find string in the string */
int32_t String::find(const char * str, int32_t pos) const
{
	int32_t result = INT32_MAX;
	if (str && str[0] != '\0' && m_string)
	{
		int32_t index = pos;
		uint32_t byte_index = 0;

		offset(pos, byte_index);

		while (m_string[byte_index] != '\0')
		{
			const char* s1 = &m_string[byte_index];
			const char* s2 = str;

			while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2)
			{
				s1++;
				s2++;
			}

			if (*s2 == '\0')
			{
				result = index;
				break;
			}

			if ((unsigned char)m_string[byte_index] < 0x80)
			{
				byte_index++;
			}
			else
			{
				while ((unsigned char)m_string[byte_index] >= 0x80 && (unsigned char)m_string[byte_index] < 0xC0)
				{
					byte_index++;
				}
				byte_index++;
			}
			index++;
		}
	}
	return result;
}


/** Find string in the string in reverse
@param str string to search
@param start start index in the string
@return index in the string where the search string was found or INT32_MAX if not found */
int32_t String::rfind(const char * str, int32_t pos) const
{
	int32_t result = INT32_MAX;
	if (str && str[0] != '\0')
	{
		uint32_t byte_index = 0;
		offset(pos, byte_index);

		int last_char_index = byte_index - 1;

		int index = pos-1;

		while (last_char_index >= 0)
		{
			const char* s1 = &m_string[last_char_index];
			const char* s2 = str;
			int temp_byte_index = last_char_index;

			while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2)
			{
				s1++;
				s2++;
			}

			if (*s2 == '\0')
			{
				result = index;
				break;
			}

			last_char_index--;

			while (last_char_index >= 0 && (unsigned char)m_string[last_char_index] >= 0x80 && (unsigned char)m_string[last_char_index] < 0xC0)
			{
				last_char_index--;
			}

			index--;
		}
	}
	return result;
}


wchar_t String::offset(int32_t index, uint32_t & pos) const
{
	wchar_t result = 0;
	wchar_t character = 0;
	uint32_t char_width;
	bool success = false;

	if (index < 0)
	{
		uint32_t c = count();
		if (abs(index) <= (int)c)
		{
			index = (int32_t)(c+index);
		}
	}

	if(index >= 0)
	{
		character = 'a';
		for(int32_t i = 0; character != 0 && character != Utf8::not_a_char ; i++)
		{
			character = Utf8::read(&m_string[pos],char_width);
			if(index == i)
			{
				success = true;
				break;
			}
			else
			{
				pos += char_width;
			}
		}
	}

	if(success)
	{
		result = character;
	}
	else
	{
		result = Utf8::not_a_char;
	}
	return result;
}

// Insert character at position
void String::insert(wchar_t character, int32_t index)
{
	char buffer[7]={0,0,0,0,0,0,0};
	Utf8::write(character,buffer, sizeof(buffer));
	insert(buffer, index);
}

// Insert string at position
void String::insert(const char * string, int32_t index)
{
	uint32_t pos = 0;
	
	if(Utf8::is_correct(string))
	{
		if (offset(index, pos) != Utf8::not_a_char)
		{
			uint32_t len = (uint32_t)strlen(string);
			alloc(size() + len);
			memmove(&m_string[pos+len], &m_string[pos], m_size-pos+1);
			m_size += len;
			memcpy(&m_string[pos], string, len);
		}
	}
}

// Insert string at position
void String::insert(const String & string, int32_t index)
{
	insert(string.m_string, index);
}

// Replace character at position
void String::replace(wchar_t character, int32_t index)
{
	uint32_t pos = 0;
	wchar_t old_char = offset(index, pos);
	if (old_char != Utf8::not_a_char && old_char != '\0')
	{
		uint32_t old_len = Utf8::length(old_char);
		uint32_t new_len = Utf8::length(character);
		alloc(size() + (old_len > new_len ? old_len : new_len));
		memmove(&m_string[pos+new_len], &m_string[pos+old_len], m_size-pos-old_len+1);
		m_size = m_size -old_len +new_len;
		Utf8::write(character,&(m_string[pos]),new_len);
	}
}

// Replace all occurrences of a substring with another substring
void String::replace(const char * searched, const char * replaced)
{
	if (!searched || searched[0] == '\0')
		return;

	if (!replaced)
		replaced = "";

	int32_t pos = 0;
	int32_t found;
	uint32_t search_length = Utf8::count(searched);

	while (true)
	{
		found = find(searched, pos);
		if (found == INT32_MAX)
			break;

		// Remove the searched string
		remove(found, found + (int32_t)search_length);

		// Insert the replacement string
		insert(replaced, found);

		// Continue after the inserted string
		pos = found + (int32_t)Utf8::count(replaced);
	}
}

// Remove character at position
void String::remove(int32_t index)
{
	uint32_t pos = 0;
	wchar_t character = offset(index, pos);
	if(character != Utf8::not_a_char && character != 0)
	{
		uint32_t len = Utf8::length(character);
		memmove(&m_string[pos], &m_string[pos+len], m_size-pos-len+1);
		m_size -= len;
	}
}

// Compare if equal string
bool String::operator==(const String& other) const
{
	return (*this == other.m_string);
}

// Compare if equal string buffer
bool String::operator==(const char * other) const
{
	if(m_string && other)
	{
		if(strcmp(m_string,other) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(m_string == 0 && other == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

// Compare if different string
bool String::operator!=(const String& other) const
{
	return !(*this == other.m_string);
}
		
// Compare if differ string buffer
bool String::operator!=(const char * other) const
{
	return !(*this == other);
}

String String::operator+(const String &other) const 
{
	String res(*this);
	res.append(other);
	return res;
}

// Concatenate two string
String String::operator+(const char * other) const
{
	String res(*this);
	res.append(other);
	return res;
}

// Concatenate two string
String String::operator+(wchar_t other) const
{
	String res(*this);
	res.append(other);
	return res;
}

String& String::operator+=(const String &other)
{
	this->append(other);
	return *this;
}

// Append to current string
String& String::operator+=(const char * other)
{
	this->append(other);
	return *this;
}

// Append to current string
String & String::operator+=(wchar_t other)
{
	this->append(other);
	return *this;
}

// Get the quantity of wide character into the string
uint32_t String::count() const
{
	return Utf8::count(m_string);
}

uint32_t String::size() const
{
	return m_size;
}

void String::clear()
{
	m_size = 0;
	if(m_string)
	{
		m_string[0] = 0;
	}
}

// Append wide char into the current string
void String::append(wchar_t character)
{
	char buffer[7]={0,0,0,0,0,0,0};
	Utf8::write(character,buffer, sizeof(buffer));
	append(buffer);
}

void String::append(const String & other)
{
	append(other.m_string);
}

const char *String::c_str() const
{
	return m_string;
}

// Cast to char point
String::operator const char *() const
{
	return m_string;
}

// To integer
bool String::to_int(int32_t & integer) const
{
	int temp;
	bool result = String::to_int(m_string, temp);
	integer = temp;
	return result;
}

// To integer
bool String::to_int(int64_t & integer) const
{
	long long temp;
	bool result = String::to_int(m_string, temp);
	integer = temp;
	return result;
}

// To integer 
bool String::to_int(const char * str, int & integer)
{
	bool result = false;
	char* endptr;
	int var = (int)strtolong(str,&endptr,0);
	integer = 0;
	if(endptr)
	{
		if(*endptr=='\0')
		{
			result = true;
			integer = var;
		}
	}
	return result;
}

// To integer 
bool String::to_int(const char * str, long long & integer)
{
	bool result = false;
	char* endptr;
	long long var = (long long)strtolong(str,&endptr,0);
	integer = 0;
	if(endptr)
	{
		if(*endptr=='\0')
		{
			result = true;
			integer = var;
		}
	}
	return result;
}

/** Get the current offset in buffer */
uint32_t String::offset()
{
	return m_offset;
}

/** Set the offset in buffer */
void String::offset(uint32_t val)
{
	if (val < m_size)
	{
		m_offset = val;
	}
	else
	{
		m_offset = m_size;
	}
}

// Get the wide character next
wchar_t String::read_char()
{
	wchar_t result = 0;

	if (m_offset <= m_size && m_string)
	{
		result = (unsigned char)m_string[m_offset];
		if (result > 0x7f)
		{
			uint32_t char_width ;
			wchar_t character = Utf8::read(&m_string[m_offset],char_width);
			if(character != 0 && character != Utf8::not_a_char)
			{
				result = character;
				m_offset += char_width;
			}
		}
		else
		{
			m_offset++;
		}
	}
	return result;
}

/** Convert string to uppercase (UTF-8 aware)
@return Reference to this String */
String& String::to_upper()
{
	String result;
	uint32_t pos = 0;

	while (pos < m_size)
	{
		wchar_t character = (unsigned char)m_string[pos];
		if (character > 0x7f)
		{
			uint32_t char_width;
			character = Utf8::read(&m_string[pos], char_width);
			if (character != 0 && character != Utf8::not_a_char)
			{
				character = (wchar_t)std::towupper(character);
				result.append(character);
				pos += char_width;
				continue;
			}
		}
		else
		{
			character = (wchar_t)std::toupper((unsigned char)m_string[pos]);
			result.append(character);
			pos++;
		}
	}

	*this = result;
	return *this;
}

/** Convert string to lowercase (UTF-8 aware)
@return Reference to this String */
String& String::to_lower()
{
	String result;
	uint32_t pos = 0;

	while (pos < m_size)
	{
		wchar_t character = (unsigned char)m_string[pos];
		if (character > 0x7f)
		{
			uint32_t char_width;
			character = Utf8::read(&m_string[pos], char_width);
			if (character != 0 && character != Utf8::not_a_char)
			{
				character = (wchar_t)std::towlower(character);
				result.append(character);
				pos += char_width;
				continue;
			}
		}
		else
		{
			character = (wchar_t)std::tolower((unsigned char)m_string[pos]);
			result.append(character);
			pos++;
		}
	}

	*this = result;
	return *this;
}



/** Search for variable pattern $(name) starting from position
@param var String to store the variable name (without $())
@param pos Starting position to search from
@return Index where the variable was found or INT32_MAX if not found */
int32_t String::search_var(String & var, int32_t pos) const
{
	int32_t result = INT32_MAX;
	var.clear();
	
	int32_t start_pos = find("$(", pos);
	if (start_pos != INT32_MAX)
	{
		// Search for the closing )
		int32_t end_pos = find(")", start_pos + 2);
		if (end_pos != INT32_MAX)
		{
			// Extract the variable name between $( and )
			slice(start_pos + 2, end_pos, var);
			result = start_pos;
		}
	}
	
	return result;
}

/** Convert filename to UTF-8, handling different encodings (UTF-8, CP437, etc.) */
String String::convert_filename_encoding(const char* src)
{
	String result;

	if (src != nullptr)
	{
		if (Utf8::is_correct(src))
		{
			// Already UTF-8, just return
			result = String(src);
		}
		else
		{
			// Not UTF-8, convert from CP437
			// CP437 to Unicode mapping for characters > 127
			static const uint16_t cp437_table[128] = {
				0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7,
				0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
				0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9,
				0x00FF, 0x00D6, 0x00DC, 0x00A2, 0x00A3, 0x00A5, 0x20A7, 0x0192,
				0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA,
				0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
				0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
				0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
				0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F,
				0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
				0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B,
				0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
				0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4,
				0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
				0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248,
				0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0
			};

			while (*src)
			{
				unsigned char ch = (unsigned char)*src;

				if (ch < 128)
				{
					// ASCII character - append as is
					result.append((char)ch);
				}
				else
				{
					// CP437 extended character - convert to Unicode then to UTF-8
					uint16_t unicode = cp437_table[ch - 128];
					result.append((wchar_t)unicode);
				}

				src++;
			}
		}
	}

	return result;
}


