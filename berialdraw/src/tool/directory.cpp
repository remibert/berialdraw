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
	
	// Dispatcher: sélectionne le backend approprié
	if (p.find("tar://", 0) == 0)
	{
		//m_backend = std::make_unique<TarDirectory>();
	}
	else
	{
		m_directory = std::make_unique<LocalDirectory>();
	}
	
	return m_directory ? m_directory->open(path) : false;
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
	return LocalDirectory::exists(path);
}
