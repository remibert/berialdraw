#include "berialdraw_imp.hpp"

using namespace berialdraw;

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
	bool result = false;
	
	close();
	
	String archive_path;
	String file_path;
	
	if (FileTools::split_path(zip_path.c_str(), archive_path, file_path))
	{
		// Open the ZIP archive using ZipArchive (centralized management)
		void* zip_handle = m_zip_archive.open(archive_path.c_str());
		if (zip_handle)
		{
			m_zip_file = zip_handle;
			m_zip_path = archive_path;
			m_current_index = -1;
			result = true;
		}
	}
	
	return result;
}

void ZipDirectory::close()
{
	if (m_zip_file)
	{
		m_zip_file = nullptr;
		m_current_index = -1;
	}
	
	// Close the archive via ZipArchive
	m_zip_archive.close();
}

bool ZipDirectory::first()
{
	bool result = false;
	
	if (m_zip_file)
	{
		// Reset to the first file
		if (m_zip_archive.go_first_file())
		{
			String filename = m_zip_archive.get_current_filename();
			if (filename.size() > 0)
			{
				m_current_filename = filename;
				m_current_index = 0;
				result = true;
			}
		}
	}
	
	return result;
}

bool ZipDirectory::next()
{
	bool result = false;
	
	if (m_zip_file && m_current_index >= 0)
	{
		if (m_zip_archive.go_next_file())
		{
			String filename = m_zip_archive.get_current_filename();
			if (filename.size() > 0)
			{
				m_current_filename = filename;
				m_current_index++;
				result = true;
			}
		}
	}
	
	return result;
}

bool ZipDirectory::exist() const
{
	return m_current_index >= 0;
}

bool ZipDirectory::match(const char *pattern, bool ignore_case)
{
	bool result = false;
	
	if (exist())
	{
		result = FileTools::match_pattern(pattern, m_current_filename.c_str(), ignore_case);
	}
	
	return result;
}

size_t ZipDirectory::file_size() const
{
	if (!m_zip_file || m_current_index < 0)
	{
		return 0;
	}
	
	unz_file_info file_info;
	if (m_zip_archive.get_current_file_info(file_info))
	{
		return file_info.uncompressed_size;
	}
	
	return 0;
}

bool ZipDirectory::is_directory() const
{
	bool result = false;
	
	if (exist())
	{
		unz_file_info file_info;
		if (m_zip_archive.get_current_file_info(file_info))
		{
			// Check external file attributes for directory flag
			// Unix attributes are in upper 16 bits, DOS attributes in lower 16 bits
			uint32_t attr = file_info.external_fa >> 16;
			if (attr == 0)
			{
				// Fallback to DOS attributes
				attr = file_info.external_fa & 0xFFFF;
			}
			// Directory flag is 0x10 in both Unix and DOS
			result = ((attr & 0x10) != 0);
		}
	}
	
	return result;
}

bool ZipDirectory::is_file() const
{
	return exist() && !is_directory();
}

String ZipDirectory::filename() const
{
	String result("");
	
	if (exist())
	{
		result = m_current_filename;
		
		// Find the last slash using String::rfind (supports both / and \)
		int32_t last_slash_pos = result.rfind("/");
		if (last_slash_pos == INT32_MAX)
		{
			// Try backslash as fallback
			last_slash_pos = result.rfind("\\");
		}
		
		if (last_slash_pos != INT32_MAX)
		{
			// Extract filename after the last slash using String::slice
			String filename_part;
			result.slice(last_slash_pos + 1, (int32_t)result.size(), filename_part);
			result = filename_part;
		}
	}
	
	return result;
}

String ZipDirectory::full_path() const
{
	String result = String("zip://") + m_zip_path + "/" + m_current_filename;
	return result;
}

bool ZipDirectory::exists(const char* zip_path)
{
	bool result = false;
	
	String archive_path;
	String dir_path;
	
	if (FileTools::split_path(zip_path, archive_path, dir_path))
	{
		// Check if ZIP file exists using ZipArchive
		ZipArchive temp_archive;
		if (temp_archive.open(archive_path.c_str()))
		{
			result = true;
		}
	}
	
	return result;
}


