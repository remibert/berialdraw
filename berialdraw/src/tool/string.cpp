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



/** Search for variable pattern ${name} starting from position
@param var String to store the variable name (without ${})
@param pos Starting position to search from
@return Index where the variable was found or INT32_MAX if not found */
int32_t String::search_var(String & var, int32_t pos) const
{
	int32_t result = INT32_MAX;
	var.clear();
	
	int32_t start_pos = find("${", pos);
	if (start_pos != INT32_MAX)
	{
		// Search for the closing }
		int32_t end_pos = find("}", start_pos + 2);
		if (end_pos != INT32_MAX)
		{
			// Extract the variable name between ${ and }
			slice(start_pos + 2, end_pos, var);
			result = start_pos;
		}
	}
	
	return result;
}


#ifdef _DEBUG
#include <assert.h>
static void check(const String & str,const char * ref)
{
	assert(strcmp(str.c_str(), ref) == 0);
	assert(strlen(str.c_str()) == str.size());
}

void String::test1()
{
	String str("toto");

	check(str, "toto");
	str.append("hello");
	check(str,"totohello");
	
	for(int i = 0; i < 64; i++)
	{
		str.append("_");
	}

	uint32_t sz = str.size();
	assert(str.size() == 73);

	String str2(str);
	assert(str2.size() == 73);
	
	str = "jjj";
	check(str,"jjj");
	str2 = str;
	check(str2, "jjj");

	str.clear();
	str.print("-%02d-",12434);
	check(str, "-12434-");

	str = "Rémi à la plage";

	assert(str == "Rémi à la plage");
	assert(str != "Rémi à la piscine");

	str2 = str;
	assert(str == str2);
	str2 = "Rémi à la piscine";
	assert(str != str2);

	assert(str.count() == 15);
	assert(str.size() == 17);
	wchar_t character = str.get(1);
	assert(character == 233);

	character = str.get(0);
	assert(character == 'R');
	character = str.get(14);
	assert(character == 'e');
	character = str.get(15);
	assert(character == '\0');
	character = str.get(5);
	assert(character == 224);

	str.write_format("bonjour");
	assert(str == "Rémi à la plagebonjour");
	str.clear();
	str2 = "toto";
	assert(!(str2 == str));
	assert((str2 != str));

	{
		String a;
		String b;
		assert((a == str));
		assert(!(a != str));

		assert(a == b);
		assert(!(a != b));
	}

	{
		str.clear();
		str += "1234";
		str2 = "5678";
		check(str, "1234");
		str = str + str2;
		check(str, "12345678");
		check(str2, "5678");
		str += str2;
		check(str, "123456785678");
		check(str2, "5678");

		str += "901";
		check(str, "123456785678901");

		str = "34";
		str = str +"56";
		check(str, "3456");
	}
}

void String::test2()
{
	{
		String str("123");
		int32_t val = 0;
		assert(str.to_int(val));
		assert(val == 123);
	}

	{
		String str("0x123");
		int32_t val = 0;
		assert(str.to_int(val));
		assert(val == 0x123);
	}
	{
		String str("0b11111111");
		int32_t val = 0;
		assert(str.to_int(val));
		assert(val == 0xFF);
	}
	{
		String str("toto");
		int32_t val = 0;
		assert(str.to_int(val) == false);
	}
	{
		String str("têàtotàoà");
		wchar_t character;

		uint32_t offset = 0;

		for(int i = 0; i<= (int)str.count(); i++)
		{
			character = str.read_char();
			assert(character == str.get(i));
		}
		
		character = str.read_char();
		character = str.read_char();
		character = str.read_char();
		character = str.read_char();
		character = str.read_char();
		character = str.read_char();
		character = str.read_char();
	}
	{
		String str("rémi");
		str.replace('e',1);
		check(str,"remi");
		str.replace(0xE9,0);
		check(str,"éemi");
		str.replace(0xE9,-1);
		check(str,"éemé");
		str.replace('0', 0);
		check(str,"0emé");
		str.replace('1', 1);
		check(str,"01mé");
		str.replace('2', 2);
		check(str,"012é");
		str.replace('3', 3);
		check(str,"0123");
		str.replace('4', 4);
		check(str,"0123");
		str.replace('5', 5);
		check(str,"0123");

		str.replace('a', -1);
		check(str,"012a");
		str.replace('b', -2);
		check(str,"01ba");
		str.replace('c', -3);
		check(str,"0cba");
		str.replace('d', -4);
		check(str,"dcba");
		str.replace('e', -5);
		check(str,"dcba");
		str.replace('f', -6);
		check(str,"dcba");
	}

	{
		String str;
		str.insert("rémi",0);
		check(str,"rémi");
		str.remove(-1);
		check(str,"rém");
		str.remove(-1);
		check(str,"ré");
		str.remove(-1);
		check(str,"r");
		str.remove(-1);
		check(str,"");
		str.remove(-1);
		check(str,"");
	}
	{
		String str;
		str.insert("rémi",0);
		check(str,"rémi");
		str.remove(0);
		check(str,"émi");
		str.remove(0);
		check(str,"mi");
		str.remove(0);
		check(str,"i");
		str.remove(0);
		check(str,"");
		str.remove(0);
		check(str,"");
	}
	{
		String str("êàtotàoà");
		str.remove((int32_t)str.count());
		check(str,"êàtotàoà");
		str.remove(-1);
		check(str,"êàtotào");
		str.remove(0);
		check(str,"àtotào");
		str.remove(2);
		check(str,"àttào");
		str.remove(2);
		check(str,"àtào");
		str.remove(2);
		check(str,"àto");
		str.remove(2);
		check(str,"àt");
		str.remove(2);
		check(str,"àt");
	}
	{
		String str("êàtotàoà");
		uint32_t len = str.count();
		for(int32_t i = (int32_t)len; i >= 0; i--)
		{
			str.insert(0xE9,i);
		}
		check(str,"éêéàétéoétéàéoéàé");
	}
	{
		String str("êàtoto");
		str.insert(0xE9,1);
		check(str,"êéàtoto");
	}
	{
		String str2;
		str2.insert(0xE9,1);
		check(str2,"");
		str2.insert(0xE9,0);
		check(str2,"é");
	}
}

void String::test3()
{
	String s("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	
	String a;
	s.slice(0,0xFFFFFF,a);
	assert(a == "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	s.slice(0,-1,a);
	assert(a == "0123456789ABCDEFGHIJKLMNOPQRSTUVWXY");
	s.slice(5,12,a);
	assert(a == "56789AB");
	s.slice(5,-12,a);
	assert(a == "56789ABCDEFGHIJKLMN");
	s.slice(-5,-12,a);
	assert(a == "");
	s.slice(-12,-5,a);
	assert(a == "OPQRSTU");
	s.slice(100000,1000000,a);
	assert(a == "");
	s.slice(-100000,-100,a);
	assert(a == "");
	s.slice(-5,0,a);
	assert(a == "VWXYZ");
	s.slice(0,5,a);
	assert(a == "01234");
}

void String::test4()
{
	String text("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	text.remove(1,3);
	assert(text == "ADEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	text.remove(-5,-3);
	assert(text == "ADEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuxyz");
	text.remove(30,0);
	assert(text == "ADEFGHIJKLMNOPQRSTUVWXYZabcdef");
	text.remove(0,10);
	assert(text == "MNOPQRSTUVWXYZabcdef");
	text.remove(2,0);
	assert(text == "MN");
	text.remove(0,2);
	assert(text == "");
	text.remove(0,2);
	assert(text == "");
}

void String::test5()
{
	String str("Salut rémi, rémi Comment tu vas. rémi hello");

	int32_t pos = str.find("rémi");
	assert(pos == 6);
	pos = str.find("rémi",++pos);
	assert(pos == 12);
	pos = str.find("rémi",++pos);
	assert(pos == 33);
	pos = str.find("rémi",++pos);
	assert(pos == INT32_MAX);

	pos = -43;
	pos = str.find("rémi",pos);
	assert(pos == -37);
	pos = str.find("rémi",++pos);
	assert(pos == -31);
	pos = str.find("rémi",++pos);
	assert(pos == -10);
	pos = str.find("rémi",++pos);
	assert(pos == INT32_MAX);

	pos = str.find("prout");
	assert(pos == INT32_MAX);

	pos = str.find("");
	assert(pos == INT32_MAX);

	pos = str.find(0);
	assert(pos == INT32_MAX);
}

void String::test6()
{
	String str("Salut rémi, rémi Comment tu vas. rémi hello");

	int32_t pos = str.rfind("rémi");
	assert(pos == -10);
	pos = str.rfind("rémi",--pos);
	assert(pos == -31);
	pos = str.rfind("rémi",--pos);
	assert(pos == -37);

	pos = str.rfind("rémi",--pos);
	assert(pos == INT32_MAX);

	pos = 44;
	pos = str.rfind("rémi",--pos);
	assert(pos == 33);
	pos = str.rfind("rémi",--pos);
	assert(pos == 12);
	pos = str.rfind("rémi",--pos);
	assert(pos == 6);
	pos = str.rfind("rémi",--pos);
	assert(pos == INT32_MAX);

	pos = str.rfind("prout");
	assert(pos == INT32_MAX);

	pos = str.rfind("");
	assert(pos == INT32_MAX);

	pos = str.rfind(0);
	assert(pos == INT32_MAX);
}


void String::test7()
{
	// Test basic replace
	String str("hello world");
	str.replace("world", "universe");
	check(str, "hello universe");

	// Test replace multiple occurrences
	str = "cat bat cat dog cat";
	str.replace("cat", "mouse");
	check(str, "mouse bat mouse dog mouse");

	// Test replace with longer string
	str = "a b c";
	str.replace("b", "hello");
	check(str, "a hello c");

	// Test replace with shorter string
	str = "hello world hello";
	str.replace("hello", "hi");
	check(str, "hi world hi");

	// Test replace with empty string (remove)
	str = "a,b,c,d";
	str.replace(",", "");
	check(str, "abcd");

	// Test replace with UTF-8 characters
	str = "café café café";
	str.replace("café", "thé");
	check(str, "thé thé thé");

	// Test replace when substring not found
	str = "hello world";
	str.replace("xyz", "abc");
	check(str, "hello world");

	// Test replace empty search (should do nothing)
	str = "hello";
	str.replace("", "x");
	check(str, "hello");

	// Test replace with null replaced (should treat as empty)
	str = "a-b-c";
	str.replace("-", "");
	check(str, "abc");

	// Test single character replace
	str = "aaa";
	str.replace("a", "b");
	check(str, "bbb");

	// Test overlapping patterns don't cause infinite loops
	str = "aaaa";
	str.replace("aa", "bb");
	check(str, "bbbb");
}

void String::test8()
{
	// Test basic variable search
	String str("Hello ${name}, welcome!");
	String var;
	int32_t pos = str.search_var(var);
	assert(pos == 6);
	assert(var == "name");

	// Test search with starting position
	str = "Hello ${first}, this is ${second} test";
	pos = str.search_var(var, 0);
	assert(pos == 6);
	assert(var == "first");
	
	pos = str.search_var(var, pos + 1);
	assert(pos == 24);
	assert(var == "second");

	// Test no variable found
	str = "Hello world";
	pos = str.search_var(var);
	assert(pos == INT32_MAX);

	// Test unclosed variable
	str = "Hello ${name world";
	pos = str.search_var(var);
	assert(pos == INT32_MAX);

	// Test variable without opening
	str = "Hello name}";
	pos = str.search_var(var);
	assert(pos == INT32_MAX);

	// Test variable with UTF-8 characters
	str = "Bonjour ${prénom}, bienvenue!";
	pos = str.search_var(var);
	assert(pos == 8);
	assert(var == "prénom");

	// Test multiple variables with UTF-8
	str = "Hello ${éléphant} and ${ours}";
	pos = str.search_var(var, 0);
	assert(pos == 6);
	assert(var == "éléphant");
	
	pos = str.search_var(var, pos + 1);
	assert(pos == 22);
	assert(var == "ours");

	// Test variable at the beginning
	str = "${var} is at start";
	pos = str.search_var(var);
	assert(pos == 0);
	assert(var == "var");

	// Test variable at the end
	str = "This is ${var}";
	pos = str.search_var(var);
	assert(pos == 8);
	assert(var == "var");

	// Test consecutive variables
	str = "${first}${second}";
	pos = str.search_var(var, 0);
	assert(pos == 0);
	assert(var == "first");
	
	pos = str.search_var(var, pos + 1);
	assert(pos == 8);
	assert(var == "second");

	// Test variable with numbers and underscore
	str = "Config: ${var_name_123}";
	pos = str.search_var(var);
	assert(pos == 8);
	assert(var == "var_name_123");

	// Test empty variable name
	str = "Empty: ${}";
	pos = str.search_var(var);
	assert(pos == 7);
	assert(var == "");
}

void String::test()
{
	test8();
	test7();
	test6();
	test5();
	test4();
	test3();
	test2();
	test1();
}
#endif
