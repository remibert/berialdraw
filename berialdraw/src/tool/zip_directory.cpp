#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Helper function to split "archive.zip@path/" format
static bool split_zip_path(const char* full_path, String& zip_path, String& dir_path)
{
	if (!full_path) return false;
	
	const char* at_pos = strchr(full_path, '@');
	if (!at_pos)
	{
		// No @ sign, assume entire path is ZIP archive
		zip_path = String(full_path);
		dir_path = String("");
		return true;
	}
	
	zip_path = String(full_path, at_pos - full_path);
	dir_path = String(at_pos + 1);
	
	return true;
}

// Helper to check if a filename matches the directory prefix
static bool is_in_directory(const char* filename, const char* dir_prefix)
{
	if (!dir_prefix || strlen(dir_prefix) == 0)
	{
		// Root directory - accept all files that don't have subdirs
		return strchr(filename, '/') == nullptr;
	}
	
	if (strncmp(filename, dir_prefix, strlen(dir_prefix)) != 0)
	{
		return false;
	}
	
	// Check if it's a direct child of the directory
	const char* after_prefix = filename + strlen(dir_prefix);
	return strchr(after_prefix, '/') == nullptr;
}

ZipDirectory::ZipDirectory(const char * zip_path)
	: m_zip_file(nullptr)
	, m_current_index(-1)
{
	if (zip_path)
	{
		open(zip_path);
	}
}

ZipDirectory::~ZipDirectory()
{
	close();
}

bool ZipDirectory::open(const String& zip_path)
{
	close();
	
	String archive_path;
	String dir_path;
	
	if (!split_zip_path(zip_path.c_str(), archive_path, dir_path))
	{
		return false;
	}
	
	// Open the ZIP archive
	unzFile zip_handle = unzOpen(archive_path.c_str());
	if (!zip_handle)
	{
		return false;
	}
	
	m_zip_file = zip_handle;
	m_zip_path = archive_path;
	m_directory_path = dir_path;
	m_current_index = -1;
	
	return true;
}

void ZipDirectory::close()
{
	if (m_zip_file)
	{
		unzClose((unzFile)m_zip_file);
		m_zip_file = nullptr;
		m_current_index = -1;
	}
}

bool ZipDirectory::first()
{
	if (!m_zip_file)
	{
		return false;
	}
	
	// Reset to the first file
	int result = unzGoToFirstFile((unzFile)m_zip_file);
	if (result != UNZ_OK)
	{
		return false;
	}
	
	// Find the first file in our directory
	while (true)
	{
		char filename[256];
		unzGetCurrentFileInfo((unzFile)m_zip_file, nullptr, filename, sizeof(filename) - 1, nullptr, 0, nullptr, 0);
		filename[255] = '\0';
		
		if (is_in_directory(filename, m_directory_path.c_str()))
		{
			m_current_filename = filename;
			m_current_index = 0;
			return true;
		}
		
		result = unzGoToNextFile((unzFile)m_zip_file);
		if (result != UNZ_OK)
		{
			return false;
		}
	}
}

bool ZipDirectory::next()
{
	if (!m_zip_file || m_current_index < 0)
	{
		return false;
	}
	
	int result = unzGoToNextFile((unzFile)m_zip_file);
	if (result != UNZ_OK)
	{
		return false;
	}
	
	// Find the next file in our directory
	while (true)
	{
		char filename[256];
		unzGetCurrentFileInfo((unzFile)m_zip_file, nullptr, filename, sizeof(filename) - 1, nullptr, 0, nullptr, 0);
		filename[255] = '\0';
		
		if (is_in_directory(filename, m_directory_path.c_str()))
		{
			m_current_filename = filename;
			m_current_index++;
			return true;
		}
		
		result = unzGoToNextFile((unzFile)m_zip_file);
		if (result != UNZ_OK)
		{
			return false;
		}
	}
}

bool ZipDirectory::exist() const
{
	return m_current_index >= 0;
}

bool ZipDirectory::match(const char *pattern, bool ignore_case)
{
	if (!exist())
	{
		return false;
	}
	return File::match_pattern(pattern, m_current_filename.c_str(), ignore_case);
}

size_t ZipDirectory::file_size() const
{
	if (!m_zip_file || m_current_index < 0)
	{
		return 0;
	}
	
	unz_file_info file_info;
	unzGetCurrentFileInfo((unzFile)m_zip_file, &file_info, nullptr, 0, nullptr, 0, nullptr, 0);
	
	return file_info.uncompressed_size;
}

bool ZipDirectory::is_directory() const
{
	if (!exist())
	{
		return false;
	}
	
	// In ZIP archives, directories end with a '/'
	const char* name = m_current_filename.c_str();
	size_t len = strlen(name);
	return len > 0 && name[len - 1] == '/';
}

bool ZipDirectory::is_file() const
{
	return exist() && !is_directory();
}

String ZipDirectory::filename() const
{
	if (!exist())
	{
		return String("");
	}
	
	const char* name = m_current_filename.c_str();
	
	// Extract just the filename without directory path
	const char* last_slash = strrchr(name, '/');
	if (last_slash)
	{
		return String(last_slash + 1);
	}
	
	return String(name);
}

String ZipDirectory::full_path() const
{
	if (!exist())
	{
		return String("");
	}
	
	return String(m_current_filename.c_str());
}

bool ZipDirectory::exists(const char* zip_path)
{
	String archive_path;
	String dir_path;
	
	if (!split_zip_path(zip_path, archive_path, dir_path))
	{
		return false;
	}
	
	// Check if ZIP file exists
	unzFile zip_handle = unzOpen(archive_path.c_str());
	if (!zip_handle)
	{
		return false;
	}
	
	unzClose(zip_handle);
	return true;
}


