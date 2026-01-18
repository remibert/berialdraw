#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Helper function for case-insensitive string comparison
// Works on both Windows and Unix platforms
static int str_compare_case_insensitive(const char* str1, const char* str2)
{
#ifdef _WIN32
	return _stricmp(str1, str2);
#else
	return strcasecmp(str1, str2);
#endif
}

// Helper function to detect and convert different encodings to UTF-8
// Tries UTF-8, CP437, and Latin-1 in sequence
static void convert_filename_to_utf8(const char* src, char* dst, size_t dst_size);

// Forward declaration needed
static void cp437_to_utf8(const char* src, char* dst, size_t dst_size);

// Implementation of encoding detection
static void convert_filename_to_utf8(const char* src, char* dst, size_t dst_size)
{
	// First check if it's already valid UTF-8
	const char* p = src;
	bool is_valid_utf8 = true;
	
	while (*p && is_valid_utf8)
	{
		unsigned char ch = (unsigned char)*p;
		if (ch >= 0x80)
		{
			// Multi-byte UTF-8 sequence
			if ((ch & 0xE0) == 0xC0)
			{
				p += 2;
			}
			else if ((ch & 0xF0) == 0xE0)
			{
				p += 3;
			}
			else if ((ch & 0xF8) == 0xF0)
			{
				p += 4;
			}
			else
			{
				is_valid_utf8 = false;
			}
		}
		else
		{
			p++;
		}
	}
	
	if (is_valid_utf8)
	{
		// Already UTF-8, just copy
		strncpy(dst, src, dst_size - 1);
		dst[dst_size - 1] = '\0';
		return;
	}
	
	// Not UTF-8, try CP437
	cp437_to_utf8(src, dst, dst_size);
}

// Convert CP437 (DOS) encoded string to UTF-8
// CP437 is commonly used in ZIP files for non-ASCII characters
static void cp437_to_utf8(const char* src, char* dst, size_t dst_size)
{
	// CP437 to Unicode mapping for characters > 127
	static const uint16_t cp437_table[128] = {
		0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7,
		0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
		0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9,
		0x00FF, 0x00D6, 0x00DC, 0x00A2, 0x00A3, 0x00A5, 0x20A7, 0x0192,
		0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA,
		0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
		0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
		0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
		0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F,
		0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
		0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B,
		0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
		0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4,
		0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
		0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248,
		0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0
	};

	size_t dst_pos = 0;
	
	while (*src && dst_pos < dst_size - 1)
	{
		unsigned char ch = (unsigned char)*src;
		
		if (ch < 128)
		{
			// ASCII character - copy as is
			dst[dst_pos++] = ch;
		}
		else
		{
			// CP437 extended character - convert to Unicode then to UTF-8
			uint16_t unicode = cp437_table[ch - 128];
			
			// Encode Unicode to UTF-8
			if (unicode < 0x80)
			{
				dst[dst_pos++] = (char)unicode;
			}
			else if (unicode < 0x800 && dst_pos < dst_size - 2)
			{
				dst[dst_pos++] = (char)(0xC0 | (unicode >> 6));
				dst[dst_pos++] = (char)(0x80 | (unicode & 0x3F));
			}
			else if (dst_pos < dst_size - 3)
			{
				dst[dst_pos++] = (char)(0xE0 | (unicode >> 12));
				dst[dst_pos++] = (char)(0x80 | ((unicode >> 6) & 0x3F));
				dst[dst_pos++] = (char)(0x80 | (unicode & 0x3F));
			}
		}
		
		src++;
	}
	
	dst[dst_pos] = '\0';
}

// Helper function to find a file in ZIP with flexible name matching
// Handles different encodings and case sensitivity
static bool find_file_in_zip(ZipArchive& archive, const char* filename)
{
	if (!filename)
	{
		return false;
	}

	// First try exact match with case sensitivity
	if (archive.locate_file(filename, true))
	{
		return true;
	}

	// Try case-insensitive match
	if (archive.locate_file(filename, false))
	{
		return true;
	}

	// Enumerate files in ZIP to find a match
	const size_t name_buffer_size = 512;
	std::unique_ptr<char[]> file_name(new char[name_buffer_size]);
	std::unique_ptr<char[]> file_name_utf8(new char[name_buffer_size]);
	unz_file_info file_info;

	// Go to first file
	if (!archive.go_first_file())
	{
		return false;
	}

	do {
		if (archive.get_current_file_info(file_info, file_name.get(), name_buffer_size))
		{
			// Convert from any encoding (UTF-8, CP437, etc.) to UTF-8
			convert_filename_to_utf8(file_name.get(), file_name_utf8.get(), name_buffer_size);
			
			// Compare with filename (case-insensitive) - try both encodings
			if (str_compare_case_insensitive(file_name_utf8.get(), filename) == 0 ||
			    str_compare_case_insensitive(file_name.get(), filename) == 0)
			{
				return true;
			}
		}
	} while (archive.go_next_file());

	return false;
}

ZipFile::ZipFile()
	: m_zip_file(nullptr)
	, m_file_size(0)
	, m_sequential(false)
	, m_zip_opened(false)
{
}

ZipFile::~ZipFile()
{
	close();
}

/** Decompress the entire file into buffer for random access */
void ZipFile::uncompress()
{
	if (!m_zip_file || m_sequential)
	{
		return;
	}

	// If already uncompressed, do nothing
	if (m_buffer.size() > 0)
	{
		return;
	}

	// Allocate buffer for decompressed data
	m_buffer.allocate(m_file_size);

	// Open file in ZIP if not already open
	if (!m_zip_opened)
	{
		if (!m_zip_archive.open_current_file())
		{
			m_buffer.clear();
			return;
		}
		m_zip_opened = true;
	}

	// Read entire file into buffer in chunks
	uint8_t* pos = m_buffer.data();
	uint32_t bytes_read_total = 0;
	uint32_t chunk_size = 4096;

	while (bytes_read_total < m_file_size)
	{
		uint32_t to_read = (m_file_size - bytes_read_total > chunk_size) ? chunk_size : (m_file_size - bytes_read_total);
		int bytes_read = m_zip_archive.read_current_file(pos, to_read);

		if (bytes_read > 0)
		{
			bytes_read_total += bytes_read;
			pos += bytes_read;
		}
		else
		{
			break;
		}
	}

	// Close file in ZIP - we'll re-open on demand if needed
	m_zip_archive.close_current_file();
	m_zip_opened = false;
}

int ZipFile::open(const char *zip_path, const char *mode)
{
	String archive_path;
	String file_path;
	
	// Close any previously opened file
	close();
	
	// Parse the zip_path format: "zip://archive.zip/path/to/file"
	if (!FileTools::split_path(zip_path, archive_path, file_path))
	{
		return -1;
	}

	// Only read mode is supported
	if (!mode || mode[0] != 'r')
	{
		return -1;
	}
	
	// Open the ZIP archive using ZipArchive (centralized management)
	void* zip_handle = m_zip_archive.open(archive_path.c_str());
	if (!zip_handle)
	{
		return -1;
	}

	// Locate the file within the archive
	if (!find_file_in_zip(m_zip_archive, file_path.c_str()))
	{
		m_zip_archive.close();
		return -1;
	}

	// Get file info
	unz_file_info file_info;
	if (!m_zip_archive.get_current_file_info(file_info))
	{
		m_zip_archive.close();
		return -1;
	}
	
	m_zip_file = zip_handle;
	m_file_size = static_cast<uint32_t>(file_info.uncompressed_size);
	m_sequential = false;
	m_zip_opened = false;
	
	return 0;
}

int ZipFile::close()
{
	if (m_zip_file)
	{
		m_zip_file = nullptr;
		m_zip_opened = false;
	}

	// Close the archive via ZipArchive
	m_zip_archive.close();

	m_buffer.clear();
	m_file_size = 0;
	
	return 0;
}

int ZipFile::read(void *ptr, uint32_t size)
{
	if (!m_zip_file || !ptr || size == 0)
	{
		return -1;
	}

	// If not sequential mode (buffered random access)
	if (!m_sequential)
	{
		// Decompress entire file into buffer if not already done
		uncompress();
		
		// Read from buffer
		return (int)m_buffer.read(ptr, size);
	}
	else
	{
		// Sequential mode: read directly from ZIP
		if (!m_zip_opened)
		{
			if (!m_zip_archive.open_current_file())
			{
				return -1;
			}
			m_zip_opened = true;
		}

		int bytes_read = m_zip_archive.read_current_file(ptr, size);
		return bytes_read;
	}
}

int ZipFile::write(const void *data, uint32_t size)
{
	// ZIP files in minizip unzip are read-only
	(void)data;
	(void)size;
	return -1;
}

long ZipFile::tell()
{
	if (!m_zip_file)
	{
		return -1;
	}

	if (!m_sequential)
	{
		// Buffered mode: return position in buffer
		return m_buffer.tell();
	}
	else
	{
		// Sequential mode: seek is not supported
		return -1;
	}
}

int ZipFile::seek(long offset, int whence)
{
	if (!m_zip_file)
	{
		return -1;
	}

	// Seeking only works in buffered (non-sequential) mode
	if (m_sequential)
	{
		return -1;
	}

	// Ensure buffer is decompressed
	uncompress();

	// Seek in buffer
	return m_buffer.seek(offset, whence);
}

uint32_t ZipFile::size()
{
	if (!m_zip_file)
	{
		return 0;
	}
	return m_file_size;
}

uint32_t ZipFile::write_string(const char * buffer)
{
	// Not supported for ZIP files
	(void)buffer;
	return 0;
}

uint32_t ZipFile::write_char(wchar_t character)
{
	// Not supported for ZIP files
	(void)character;
	return 0;
}

void ZipFile::clear()
{
	// Not applicable for ZIP files
}

uint32_t ZipFile::offset()
{
	return tell();
}

void ZipFile::offset(uint32_t val)
{
	if (!m_sequential)
	{
		m_buffer.set_position(val);
	}
}

wchar_t ZipFile::read_char()
{
	if (!m_zip_file)
	{
		return 0;
	}

	char ch = 0;
	int bytes_read = read(&ch, 1);
	if (bytes_read > 0)
	{
		return (wchar_t)ch;
	}

	return 0;
}

bool ZipFile::exists(const char* zip_path)
{
	String archive_path;
	String file_path;
	
	if (!FileTools::split_path(zip_path, archive_path, file_path))
	{
		return false;
	}
	
	// Use ZipArchive to open temporarily
	ZipArchive temp_archive;
	void* zip_handle = temp_archive.open(archive_path.c_str());
	if (!zip_handle)
	{
		return false;
	}
	
	bool found = find_file_in_zip(temp_archive, file_path.c_str());
	// temp_archive will close automatically when destroyed
	
	return found;
}

void ZipFile::set_sequential(bool sequential)
{
	m_sequential = sequential;
}

char * ZipFile::tmp_alloc(uint32_t length)
{
	return new char[length];
}

void ZipFile::tmp_dealloc(char * tmp, uint32_t length)
{
	(void)length;
	delete[] tmp;
}
