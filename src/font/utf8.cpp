#include "berialdraw_imp.hpp"

using namespace berialdraw;

wchar_t Utf8::to_int(const char * utf8String)
{
	uint32_t char_size;
	(void)(char_size);
	return read(utf8String, char_size);
}

wchar_t Utf8::read(const char * utf8String, uint32_t & char_size)
{
	wchar_t res = 0;
	uint32_t len = 0;
	if (utf8String)
	{
		if ((uint8_t)*utf8String <= 0x7F) 
		{
			res = (wchar_t)(*utf8String);
			len = 1;
		}
		else
		{
			if ((((uint8_t)*utf8String) & 0xE0) == 0xC0)
			{
				res = (wchar_t)(((uint8_t)*utf8String) & 0x1F);
				len = 2;
			}
			else if ((((uint8_t)*utf8String) & 0xF0) == 0xE0)
			{
				res = (wchar_t)(((uint8_t)*utf8String) & 0x0F);
				len = 3;
			}
			else if ((((uint8_t)*utf8String) & 0xF8) == 0xF0)
			{
				res = (wchar_t)(((uint8_t)*utf8String) & 0x07);
				len = 4;
			}
			else if ((((uint8_t)*utf8String) & 0xFC) == 0xF8)
			{
				res = (wchar_t)(((uint8_t)*utf8String) & 0x03);
				len = 5;
			}
			else if ((((uint8_t)*utf8String) & 0xFE) == 0xFC)
			{
				res = (wchar_t)(((uint8_t)*utf8String) & 0x01);
				len = 6;
			}
			else
			{
				len = 1;
				res = Utf8::not_a_char;
			}

			utf8String++;

			if (*utf8String && res != Utf8::not_a_char)
			{
				uint32_t i;
				for (i=0; i < len-1; ++i )
				{
					if ( !utf8String[i] || (((const unsigned char *)utf8String)[i] & 0xC0) != 0x80 )
					{
						res = Utf8::not_a_char;
						len = i;
						break;
					}
					res = (res<<6) | (((const unsigned char *)utf8String)[i] & 0x3F);
				}
			}
		}
	}
	else
	{
		res = Utf8::not_a_char;
	}

	char_size = len;

	return res;
}

bool Utf8::is_correct(const char * str)
{
	bool res = true;

	if (str)
	{
		const char * ptr = str;

		while (*ptr != 0)
		{
			if ((unsigned char)*ptr > 0x7F)
			{
				uint32_t char_size;
				if (Utf8::read(ptr, char_size) == Utf8::not_a_char)
				{
					res = false;
					break;
				}
				ptr += char_size;
			}
			else
			{
				ptr ++;
			}
		}
	}
	else
	{
		res = false;
	}
	return res;
}

uint32_t Utf8::write (wchar_t wide_char, char * utf8String, uint32_t size)
{
	uint32_t res = 0;

	if (utf8String)
	{
		uint32_t length = Utf8::length(wide_char);

		if (size >= length)
		{
			memset(utf8String, 0, size);

			switch(length)
			{
			case 1: *utf8String++ = (uint8_t) wide_char; break;
			case 2: *utf8String++ = (uint8_t) (0xC0 | (wide_char >>  6)); break;
			case 3: *utf8String++ = (uint8_t) (0xE0 | (wide_char >> 12)); break;
#ifndef WIN32
			case 4: *utf8String++ = (uint8_t) (0xF0 | (wide_char >> 18)); break;
			case 5: *utf8String++ = (uint8_t) (0xF8 | (wide_char >> 24)); break;
			case 6: *utf8String++ = (uint8_t) (0xFC | (wide_char >> 30)); break;
#endif
			default: res = 0; break;
			}
			
			switch(length)
			{
#ifndef WIN32
			case 6: *utf8String++ = (uint8_t) (0x80 | ((wide_char >> 24) & 0x3F));
			case 5: *utf8String++ = (uint8_t) (0x80 | ((wide_char >> 18) & 0x3F));
			case 4: *utf8String++ = (uint8_t) (0x80 | ((wide_char >> 12) & 0x3F));
#endif
			case 3: *utf8String++ = (uint8_t) (0x80 | ((wide_char >>  6) & 0x3F));
			case 2: *utf8String++ = (uint8_t) (0x80 |  (wide_char        & 0x3F));
			}
			res = length;
		}
	}
	return res;
}

uint32_t Utf8::capacity(wchar_t * str)
{
	uint32_t res = 0;
	if (str)
	{
		while (*str)
		{
			res += Utf8::length(*str++);
		}
		res += 1;
	}
	return res;
}

uint32_t Utf8::length(wchar_t wide_char)
{
	uint32_t res = 0;

	if      (wide_char <= 0x7F)       {res = 1;}
	else if (wide_char <= 0x7FF)      {res = 2;}
	else if (wide_char <= 0xFFFF)     {res = 3;}
	else if (wide_char <= 0x1FFFFF)   {res = 4;}
	else if (wide_char <= 0x3FFFFFF)  {res = 5;}
	else if (wide_char <= 0x7FFFFFFF) {res = 6;}
	return res;
}

/** Gets the length in bytes of a begin byte when it is encoded utf8
@param begin byte
@return length in bytes when it is encoded utf8 */
int Utf8::length(char begin_)
{
	int result = -1;
	unsigned char begin = (unsigned char)begin_;
	if (begin <= 0x7F) 
	{
		result = 1;
	}
	else if ((begin & 0xE0) == 0xC0)
	{
		result = 2;
	}
	else if ((begin & 0xF0) == 0xE0)
	{
		result = 3;
	}
	else if ((begin & 0xF8) == 0xF0)
	{
		result = 4;
	}
	else if ((begin & 0xFC) == 0xF8)
	{
		result = 5;
	}
	else if ((begin & 0xFE) == 0xFC)
	{
		result = 6;
	}
	return result;
}

uint32_t Utf8::count(const char * string)
{
	uint32_t res = 0;
	uint32_t size_char;
	uint32_t pos = 0;
	wchar_t character;
	do
	{
		character = read(&string[pos], size_char);
		if(character == 0 || character == Utf8::not_a_char)
		{
			break;
		}
		res += 1;
		pos += size_char;
	}
	while(character!= 0 && character != Utf8::not_a_char);

	return res;
}


