#include "berialdraw_imp.hpp"

using namespace berialdraw;

LocalDirectory::LocalDirectory(const char * path)
{
	if (path)
	{
		if (open(path))
		{
			first();
		}
	}
}

LocalDirectory::~LocalDirectory()
{
	close();
}

bool LocalDirectory::open(const String& path)
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

void LocalDirectory::close()
{
	bd_dir_close(m_dir);
	m_dir = 0;
}

bool LocalDirectory::first()
{
	const char* first_file_name = bd_dir_first(m_dir);
	if (first_file_name)
	{
		m_filename = first_file_name;
		return true;
	}
	return false;
}

bool LocalDirectory::next()
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

bool LocalDirectory::exist()
{
	if (m_filename == "")
	{
		return false;
	}
	return true;
}

size_t LocalDirectory::file_size() const
{
	return bd_file_size(m_filename.c_str());
}

bool LocalDirectory::is_directory() const
{
	return bd_is_directory(m_filename.c_str());
}

bool LocalDirectory::is_file() const
{
	return !is_directory();
}

String LocalDirectory::filename() const
{
	return m_filename;
}

String LocalDirectory::full_path() const
{
	return m_directory + "/" + m_filename;
}

/** Matches a string against a pattern containing wildcards */ 
bool LocalDirectory::match(const char *pattern, bool ignore_case)
{
	return File::match_pattern(pattern, m_filename.c_str());
}


/** Check if a directory exists
@param path directory path to check
@return true if directory exists */
bool LocalDirectory::exists(const char* path)
{
	return bd_is_directory(path);
}
