#include "berialdraw_imp.hpp"
using namespace berialdraw;

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

// Test URL, query parameter, filesystem path, and Settings variable parsing
void File::test9()
{
	Location location;
	assert(File::parse_location("zip://archive.zip/dir/file.ext?var=one&var2=two#section", location));
	assert(location.type == LocationType::URL);
	assert(location.protocol == "zip");
	assert(location.authority == "archive.zip");
	assert(location.path == "/dir/");
	assert(location.filename == "file.ext");
	assert(location.full_path == "zip://archive.zip/dir/file.ext");
	assert(location.fragment == "section");
	assert(location.query_parameters.size() == 2);
	assert(location.query_parameters[0].name == "var");
	assert(location.query_parameters[0].value == "one");
	assert(location.query_parameters[1].name == "var2");
	assert(location.query_parameters[1].value == "two");

	assert(File::parse_location("C:\\directory\\filename.ext", location));
	assert(location.type == LocationType::FILE_PATH);
	assert(location.protocol == "");
	assert(location.path == "C:\\directory\\");
	assert(location.filename == "filename.ext");
	assert(location.full_path == "C:\\directory\\filename.ext");
	assert(File::parse_location("/tmp/directory/filename.ext", location));
	assert(location.type == LocationType::FILE_PATH);
	assert(location.path == "/tmp/directory/");
	assert(location.filename == "filename.ext");

	assert(File::parse_location("$(ui.tests)/out/test.svg", location));
	assert(location.type == LocationType::FILE_PATH);
	assert(location.protocol == "");
	assert(location.authority == "");
	assert(location.path == File::resolve_path("$(ui.tests)/out/"));
	assert(location.filename == "test.svg");
	assert(location.full_path == File::resolve_path("$(ui.tests)/out/test.svg"));
	assert(location.query_parameters.empty());
	assert(location.fragment == "");
	assert(File::parse_location("$(ui.tests)/out/snapshot.JPG?quality=82&background=0xFF112233#preview", location));
	assert(location.full_path == File::resolve_path("$(ui.tests)/out/snapshot.JPG"));
	assert(location.filename == "snapshot.JPG");
	assert(location.query_parameters.size() == 2);
	assert(location.query_parameters[0].name == "quality");
	assert(location.query_parameters[0].value == "82");
	assert(location.fragment == "preview");

	assert(File::parse_location("zip://archive.zip/file.ext?root=$(ui.tests)", location));
	assert(location.query_parameters.size() == 1);
	assert(location.query_parameters[0].name == "root");
	assert(location.query_parameters[0].value == File::resolve_path("$(ui.tests)"));

	assert(File::parse_location("https://example.org/dir/file.ext#section?ignored=yes", location));
	assert(location.type == LocationType::URL);
	assert(location.path == "/dir/");
	assert(location.filename == "file.ext");
	assert(location.full_path == "https://example.org/dir/file.ext");
	assert(location.fragment == "section?ignored=yes");
	assert(location.query_parameters.empty());

	assert(File::parse_location("mailto:user@example.org?subject=hello", location));
	assert(location.type == LocationType::URL);
	assert(location.protocol == "mailto");
	assert(location.path == "user@example.org");
	assert(location.filename == "");
	assert(location.query_parameters.size() == 1);
	assert(location.query_parameters[0].name == "subject");
	assert(location.query_parameters[0].value == "hello");

	assert(File::parse_location("C:file.ext", location));
	assert(location.type == LocationType::FILE_PATH);
	assert(location.path == "C:");
	assert(location.filename == "file.ext");

	assert(File::parse_location("file.ext", location));
	assert(location.type == LocationType::FILE_PATH);
	assert(location.path == "");
	assert(location.filename == "file.ext");

	assert(File::parse_location("invalid_scheme://host/file.ext", location));
	assert(location.type == LocationType::FILE_PATH);
	assert(location.protocol == "");
	assert(location.filename == "file.ext");

	assert(!File::parse_location(nullptr, location));
	assert(!File::parse_location("", location));
}

void File::test()
{
	MemoryLeakLog
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
	test8();
	test9();
}

