#include "berialdraw_imp.hpp"

using namespace berialdraw;

File::File()
{
}

File::~File()
{
	close();
}

int File::open(const char *pathname, const char *mode)
{
	String path(pathname);
	
	// Dispatcher: sélectionne le backend approprié
	if (path.find("zip://", 0) == 0)
	{
		m_file = std::make_unique<ZipFile>();
	}
	else
	{
		m_file = std::make_unique<LocalFile>();
	}
	
	return m_file ? m_file->open(pathname, mode) : -1;
}

int File::close()
{
	return m_file ? m_file->close() : -1;
}

int File::read(void *ptr, uint32_t size)
{
	return m_file ? m_file->read(ptr, size) : -1;
}

int File::write(const void *ptr, uint32_t size)
{
	return m_file ? m_file->write(ptr, size) : -1;
}

long File::tell()
{
	return m_file ? m_file->tell() : -1;
}

int File::seek(long offset, int whence)
{
	return m_file ? m_file->seek(offset, whence) : -1;
}

uint32_t File::size()
{
	return m_file ? m_file->size() : 0;
}

uint32_t File::write_char(wchar_t character)
{
	return m_file ? m_file->write_char(character) : 0;
}

uint32_t File::write_string(const char * buffer)
{
	return m_file ? m_file->write_string(buffer) : 0;
}

void File::clear()
{
	if (m_file) m_file->clear();
}

uint32_t File::offset()
{
	return m_file ? m_file->offset() : 0;
}

void File::offset(uint32_t val)
{
	if (m_file) m_file->offset(val);
}

wchar_t File::read_char()
{
	return m_file ? m_file->read_char() : 0;
}

bool File::exists(const char* file_name)
{
	return LocalFile::exists(file_name);
}

String File::resolve(const String & dir)
{
	return LocalFile::resolve(dir);
}
bool File::match_pattern(const char *pattern, const char *string, bool ignore_case)
{
	if (!pattern || !string)
	{
		return false;
	}
	
	while (*pattern && *string)
	{
		if (*pattern == '*')
		{
			// Handle wildcard
			if (*(pattern + 1) == '\0')
			{
				return true; // * at end matches everything
			}
			
			// Try to match the rest of the pattern
			const char* s = string;
			while (*s)
			{
				if (match_pattern(pattern + 1, s, ignore_case))
				{
					return true;
				}
				s++;
			}
			return false;
		}
		else if (*pattern == '?')
		{
			// ? matches any single character
			pattern++;
			string++;
		}
		else
		{
			char p = *pattern;
			char c = *string;
			
			if (ignore_case)
			{
				p = tolower(p);
				c = tolower(c);
			}
			
			if (p != c)
			{
				return false;
			}
			
			pattern++;
			string++;
		}
	}
	
	// Check if we consumed both strings or pattern ends with *
	return (*pattern == '\0' || (*pattern == '*' && *(pattern + 1) == '\0')) && *string == '\0';
}
char * File::tmp_alloc(uint32_t length)
{
	return m_file ? m_file->tmp_alloc(length) : new char[length];
}

void File::tmp_dealloc(char * tmp, uint32_t length)
{
	if (m_file) m_file->tmp_dealloc(tmp, length);
}

