#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor - initializes all members to default values */
Tar::Tar()
	: m_file(nullptr), m_size(0), m_position(0), m_begin_tar(0), m_begin_file(0),
	  m_filename(), m_file_type(0), m_time(0), m_is_open(false), m_is_dir_mode(false)
{
}

/** Destructor - ensures proper cleanup of resources */
Tar::~Tar()
{
	close();
}

/** Open a TAR archive by pathname (C-string wrapper) */
Tar::TarResult Tar::open(const char* pathname)
{
	if (!pathname)
	{
		return TarResult::Failure;
	}

	String path(pathname);
	return open(path);
}

/** Open and index a TAR archive file
@param pathname Path to the TAR file to open
@return Success if archive opened and indexed successfully, Failure otherwise */
Tar::TarResult Tar::open(const String& pathname)
{
	close();

	// Attempt to open the TAR file in binary read mode
	auto file = std::make_unique<File>();
	if (!file || file->open(pathname.c_str(), "rb") != 0)
	{
		return TarResult::Failure;
	}
	m_file = std::move(file);

	// Initialize state variables for archive traversal
	m_is_open = true;
	m_is_dir_mode = true;
	m_position = 0;
	m_begin_file = 0;
	m_begin_tar = 0;

	// Build index of all files in the archive for quick lookup
	if (build_index() != TarResult::Success)
	{
		close();
		return TarResult::Failure;
	}

	return TarResult::Success;
}

/** Close the currently open TAR archive and release all resources */
Tar::TarResult Tar::close()
{
	if (m_file)
	{
		File* file = dynamic_cast<File*>(m_file.get());
		if (file)
		{
			file->close();
		}
		m_file.reset();
	}

	// Clear all cached data
	m_index.clear();
	m_filename.clear();
	m_size = 0;
	m_position = 0;
	m_is_open = false;
	m_is_dir_mode = false;

	return TarResult::Success;
}

/** Read data from the current file in the archive
@param ptr Buffer to store read data
@param size Number of bytes to read
@return Number of bytes read, 0 at EOF, -1 on error */
int Tar::read(void *ptr, uint32_t size)
{
	if (!m_file || !ptr || !m_is_open)
	{
		return -1;
	}

	// Check if we've reached the end of the current file
	if (m_position >= m_size)
	{
		return 0;
	}

	// Clamp read size to avoid reading past file boundary
	uint32_t to_read = size;
	if ((m_position + to_read) > m_size)
	{
		to_read = m_size - m_position;
	}

	int result = m_file->read(ptr, to_read);
	if (result > 0)
	{
		m_position += result;
	}

	return result;
}

/** Write operation - not supported for TAR archives
@return Always returns -1 (TAR files are read-only) */
int Tar::write(const void *data, uint32_t size)
{
	(void)data;
	(void)size;
	return -1;
}

/** Get the current read position in the open file
@return Current byte offset in the file */
long Tar::tell()
{
	return m_position;
}

/** Seek to a position in the current file (SEEK_SET, SEEK_CUR, SEEK_END)
@param offset Byte offset relative to whence parameter
@param whence SEEK_SET, SEEK_CUR, or SEEK_END
@return New position on success, -1 on error */
int Tar::seek(long offset, int whence)
{
	if (!m_file || !m_is_open)
	{
		return -1;
	}

	long new_position = m_position;
	bool do_seek = false;

	switch(whence)
	{
	case SEEK_SET:
		if (offset >= 0)
		{
			if ((uint32_t)offset > m_size)
			{
				new_position = m_size;
			}
			else
			{
				new_position = offset;
			}
			do_seek = true;
		}
		break;

	case SEEK_CUR:
		do_seek = true;
		if (offset >= 0)
		{
			if ((uint32_t)(m_position + offset) > m_size)
			{
				new_position = m_size;
			}
			else
			{
				new_position = m_position + offset;
			}
		}
		else
		{
			if ((m_position + offset) < 0)
			{
				new_position = 0;
			}
			else
			{
				new_position = m_position + offset;
			}
		}
		break;

	case SEEK_END:
		if (offset <= 0)
		{
			if ((uint32_t)(-offset) > m_size)
			{
				new_position = 0;
			}
			else
			{
				new_position = m_size + offset;
			}
			do_seek = true;
		}
		break;
	}

	if (do_seek)
	{
		if ((new_position != 0 && new_position != (long)m_size) || offset == 0 || new_position != (long)m_position)
		{
			if (m_file->seek(m_begin_file + new_position, SEEK_SET) == 0)
			{
				m_position = new_position;
				return new_position;
			}
		}
	}

	return -1;
}

/** Get the size of the current file in the archive
@return Size in bytes of the current entry */
uint32_t Tar::size()
{
	return m_size;
}

/** Open TAR archive in directory iteration mode (C-string wrapper) */
Tar::TarResult Tar::dir_open(const char* pathname)
{
	if (!pathname)
	{
		return TarResult::Failure;
	}

	String path(pathname);
	return dir_open(path);
}

/** Open TAR archive in directory iteration mode
@param pathname Path to the TAR file
@return Success if archive opened successfully, Failure otherwise */
Tar::TarResult Tar::dir_open(const String& pathname)
{
	return open(pathname);
}

/** Close the archive opened in directory mode */
Tar::TarResult Tar::dir_close()
{
	return close();
}

/** Advance to the next entry in directory iteration
@return 0 on success, -1 if no more entries or error */
int Tar::dir_next()
{
	if (!m_file || !m_is_open)
	{
		return -1;
	}

	TarHeader header;
	if (read_header(header, nullptr) != TarResult::EndOfFile)
	{
		// Move to next file
		long seek_pos = m_begin_tar + (long)align_size(m_size);
		if (m_file->seek(seek_pos, SEEK_SET) == 0)
		{
			return 0;
		}
	}

	return -1;
}

/** Get the filename of the current directory entry
@return Reference to the current filename string */
const String& Tar::dir_get_filename() const
{
	return m_filename;
}

/** Get the size of the current directory entry
@return File size in bytes */
long Tar::dir_get_size() const
{
	return (long)m_size;
}

/** Get the byte offset of the current entry in the archive
@return Absolute position in the TAR file */
long Tar::dir_get_offset() const
{
	return m_begin_tar;
}

/** Check if the current entry is a directory
@return True if entry is a directory, false otherwise */
bool Tar::dir_is_directory() const
{
	return (m_file_type == TAR_TYPE_DIRECTORY);
}

/** Check if the current entry is a regular file
@return True if entry is a file, false otherwise */
bool Tar::dir_is_file() const
{
	return (m_file_type == TAR_TYPE_NORMAL_FILE || m_file_type == '\0');
}

/** Align size up to the next TAR record boundary (512 bytes)
@param size Size to align
@return Aligned size (multiple of TAR_RECORDSIZE) */
uint32_t Tar::align_size(uint32_t size)
{
	return ((size % TAR_RECORDSIZE) == 0) ? size : (((size / TAR_RECORDSIZE) + 1) * TAR_RECORDSIZE);
}

/** Compare two file paths case-insensitively, treating backslashes as forward slashes
@param path1 First path to compare
@param path2 Second path to compare
@return True if paths are equal (case and separator insensitive), false otherwise */
bool Tar::compare_paths(const char* path1, const char* path2)
{
	if (!path1 || !path2)
	{
		return false;
	}

	while (*path1 && *path2)
	{
		char c1 = (*path1 == '\\') ? '/' : (char)std::tolower((unsigned char)*path1);
		char c2 = (*path2 == '\\') ? '/' : (char)std::tolower((unsigned char)*path2);

		if (c1 != c2)
		{
			return false;
		}

		++path1;
		++path2;
	}

	return (*path1 == '\0' && *path2 == '\0');
}

/** Convert an octal string to unsigned long (TAR header field format)
@param str Octal string to convert
@return Converted value, or 0 if str is null */
uint32_t Tar::octal_to_ulong(const char* str)
{
	uint32_t result = 0;
	if (!str)
	{
		return 0;
	}

	while (*str >= '0' && *str <= '7')
	{
		result = (result << 3) | (*str - '0');
		++str;
	}

	return result;
}

/** Read and parse the next TAR header record
@param header Reference to TAR header structure to fill
@param search_filename Optional filename to search for (nullptr to read next)
@return Found if search matches, EndOfFile at archive end, NotFound or error otherwise */
Tar::TarResult Tar::read_header(TarHeader& header, const char* search_filename)
{
	TarResult result = TarResult::NotFound;

	// Read TAR header record (512 bytes)
	if (m_file->read(&header, TAR_RECORDSIZE) > 0)
	{
		// Handle extended long filename (GNU extension: ././@LongLink)
		if (compare_paths((const char*)header.name, "././@LongLink"))
		{
			// Read the extended name record
			if (m_file->read(&header, TAR_RECORDSIZE) > 0)
			{
				m_filename = (const char*)header.name;

				// Check if this filename matches search criteria
				if (search_filename && compare_paths((const char*)header.name, search_filename))
				{
					result = TarResult::Found;
				}

				// Read the actual file metadata record
				if (m_file->read(&header, TAR_RECORDSIZE) != (int)TAR_RECORDSIZE)
				{
					result = TarResult::EndOfFile;
				}
			}
			else
			{
				result = TarResult::EndOfFile;
			}
		}
		else
		{
			// Standard filename from header
			m_filename = (const char*)header.name;

			// Check if this filename matches search criteria
			if (search_filename && compare_paths((const char*)header.name, search_filename))
			{
				result = TarResult::Found;
			}
		}

		// Parse file size from octal-encoded field
		m_size = octal_to_ulong((const char*)header.size);

		// Extract entry type (file, directory, symlink, etc.)
		m_file_type = header.file_type;

		// Save the byte offset where file data begins in archive
		m_begin_tar = m_file->tell();

		// Parse modification timestamp from octal-encoded field
		m_time = octal_to_ulong((const char*)header.mtime);
		if (m_time == 0)
		{
			m_time = 1;  // Ensure valid timestamp
		}

		// Normalize path separators (convert backslashes to forward slashes)
		m_filename.replace("\\", "/");

		// Detect end of archive (two consecutive empty records)
		if (header.name[0] == '\0')
		{
			bool is_empty = true;
			for (uint32_t i = 0; i < TAR_RECORDSIZE; ++i)
			{
				if (((uint8_t*)&header)[i] != 0)
				{
					is_empty = false;
					break;
				}
			}
			if (is_empty)
			{
				result = TarResult::EndOfFile;
			}
		}

		// Validate TAR header magic and supported file types
		switch(header.file_type)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '\0':
		case 'x':
			if (header.magic[0] != '\0')
			{
				if (std::strncmp((const char*)header.magic, "ustar", 5) != 0)
				{
					result = TarResult::EndOfFile;
				}
			}
			break;

		default:
			result = TarResult::EndOfFile;
			break;
		}
	}
	else
	{
		result = TarResult::EndOfFile;
	}

	return result;
}

/** Build a searchable index of all files in the TAR archive
@param tar_offset Optional offset for nested archives (default: 0)
@return Success if index built successfully, Failure on error */
Tar::TarResult Tar::build_index(long tar_offset)
{
	TarHeader header;
	uint32_t size = 0;

	if (tar_offset != 0)
	{
		// Start at specified offset (for nested TAR files)
		if (m_file->seek(align_size(tar_offset), SEEK_SET) != 0)
		{
			return TarResult::Failure;
		}
	}

	// Scan through all archive entries and build index map
	while (read_header(header, nullptr) != TarResult::EndOfFile)
	{
		// Create index entry for fast lookup
		TarIndexEntry entry;
		entry.fileSize = m_size;
		entry.fileTime = m_time;
		entry.offset = m_begin_tar;

		// Store in map using uppercase filename as key (case-insensitive search)
		String filename = m_filename;
		filename.to_upper();
		m_index[filename] = entry;

		size += 4 + (uint32_t)m_filename.size() + 20;

		// Seek to next entry (accounting for data alignment)
		long next_pos = m_begin_tar + (long)align_size(m_size);
		if (m_file->seek(next_pos, SEEK_SET) != 0)
		{
			break;
		}
	}

	// Reset file pointer to archive beginning
	m_file->seek(0, SEEK_SET);

	return TarResult::Success;
}

/** Find a file in the archive index (exact or partial match for nested archives)
@param filename Name of file to find
@param file_opened Whether archive is already open
@param begin_tar Starting offset if file_opened is false
@param out_split_filename Output: length of matched path component
@param out_begin_tar Output: byte offset of found entry in archive
@return True if found, false otherwise */
bool Tar::find_file(const char* filename, bool file_opened, long begin_tar,
				   uint32_t& out_split_filename, long& out_begin_tar)
{
	if (!filename || !m_file)
	{
		return false;
	}

	out_split_filename = 0;
	out_begin_tar = 0;

	// Handle empty filename case for continuation
	if (!file_opened && filename[0] == '\0')
	{
		out_begin_tar = begin_tar;
		return true;
	}

	// Prepare search filename (case-insensitive, normalized)
	String search_name(filename);
	search_name.to_upper();

	// First attempt: exact match in index
	auto it = m_index.find(search_name);
	if (it != m_index.end())
	{
		if (m_file->seek(it->second.offset, SEEK_SET) == 0)
		{
			out_begin_tar = it->second.offset;
			out_split_filename = (uint32_t)std::strlen(filename);
			return true;
		}
	}

	// Second attempt: partial match (for nested archives)
	// Example: "archive1.tar/archive2.tar/file.txt" matches "archive1.tar" first
	String partial;
	for (uint32_t i = 0; filename[i] != '\0'; ++i)
	{
		if (filename[i] == '/' || filename[i] == '\\')
		{
			auto it2 = m_index.find(partial);
			if (it2 != m_index.end())
			{
				// Found partial match - return position after the header
				out_split_filename = i;
				out_begin_tar = it2->second.offset + TAR_RECORDSIZE;
				return true;
			}
			partial += '/';
		}
		else
		{
			partial += (char)std::tolower((unsigned char)filename[i]);
		}
	}

	return false;
}

#ifdef _DEBUG
/** Test align_size function */
void Tar::test1()
{
	// Test various sizes and their alignment
	assert(align_size(0) == 0);
	assert(align_size(1) == TAR_RECORDSIZE);
	assert(align_size(512) == 512);
	assert(align_size(513) == TAR_RECORDSIZE * 2);
	assert(align_size(1024) == 1024);
	assert(align_size(1025) == TAR_RECORDSIZE * 3);
}

/** Test octal_to_ulong function */
void Tar::test2()
{
	// Test octal string conversion
	assert(octal_to_ulong("0") == 0);
	assert(octal_to_ulong("1") == 1);
	assert(octal_to_ulong("10") == 8);
	assert(octal_to_ulong("77") == 63);
	assert(octal_to_ulong("100") == 64);
	assert(octal_to_ulong("777") == 511);
	assert(octal_to_ulong("0000644") == 420);  // Common file permission
	assert(octal_to_ulong("0000755") == 493);  // Common directory permission
}

/** Test compare_paths function */
void Tar::test3()
{
	// Test path comparison (case-insensitive, / and \ equivalence)
	assert(compare_paths("file.txt", "file.txt") == true);
	assert(compare_paths("FILE.TXT", "file.txt") == true);
	assert(compare_paths("File.Txt", "FILE.txt") == true);
	
	// Test path separators
	assert(compare_paths("dir/file.txt", "dir\\file.txt") == true);
	assert(compare_paths("dir\\subdir/file.txt", "dir/subdir\\file.txt") == true);
	
	// Test different paths
	assert(compare_paths("file1.txt", "file2.txt") == false);
	assert(compare_paths("dir1/file.txt", "dir2/file.txt") == false);
	assert(compare_paths("file.txt", "file.txt.bak") == false);
}

/** Test Tar construction and destruction */
void Tar::test4()
{
	{
		Tar tar;
		assert(tar.m_file == nullptr);
		assert(tar.m_size == 0);
		assert(tar.m_position == 0);
		assert(tar.m_is_open == false);
		assert(tar.m_is_dir_mode == false);
		assert(tar.m_filename.size() == 0);
	}
	
	// Test multiple constructions
	{
		Tar tar1;
		Tar tar2;
		Tar tar3;
		// All should be independent and empty
		assert(tar1.m_is_open == false);
		assert(tar2.m_is_open == false);
		assert(tar3.m_is_open == false);
	}
}

/** Test opening/closing tar files */
void Tar::test5()
{
	// Test close on unopened archive
	{
		Tar tar;
		assert(tar.close() == TarResult::Success);
	}
	
	// Test opening with null pointer
	{
		Tar tar;
		assert(tar.open((const char*)nullptr) == TarResult::Failure);
	}
	
	// Test opening non-existent file
	{
		Tar tar;
		assert(tar.open("nonexistent_file_xyz.tar") == TarResult::Failure);
	}
	
	// Test close after failed open
	{
		Tar tar;
		tar.open("nonexistent.tar");
		assert(tar.close() == TarResult::Success);
		assert(tar.m_is_open == false);
	}
}

/** Test Stream interface methods */
void Tar::test6()
{
	// Test unopened archive
	{
		Tar tar;
		
		// tell() should work but return current position
		assert(tar.tell() == 0);
		
		// size() should return 0 for unopened archive
		assert(tar.size() == 0);
		
		// write() should not be supported
		int result = tar.write("test", 4);
		assert(result == -1);
	}
	
	// Test read on unopened archive
	{
		Tar tar;
		char buffer[512];
		assert(tar.read(buffer, 512) == -1);
	}
	
	// Test read with null pointer
	{
		Tar tar;
		assert(tar.read(nullptr, 100) == -1);
	}
}

/** Test directory listing on unopened archive */
void Tar::test7()
{
	// Test dir_open with null pointer
	{
		Tar tar;
		assert(tar.dir_open((const char*)nullptr) == TarResult::Failure);
	}
	
	// Test dir_open non-existent file
	{
		Tar tar;
		assert(tar.dir_open("nonexistent.tar") == TarResult::Failure);
	}
	
	// Test dir_close on unopened archive
	{
		Tar tar;
		assert(tar.dir_close() == TarResult::Success);
	}
	
	// Test dir_next on unopened archive
	{
		Tar tar;
		assert(tar.dir_next() != 0);  // Should fail
	}
	
	// Test dir_get_filename on unopened archive
	{
		Tar tar;
		const String& filename = tar.dir_get_filename();
		assert(filename.size() == 0);
	}
}

/** Test directory entry inspection methods */
void Tar::test8()
{
	Tar tar;
	
	// Test on unopened archive
	assert(tar.dir_get_size() == -1);
	assert(tar.dir_get_offset() == -1);
	assert(tar.dir_is_directory() == false);
	assert(tar.dir_is_file() == false);
	
	// Test m_filename and m_file_type
	assert(tar.m_filename.size() == 0);
	assert(tar.m_file_type == 0);
	assert(tar.m_time == 0);
}

/** Test seek operations on unopened archive */
void Tar::test9()
{
	Tar tar;
	
	// Test seek from beginning
	assert(tar.seek(0, SEEK_SET) == 0);
	
	// Test seek from current
	assert(tar.seek(100, SEEK_CUR) == 100);
	
	// Test seek from end
	assert(tar.seek(-10, SEEK_END) != -1 || tar.tell() == 0);
	
	// Reset to beginning
	tar.seek(0, SEEK_SET);
	assert(tar.tell() == 0);
}

/** Test index structure */
void Tar::test10()
{
	Tar tar;
	
	// Index should be empty initially
	assert(tar.m_index.empty());
	
	// Test TarIndexEntry structure
	Tar::TarIndexEntry entry;
	entry.fileSize = 1024;
	entry.fileTime = 1234567890;
	entry.offset = 512;
	
	assert(entry.fileSize == 1024);
	assert(entry.fileTime == 1234567890);
	assert(entry.offset == 512);
}

/** Test header structure */
void Tar::test11()
{
	// Test TarHeader structure members
	TarHeader header;
	memset(&header, 0, sizeof(header));
	
	assert(sizeof(header.name) == 100);
	assert(sizeof(header.mode) == 8);
	assert(sizeof(header.size) == 12);
	assert(sizeof(header.mtime) == 12);
	assert(sizeof(header.chksum) == 8);
	assert(sizeof(header.magic) == 8);
	assert(sizeof(header.linkname) == 100);
	assert(sizeof(header.uname) == 32);
	assert(sizeof(header.gname) == 32);
	assert(sizeof(header.devmajor) == 8);
	assert(sizeof(header.devminor) == 8);
	assert(sizeof(header) == 512);  // Tar record size
}

/** Test file type constants */
void Tar::test12()
{
	assert(Tar::TAR_TYPE_NORMAL_FILE == '0');
	assert(Tar::TAR_TYPE_HARD_LINK == '1');
	assert(Tar::TAR_TYPE_SYMBOLIC_LINK == '2');
	assert(Tar::TAR_TYPE_CHAR_FILE == '3');
	assert(Tar::TAR_TYPE_BLOCK_FILE == '4');
	assert(Tar::TAR_TYPE_DIRECTORY == '5');
	assert(Tar::TAR_TYPE_NAMED_PIPE == '6');
	assert(Tar::TAR_TYPE_CONTIGUOUS == '7');
}

/** Main test function that runs all tar tests */
void Tar::test()
{
	test12();
	test11();
	test10();
	test9();
	test8();
	test7();
	test6();
	test5();
	test4();
	test3();
	test2();
	test1();
}
#endif
