#include "berialdraw_imp.hpp"

using namespace berialdraw;

LocalFile::LocalFile()
{
}

LocalFile::~LocalFile()
{
	close();
}

int LocalFile::open(const char *path, const char *mode)
{
	String p(path);
	if (UIManager::settings())
	{
		p = UIManager::settings()->resolve(path);
	}

	m_file = fopen(p, mode);

	if (m_file)
	{
		return 0;
	}
	else
	{
		bd_printf("Cannot open file '%s'\n",p.c_str());
	}
	return -1;
}

int LocalFile::close()
{
	if (m_file)
	{
		fclose(m_file);
		m_file = 0;
		return 0;
	}
	return -1;
}

int LocalFile::read(void *ptr, uint32_t size)
{
	int result = -1;
	if (m_file)
	{
		uint32_t res = (uint32_t)fread(ptr,1,size,m_file);
		result = (int)res;
		if (res < size)
		{
			if (feof(m_file) != 0)
			{
				result = -1;
			}
		}
	}
	return result;
}

int LocalFile::write(const void *ptr, uint32_t size)
{
	int result = -1;
	if (m_file)
	{
		uint32_t res = (uint32_t)fwrite(ptr,1,size,m_file);
		result = (int)res;
		if (res < size)
		{
			if (feof(m_file) != 0)
			{
				result = -1;
			}
		}
	}
	return result;
}

long LocalFile::tell()
{
	if (m_file)
	{
		return ftell(m_file);
	}
	return -1;
}

int LocalFile::seek(long offset, int whence)
{
	if (m_file)
	{
		return fseek(m_file,offset,whence);
	}
	return -1;
}

uint32_t LocalFile::size()
{
	uint32_t result = 0;
	long pos = tell();
    if (seek(0, SEEK_END) >= 0)
	{
		result = tell();
		seek(pos, SEEK_SET);
	}
	return result;
}

/** Write a wide character to the end of the current text stream
@param character wide character 
@return the number of bytes written, or 0 if an error occurred */
uint32_t LocalFile::write_char(wchar_t character)
{
	if (character <= 0x7F)
	{
		unsigned char charac = (unsigned char) character;
		return write(&charac, 1);
	}
	else
	{
		char buffer[7]={0,0,0,0,0,0,0};
		uint32_t length = Utf8::write(character,buffer, sizeof(buffer));
		return write((void*)buffer, length);
	}
}

uint32_t LocalFile::write_string(const char * buffer)
{
	uint32_t length = (uint32_t)strlen(buffer);
	if(Utf8::is_correct(buffer))
	{
		write((void*)buffer, length);
	}
	else
	{
		length = 0;
	}
	return length;
}

/** Clear the content of stream */
void LocalFile::clear()
{
}

/** Get the current offset in buffer */
uint32_t LocalFile::offset()
{
	return tell();
}

/** Set the offset in buffer */
void LocalFile::offset(uint32_t val)
{
	seek((long)val, SEEK_SET);
}

/** Read the wide character next
@return the wide character read or null if it ends */
wchar_t LocalFile::read_char()
{
	wchar_t result = 0;
	char buffer[7]={0,0,0,0,0,0,0};

	if (read((void*)&(buffer[0]), 1) == 1)
	{
		int length = Utf8::length(buffer[0]);
		if (length > 1)
		{
			int i;
			for (i = 1; i < length; i++)
			{
				if (read((void*)&(buffer[i]), 1) != 1)
				{
					break;
				}
			}

			if (i == length)
			{
				uint32_t charwidth;
				result = Utf8::read(buffer, charwidth);
			}
		}
		else
		{
			result = (wchar_t)buffer[0];
		}
	}
	return result;
}

/** Allocate temporary string
@param length string length to allocate
@return allocated temporary pointer */
char * LocalFile::tmp_alloc(uint32_t length)
{
	return new char[length];
}

/** Deallocate temporary string
@param tmp allocated temporary pointer 
@param length string length to allocate */
void LocalFile::tmp_dealloc(char * tmp, uint32_t length)
{
	write((void*)tmp, length);
	delete[] tmp;
}

/** Indicates if the files exists
@param file_name Name of the file.
@return True if file existing. */
bool LocalFile::exists(const char* file_name)
{
	String p(file_name);
	if (UIManager::settings())
	{
		p = UIManager::settings()->resolve(file_name);
	}

	return bd_file_exists(p.c_str());
}

/** Select the directory existing in the list and return it */
String LocalFile::resolve(const String & dir)
{
	// Split the input by semicolons and find first existing directory
	String current_path = "";
	int32_t start = 0;
	int32_t end = 0;

	String result = ".";
	if (dir.size() > 0)
	{
		do
		{
			end = dir.find(";",start);	
			dir.slice(start, end, current_path);
			start = end + 1;
		
			// Check if this directory exists
			if (Directory::exists(current_path.c_str()))
			{
				result = current_path;
				break;
			}
		}
		while (end != INT32_MAX);
	}
	return result;
}
