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
	result = bd_fopen(p.c_str(), "rb");
	return result;
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

// Helper function for case-insensitive string comparison
static int str_compare_case_insensitive(const char* str1, const char* str2)
{
#ifdef _WIN32
	return _stricmp(str1, str2);
#else
	return strcasecmp(str1, str2);
#endif
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

bool ZipArchive::go_first_file() const
{
	bool result = false;
	
	if (m_handle)
	{
		result = (unzGoToFirstFile((unzFile)m_handle) == UNZ_OK);
	}
	
	return result;
}

bool ZipArchive::go_next_file() const
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
			result = String::convert_filename_encoding(filename.get());
		}
	}
	
	return result;
}

bool ZipArchive::find_file(const char* filename) const
{
	bool result = false;

	if (filename != nullptr)
	{
		// First try exact match with case sensitivity
		if (locate_file(filename, true))
		{
			result = true;
		}
		// Try case-insensitive match
		else if (locate_file(filename, false))
		{
			result = true;
		}
		// Enumerate files in ZIP to find a match
		else
		{
			const size_t name_buffer_size = 512;
			std::unique_ptr<char[]> file_name(new char[name_buffer_size]);
			unz_file_info file_info;

			// Go to first file
			if (go_first_file())
			{
				do
				{
					if (get_current_file_info(file_info, file_name.get(), name_buffer_size))
					{
						// Convert from any encoding (UTF-8, CP437, etc.) to UTF-8
						String file_name_str = String::convert_filename_encoding(file_name.get());
						
						// Compare with filename (case-insensitive) - try both encodings
						if (str_compare_case_insensitive(file_name_str.c_str(), filename) == 0 ||
						    str_compare_case_insensitive(file_name.get(), filename) == 0)
						{
							result = true;
							break;
						}
					}
				}
				while (go_next_file());
			}
		}
	}

	return result;
}

