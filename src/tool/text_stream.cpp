#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Repeat wide character and write to the end of the current text stream
@param character wide character 
@param count number of wide character
@return the number of bytes written, or 0 if an error occurred */
uint32_t TextStream::pad(wchar_t character, int32_t count)
{
	uint32_t result = 0;
	uint32_t res;
	if (count > 0)
	{
		for (int i = 0; i < count; i++)
		{
			res = write_char(character);
			if (res == 0)
			{
				break;
			}
			result += res;
		}
	}
	return result;
}

/** Reads the following name (the name consists of a sequence of letters a to z, A to Z, 0 to 9 and _)
@param name name to read
@return the number of bytes written, or 0 if an error occurred */
uint32_t TextStream::read_name(String & name)
{
	wchar_t character = 0;
	int i = 0;

	name.clear();
	while (true)
	{
		character = read_char();
		if ((character >= 'a' && character <= 'z') ||
			(character >= 'A' && character <= 'Z') ||
			((character >= '0' && character <= '9') && i > 0)|| 
			(character == '_'))
		{
			name += character;
		}
		else
		{
			offset(offset()-1);
			break;
		}
		i++;
	}
	return name.size();
}

/** Reads the following integer
@param integer integer read
@param precise if true, the string can contain a period and the result is returned as a value multiplied by 64
@return the number of bytes read, or 0 if an error occurred */
uint32_t TextStream::read_integer(int64_t & value, bool & accurate)
{
	uint32_t result = 0;
	wchar_t sign = '+';
	int     base = 10;
	wchar_t current;
	int64_t multi = 0;
	int64_t temp = 0;
	int64_t val = 0;

	value = 0;
	current = read_char();
	if (current)
	{
		result ++;
		while (current == ' ' || current == '\t' || current == '\v' || current == '\f' || current == '\r' || current == '\n')
		{
			current = read_char(); result ++;
		}

		if (current == '-' || current == '+')
		{
			sign = current;
			current = read_char(); result ++;
		}

		if (current == '0')
		{
			current = read_char(); result ++;

			// hexadecimal
			if (current == 'x' || current == 'X')
			{
				base = 16;
				current = read_char(); result ++;
			}
			// octal
			else if (current == 'o' || current == 'O')
			{
				base = 8;
				current = read_char(); result ++;
			}
			// binary
			else if (current == 'b' || current == 'B')
			{
				base = 2;
				current = read_char(); result ++;
			}
			// float
			else if (current == '.')
			{
				base = 10;
				multi = 1;
				current = read_char(); result ++;
			}
			// decimal
			else
			{
				base = 10;
			}
		}
		else if (current == '\\')
		{
			current = read_char(); result ++;

			// unicode (hexadecimal)
			if (current == 'u' || current == 'U')
			{
				base = 16;
				current = read_char(); result ++;
			}
			// octal
			else if (current == 'o' || current == 'O')
			{
				base = 8;
				current = read_char(); result ++;
			}
			// hexadecimal
			else if (current == 'x' || current == 'X')
			{
				base = 16;
				current = read_char(); result ++;
			}
			// binary
			else if (current == 'b' || current == 'B')
			{
				base = 2;
				current = read_char(); result ++;
			}
			else
			{
				base = 10;
				current = read_char(); result ++;
			}
		}
		else if (base == 0)
		{
			// decimal
			base = 10;
		}

		while (current)
		{
			if (current >= '0' && current <= '9')
			{
				int digit = (current - 0x30);
				if (digit >= base) 
					break;
				value = (value * base) + digit;
				multi *= 10;
			}
			else if (current >= 'A' && current <= 'Z')
			{
				int digit = (current - 55);
				if (digit >= base) 
					break;
				value = (value * base) + digit;
			}
			else if (current >= 'a' && current <= 'z')
			{
				int digit = (current - 87);
				if (digit >= base) 
					break;
				value = (value * base) + digit;
			}
			else if (current == '.' && base == 10)
			{
				multi = 1;
			}
			else
			{
				break;
			}

			current = read_char(); result ++;
		}

		if (multi != 0 && base == 10)
		{
			value = (value * 64)/multi;
			accurate = true;
		}
		else
		{
			accurate = false;
		}

		if (value)
		{
			if (sign == '-')
			{
				value = -value;
			}
		}
		if (result >= 1)
		{
			result --;
			offset(offset()-1);
		}
	}
	return result;
}

/** Write with sprintf format to the end of the current text stream
@param format sprintf format
@param args va args parameter 
@return the number of bytes written, or 0 if an error occurred */
uint32_t TextStream::write_format(const char *format, va_list args)
{
	uint32_t result = 0;
	if (format)
	{
		if(strchr(format,'%') != NULL)
		{
			uint32_t length;
			va_list argslen;
	
			va_copy(argslen, args);
			length = vsnprintf(NULL,0, format,argslen);
			va_end(argslen);
			
			char * buffer = tmp_alloc(length+1);
			if (buffer)
			{
				vsnprintf(buffer, length+1, format, args);
				tmp_dealloc(buffer, length);
				result = length;
			}
		}
		else
		{
			result = write_string(format);
		}
	}
	return result;
}

/** Write with sprintf format to the end of the current text stream
@param format sprintf format
@param other parameter 
@return the number of bytes written, or 0 if an error occurred */
uint32_t TextStream::write_format(const char *format, ...)
{
	uint32_t result = 0;
	va_list args;
	va_start(args, format);
	result = write_format(format, args);
	va_end(args);
	return result;
}
