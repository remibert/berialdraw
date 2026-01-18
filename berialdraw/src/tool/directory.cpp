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
	
	if (p.find("zip://", 0) == 0)
	{
		m_directory = std::make_unique<ZipDirectory>();
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

#ifdef _DEBUG
static const char* TEST_ZIP_PATH = "zip://test/test_zip.zip";

// Open ZIP archive
void Directory::test1()
{
	Directory dir(TEST_ZIP_PATH);
	assert(dir.first());
}

// Iterate through all files and directories
void Directory::test2()
{
	Directory dir(TEST_ZIP_PATH);
	dir.first();
	
	int file_count = 0;
	int dir_count = 0;
	do
	{
		if (dir.is_file())
		{
			file_count++;
		}
		else if (dir.is_directory())
		{
			dir_count++;
		}
	}
	while (dir.next());
	
	assert(file_count == 6);
	assert(dir_count ==  3);
}

// Verify file existence
void Directory::test3()
{
	Directory dir(TEST_ZIP_PATH);
	dir.first();
	assert(dir.exist());
}

// Test match pattern with wildcards
void Directory::test4()
{
	Directory dir(TEST_ZIP_PATH);
	bool found_txt = false;
	do
	{
		if (dir.match("*.txt", false))
		{
			found_txt = true;
		}
	}
	while (dir.next());
	assert(found_txt);
}

// Test case-insensitive matching
void Directory::test5()
{
	Directory dir(TEST_ZIP_PATH);
	bool found_txt_upper = false;
	do
	{
		if (dir.match("*.TXT", true))
		{
			found_txt_upper = true;
			break;
		}
	}
	while (dir.next());
	assert(found_txt_upper);
}

// Verify directory vs file distinction
void Directory::test6()
{
	Directory dir(TEST_ZIP_PATH);
	bool has_dir_entry = false;
	bool has_file_entry = false;
	do
	{
		if (dir.is_directory() && !dir.is_file())
		{
			has_dir_entry = true;
		}
		if (dir.is_file() && !dir.is_directory())
		{
			has_file_entry = true;
		}
	}
	while (dir.next());
	assert(has_dir_entry);
	assert(has_file_entry);
}

// Test file sizes
void Directory::test7()
{
	Directory dir(TEST_ZIP_PATH);
	bool found_nonzero_size = false;
	do
	{
		if (dir.is_file() && dir.file_size() > 0)
		{
			found_nonzero_size = true;
			break;
		}
	}
	while (dir.next());
	assert(found_nonzero_size);
}

void Directory::test()
{
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
}

#endif  // _DEBUG
