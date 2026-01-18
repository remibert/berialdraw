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
	return FileTools::match_pattern(pattern, string, ignore_case);
}
char * File::tmp_alloc(uint32_t length)
{
	return m_file ? m_file->tmp_alloc(length) : new char[length];
}

void File::tmp_dealloc(char * tmp, uint32_t length)
{
	if (m_file) m_file->tmp_dealloc(tmp, length);
}

#ifdef _DEBUG
static const char* TEST_ZIP_PATH = "zip://test/test_zip.zip";

// Open and close file from ZIP
void File::test1()
{
	File file;
	String path1 = String(TEST_ZIP_PATH) + "/Rémiroot.txt";
	assert(file.open(path1.c_str(), "r") == 0);
	
	uint32_t file_size = file.size();
	assert(file_size > 0);
	
	assert(file.close() == 0);
}

// Read entire file and calculate checksum
void File::test2()
{
	File file;
	String path1 = String(TEST_ZIP_PATH) + "/Rémiroot.txt";
	assert(file.open(path1.c_str(), "r") == 0);
	
	uint32_t file_size_2 = file.size();
	char* buffer = new char[file_size_2 + 1];
	memset(buffer, 0, file_size_2 + 1);
	
	int bytes_read = file.read(buffer, file_size_2);
	assert((uint32_t)bytes_read == file_size_2);
	
	uint32_t crc = compute_crc32(buffer, bytes_read);
	
	file.close();
	delete[] buffer;
}

// Test tell() position
void File::test3()
{
	File file;
	String path2 = String(TEST_ZIP_PATH) + "/Testroot.txt";
	assert(file.open(path2.c_str(), "r") == 0);
	
	long initial_pos = file.tell();
	assert(initial_pos == 0);
	
	char byte_buffer[10];
	file.read(byte_buffer, 5);
	long after_read_pos = file.tell();
	assert(after_read_pos == 5);
	
	file.close();
}

// Test seek() operations
void File::test4()
{
	File file;
	String path2 = String(TEST_ZIP_PATH) + "/Testroot.txt";
	assert(file.open(path2.c_str(), "r") == 0);
	
	file.seek(2, SEEK_SET);
	long pos_set = file.tell();
	assert(pos_set == 2);
	
	file.seek(3, SEEK_CUR);
	long pos_cur = file.tell();
	assert(pos_cur == 5);
	
	uint32_t end_size = file.size();
	file.seek(-2, SEEK_END);
	long pos_end = file.tell();
	assert(pos_end == (long)end_size - 2);
	
	file.close();
}

// Read file at different offsets
void File::test5()
{
	File file;
	String path3 = String(TEST_ZIP_PATH) + "/root1/dir1/Rémi.txt";
	assert(file.open(path3.c_str(), "r") == 0);
	
	uint32_t sz = file.size();
	char* buf1 = new char[sz];
	memset(buf1, 0, sz);
	
	file.seek(0, SEEK_SET);
	file.read(buf1, sz);
	uint32_t crc_full = compute_crc32(buf1, sz);
	
	file.close();
	delete[] buf1;
}

// Test offset() methods
void File::test6()
{
	File file;
	String path4 = String(TEST_ZIP_PATH) + "/root1/dir1/Test.txt";
	assert(file.open(path4.c_str(), "r") == 0);

	char buf2;
	assert(file.read(&buf2, 1) == 1);
	assert(file.seek(0, SEEK_SET) == 0);
	
	uint32_t offset_0 = file.offset();
	assert(offset_0 == 0);
	
	file.offset(10);
	uint32_t offset_10 = file.offset();
	assert(offset_10 == 10);
	
	file.close();
}

// Multiple file reads with checksums
void File::test7()
{
	struct TestFile {
		const char* name;
		uint32_t expected_crc;
	};
	
	static TestFile test_files[] = {
		{"/Rémiroot.txt", 0x4c5342da},
		{"/Testroot.txt", 0x1870840b},
		{"/root1/dir1/Rémi.txt", 0x4c5342da},
		{"/root1/dir1/Test.txt", 0x1870840b},
		{"/root1/dir2/Rémi2.txt", 0x4c5342da},
		{"/root1/dir2/Test2.txt", 0x1870840b}
	};
	
	File file;
	for (int i = 0; i < 6; i++)
	{
		String full_path = String(TEST_ZIP_PATH) + test_files[i].name;
		assert(file.open(full_path.c_str(), "r") == 0);
		uint32_t sz = file.size();
		
		if (sz > 0)
		{
			char* temp_buf = new char[sz + 1];
			memset(temp_buf, 0, sz + 1);
			int bytes = file.read(temp_buf, sz);
			uint32_t crc = compute_crc32(temp_buf, bytes);
			assert(crc == test_files[i].expected_crc);
			delete[] temp_buf;
		}
		file.close();
	}
}

// Test match_pattern static method
void File::test8()
{
	bool match1 = File::match_pattern("*.txt", "file.txt", false);
	assert(match1);
	
	bool match2 = File::match_pattern("*.TXT", "file.txt", true);
	assert(match2);
	
	bool match3 = File::match_pattern("test*.txt", "testfile.txt", false);
	assert(match3);
	
	bool nomatch = File::match_pattern("*.doc", "file.txt", false);
	assert(!nomatch);
}

void File::test()
{
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
	test8();
}

#endif  // _DEBUG


