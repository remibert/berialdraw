#include "berialdraw_imp.hpp"
using namespace berialdraw;

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
	MemoryLeakLog
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
}

