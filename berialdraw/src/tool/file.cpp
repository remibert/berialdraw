#include "berialdraw_imp.hpp"

using namespace berialdraw;

String File::m_resource_dir = "";

File::File()
{
}

File::~File()
{
	close();
}

int File::open(const char *pathname, const char *mode)
{
	String full_name;

	if (m_resource_dir == "")
	{
		full_name = pathname;
	}
	else
	{
		full_name = m_resource_dir + "/";
		full_name += pathname;
	}
	m_file = fopen(full_name.c_str(), mode);

	if (m_file)
	{
		return 0;
	}
	return -1;
}

int File::close()
{
	if (m_file)
	{
		fclose(m_file);
		m_file = 0;
		return 0;
	}
	return -1;
}

int File::read(void *ptr, uint32_t size)
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

int File::write(const void *ptr, uint32_t size)
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

long File::tell()
{
	if (m_file)
	{
		return ftell(m_file);
	}
	return -1;
}

int File::seek(long offset, int whence)
{
	if (m_file)
	{
		return fseek(m_file,offset,whence);
	}
	return -1;
}

uint32_t File::size()
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
uint32_t File::write_char(wchar_t character)
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

uint32_t File::write_string(const char * buffer)
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
void File::clear()
{
}

/** Get the current offset in buffer */
uint32_t File::offset()
{
	return tell();
}

/** Set the offset in buffer */
void File::offset(uint32_t val)
{
	seek((long)val, SEEK_SET);
}

/** Read the wide character next
@return the wide character read or null if it ends */
wchar_t File::read_char()
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
char * File::tmp_alloc(uint32_t length)
{
	return new char[length];
}

/** Deallocate temporary string
@param tmp allocated temporary pointer 
@param length string length to allocate */
void File::tmp_dealloc(char * tmp, uint32_t length)
{
	write((void*)tmp, length);
	delete[] tmp;
}

/** Indicates if the files exists
@param file_name Name of the file.
@return True if file existing. */
bool File::exists(const char* file_name)
{
	String full_name;
	if (m_resource_dir == "")
	{
		full_name = file_name;
	}
	else
	{
		full_name = m_resource_dir + "/";
		full_name += file_name;
	}

	return bd_file_exists(full_name.c_str());
}

/** Get the resource directory */
const String & File::resource_dir()
{
	return m_resource_dir;
}

/** Set the resource directory
@param dir resource directory */
void File::resource_dir(const String & dir)
{
	m_resource_dir = dir;
}

