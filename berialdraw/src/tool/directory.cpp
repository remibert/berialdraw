#include "berialdraw_imp.hpp"

using namespace berialdraw;

Directory::Directory(const char * path)
{
	if (path)
	{
		if (open(path))
		{
			first();
		}
	}
}

Directory::~Directory()
{
	close();
}

bool Directory::open(const String& path)
{
	String p(path);
	if (UIManager::settings())
	{
		p = UIManager::settings()->resolve(path);
	}

	if (m_dir)
	{
		bd_dir_close(m_dir);
	}

	m_dir = bd_dir_open(p.c_str());
	if (m_dir == 0)
	{
		bd_printf("Cannot open directory '%s'\n",p.c_str());
	}
	m_directory = path;
	return m_dir != 0;
}

void Directory::close()
{
	bd_dir_close(m_dir);
	m_dir = 0;
}

bool Directory::first()
{
	const char* first_file_name = bd_dir_first(m_dir);
	if (first_file_name)
	{
		m_filename = first_file_name;
		return true;
	}
	return false;
}

bool Directory::next()
{
	const char* next_file_name = bd_dir_next(m_dir);
	if (next_file_name)
	{
		m_filename = next_file_name;
		return true;
	}
	m_filename = "";
	return false;
}

bool Directory::exist()
{
	if (m_filename == "")
	{
		return false;
	}
	return true;
}

size_t Directory::file_size() const
{
	return bd_file_size(m_filename.c_str());
}

bool Directory::is_directory() const
{
	return bd_is_directory(m_filename.c_str());
}

bool Directory::is_file() const
{
	return !is_directory();
}

String Directory::filename() const
{
	return m_filename;
}

String Directory::full_path() const
{
	return m_directory + "/" + m_filename;
}

/** Matches a string against a pattern containing wildcards */ 
bool Directory::match(const char *pattern, bool ignore_case)
{
	return (match_pattern(pattern, m_filename.c_str()));
}

/** Check if a directory exists
@param path directory path to check
@return true if directory exists */
bool Directory::exists(const char* path)
{
	return bd_is_directory(path);
}
