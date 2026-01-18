#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Wrapper functions for minizip's zlib_filefunc_def structure
// These use the bd_system.c file operations
static voidpf zip_open(voidpf opaque, const char* filename, int mode)
{
	voidpf  result = 0;
	(void)opaque;
	(void)mode;  // minizip only supports read mode for unzip

	String p(filename);
	if (UIManager::settings())
	{
		p = UIManager::settings()->resolve(filename);
	}

	return bd_fopen(p.c_str(), "rb");
}

static uLong zip_read(voidpf opaque, voidpf stream, void* buf, uLong size)
{
	(void)opaque;
	return (uLong)bd_fread(buf, 1, size, (bd_FILE*)stream);
}

static long zip_tell(voidpf opaque, voidpf stream)
{
	(void)opaque;
	return bd_ftell((bd_FILE*)stream);
}

static long zip_seek(voidpf opaque, voidpf stream, uLong offset, int origin)
{
	(void)opaque;
	return bd_fseek((bd_FILE*)stream, offset, origin);
}

static int zip_close(voidpf opaque, voidpf stream)
{
	(void)opaque;
	return bd_fclose((bd_FILE*)stream);
}

static int zip_error(voidpf opaque, voidpf stream)
{
	(void)opaque;
	(void)stream;
	return 0;
}

// Forward declaration for conversion functions
static String cp437_to_utf8(const char* src);

// Helper function to detect and convert different encodings to UTF-8
// Tries UTF-8, CP437, and Latin-1 in sequence
static String convert_filename_to_utf8(const char* src)
{
	String result("");
	
	if (!src)
	{
		return result;
	}
	
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
		// Already UTF-8, just return
		result = String(src);
	}
	else
	{
		// Not UTF-8, try CP437
		result = cp437_to_utf8(src);
	}
	
	return result;
}

// Convert CP437 (DOS) encoded string to UTF-8
// CP437 is commonly used in ZIP files for non-ASCII characters
static String cp437_to_utf8(const char* src)
{
	String result("");
	
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

	const size_t buffer_size = 512;
	std::unique_ptr<char[]> buffer(new char[buffer_size]);
	size_t buffer_pos = 0;
	
	while (*src && buffer_pos < buffer_size - 1)
	{
		unsigned char ch = (unsigned char)*src;
		
		if (ch < 128)
		{
			// ASCII character - copy as is
			buffer[buffer_pos++] = ch;
		}
		else
		{
			// CP437 extended character - convert to Unicode then to UTF-8
			uint16_t unicode = cp437_table[ch - 128];
			
			// Encode Unicode to UTF-8
			if (unicode < 0x80)
			{
				buffer[buffer_pos++] = (char)unicode;
			}
			else if (unicode < 0x800 && buffer_pos < buffer_size - 2)
			{
				buffer[buffer_pos++] = (char)(0xC0 | (unicode >> 6));
				buffer[buffer_pos++] = (char)(0x80 | (unicode & 0x3F));
			}
			else if (buffer_pos < buffer_size - 3)
			{
				buffer[buffer_pos++] = (char)(0xE0 | (unicode >> 12));
				buffer[buffer_pos++] = (char)(0x80 | ((unicode >> 6) & 0x3F));
				buffer[buffer_pos++] = (char)(0x80 | (unicode & 0x3F));
			}
		}
		
		src++;
	}
	
	buffer[buffer_pos] = '\0';
	result = String(buffer.get());
	
	return result;
}

ZipArchive::ZipArchive()
	: m_handle(nullptr)
	, m_current_file_open(false)
{
}

ZipArchive::~ZipArchive()
{
	close();
}

void* ZipArchive::open(const char* archive_path)
{
	void* result = nullptr;
	
	if (archive_path)
	{
		close();

		// Set up file function callbacks for minizip
		zlib_filefunc_def filefunc;
		filefunc.zopen_file = zip_open;
		filefunc.zread_file = zip_read;
		filefunc.zwrite_file = nullptr;  // Not used for unzip
		filefunc.ztell_file = zip_tell;
		filefunc.zseek_file = zip_seek;
		filefunc.zclose_file = zip_close;
		filefunc.zerror_file = zip_error;
		filefunc.opaque = nullptr;
		
		// Open the ZIP archive using unzOpen2 with custom file functions
		m_handle = unzOpen2(archive_path, &filefunc);
		result = m_handle;
	}
	
	return result;
}

void ZipArchive::close()
{
	if (m_current_file_open)
	{
		unzCloseCurrentFile((unzFile)m_handle);
		m_current_file_open = false;
	}

	if (m_handle)
	{
		unzClose((unzFile)m_handle);
		m_handle = nullptr;
	}
}

bool ZipArchive::is_open() const
{
	return m_handle != nullptr;
}

void* ZipArchive::get_handle() const
{
	return m_handle;
}

bool ZipArchive::locate_file(const char* filename, bool case_sensitive) const
{
	bool result = false;
	
	if (m_handle && filename)
	{
		// Try exact match with specified case sensitivity
		if (unzLocateFile((unzFile)m_handle, filename, case_sensitive ? 1 : 0) == UNZ_OK)
		{
			result = true;
		}
	}
	
	return result;
}

bool ZipArchive::get_current_file_info(unz_file_info& info, char* filename, uint32_t filename_size) const
{
	bool result = false;
	
	if (m_handle)
	{
		int unz_result = unzGetCurrentFileInfo((unzFile)m_handle, &info, filename, filename_size, nullptr, 0, nullptr, 0);
		if (filename && filename_size > 0)
		{
			filename[filename_size - 1] = '\0';
		}
		result = (unz_result == UNZ_OK);
	}
	
	return result;
}

bool ZipArchive::open_current_file()
{
	bool result = false;
	
	if (m_handle)
	{
		if (unzOpenCurrentFile((unzFile)m_handle) == UNZ_OK)
		{
			m_current_file_open = true;
			result = true;
		}
	}
	
	return result;
}

bool ZipArchive::close_current_file()
{
	bool result = false;
	
	if (m_handle && m_current_file_open)
	{
		result = (unzCloseCurrentFile((unzFile)m_handle) == UNZ_OK);
		m_current_file_open = false;
	}
	
	return result;
}

int ZipArchive::read_current_file(void* buf, uint32_t size)
{
	if (!m_handle || !m_current_file_open || !buf || size == 0)
	{
		return -1;
	}

	return unzReadCurrentFile((unzFile)m_handle, buf, size);
}

bool ZipArchive::go_first_file()
{
	bool result = false;
	
	if (m_handle)
	{
		result = (unzGoToFirstFile((unzFile)m_handle) == UNZ_OK);
	}
	
	return result;
}

bool ZipArchive::go_next_file()
{
	bool result = false;
	
	if (m_handle)
	{
		result = (unzGoToNextFile((unzFile)m_handle) == UNZ_OK);
	}
	
	return result;
}

String ZipArchive::get_current_filename() const
{
	String result("");
	
	if (m_handle)
	{
		const size_t filename_size = 512;
		std::unique_ptr<char[]> filename(new char[filename_size]);
		unz_file_info file_info;
		int unz_result = unzGetCurrentFileInfo((unzFile)m_handle, &file_info, filename.get(), filename_size - 1, nullptr, 0, nullptr, 0);
		if (unz_result == UNZ_OK)
		{
			filename[filename_size - 1] = '\0';
			// Convert filename to UTF-8 if needed (handles CP437 and other encodings)
			result = convert_filename_to_utf8(filename.get());
		}
	}
	
	return result;
}

