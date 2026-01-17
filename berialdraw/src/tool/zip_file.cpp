//#include "berialdraw.hpp"
//#include "tool/zip_file.hpp"
//#include "zlib.h"
//#include "zconf.h"
//#include "unzip.h"
//#include <cstring>
#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Helper function to split "archive.zip@path/file.txt" into archive and file path
static bool split_zip_path(const char* full_path, String& zip_path, String& file_path)
{
	if (!full_path) return false;
	
	const char* at_pos = strchr(full_path, '@');
	if (!at_pos) return false;
	
	zip_path = String(full_path, at_pos - full_path);
	file_path = String(at_pos + 1);
	
	return true;
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
		if (unzOpenCurrentFile((unzFile)m_zip_file) != UNZ_OK)
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
		int bytes_read = unzReadCurrentFile((unzFile)m_zip_file, pos, to_read);

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
	unzCloseCurrentFile((unzFile)m_zip_file);
	m_zip_opened = false;
}

int ZipFile::open(const char *zip_path, const char *mode)
{
	String archive_path;
	String file_path;
	
	// Close any previously opened file
	close();
	
	// Parse the zip_path format: "archive.zip@path/to/file"
	if (!split_zip_path(zip_path, archive_path, file_path))
	{
		return -1;
	}

	// Only read mode is supported
	if (!mode || mode[0] != 'r')
	{
		return -1;
	}
	
	// Open the ZIP archive
	unzFile zip_handle = unzOpen(archive_path.c_str());
	if (!zip_handle)
	{
		return -1;
	}

	// Locate the file within the archive
	int result = unzLocateFile(zip_handle, file_path.c_str(), 0);
	if (result != UNZ_OK)
	{
		unzClose(zip_handle);
		return -1;
	}

	// Get file info
	unz_file_info file_info;
	unzGetCurrentFileInfo(zip_handle, &file_info, nullptr, 0, nullptr, 0, nullptr, 0);
	
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
		if (m_zip_opened)
		{
			unzCloseCurrentFile((unzFile)m_zip_file);
		}
		unzClose((unzFile)m_zip_file);
		m_zip_file = nullptr;
		m_zip_opened = false;
	}

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
			if (unzOpenCurrentFile((unzFile)m_zip_file) != UNZ_OK)
			{
				return -1;
			}
			m_zip_opened = true;
		}

		int bytes_read = unzReadCurrentFile((unzFile)m_zip_file, ptr, size);
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
	
	if (!split_zip_path(zip_path, archive_path, file_path))
	{
		return false;
	}
	
	unzFile zip_handle = unzOpen(archive_path.c_str());
	if (!zip_handle)
	{
		return false;
	}
	
	int result = unzLocateFile(zip_handle, file_path.c_str(), 0);
	unzClose(zip_handle);
	
	return result == UNZ_OK;
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
