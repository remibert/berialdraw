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
	if (strchr(path.c_str(),'$') != 0 && UIManager::settings())
	{
		p = UIManager::settings()->resolve(path);
	}

	if (p.find("zip://", 0) == 0)
	{
		m_directory = std::make_unique<ZipDirectory>();
	}
	else
	{
		m_directory = std::make_unique<LocalDirectory>();
	}
	
	return m_directory ? m_directory->open(p) : false;
}

void Directory::close()
{
	if (m_directory) m_directory->close();
}

bool Directory::first()
{
	return m_directory ? m_directory->first() : false;
}

bool Directory::next()
{
	return m_directory ? m_directory->next() : false;
}

bool Directory::exist()
{
	return m_directory ? m_directory->exist() : false;
}

size_t Directory::file_size() const
{
	return m_directory ? m_directory->file_size() : 0;
}

bool Directory::is_directory() const
{
	return m_directory ? m_directory->is_directory() : false;
}

bool Directory::is_file() const
{
	return m_directory ? m_directory->is_file() : false;
}

String Directory::filename() const
{
	return m_directory ? m_directory->filename() : String("");
}

String Directory::full_path() const
{
	return m_directory ? m_directory->full_path() : String("");
}

bool Directory::match(const char *pattern, bool ignore_case)
{
	return m_directory ? m_directory->match(pattern, ignore_case) : false;
}

bool Directory::exists(const char* path)
{
	if (path == nullptr)
	{
		return false;
	}

	String p(path);
	if (p.find("zip://", 0) == 0)
	{
		return ZipDirectory::exists(path);
	}
	else
	{
		return LocalDirectory::exists(path);
	}
}

