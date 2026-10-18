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
	String resolved_path = File::resolve_path(pathname);

	if (resolved_path.find("zip://", 0) == 0)
	{
		m_file = std::make_unique<ZipFile>();
	}
	else
	{
		m_file = std::make_unique<LocalFile>();
	}
	
	return m_file ? m_file->open(resolved_path.c_str(), mode) : -1;
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
	bool result = false;
	if (file_name != nullptr && file_name[0] != '\0')
	{
		String resolved_path = File::resolve_path(file_name);

		if (resolved_path.find("zip://", 0) == 0)
		{
			result = ZipFile::exists(resolved_path);
		}
		else
		{
			result = LocalFile::exists(resolved_path);
		}
	}
	return result;
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

/** Resolve all variables in the paths */
String File::resolve_path(const char* path)
{
	String result(path);
	if (path)
	{
		if (strchr(path, '$') != nullptr && UIManager::settings())
		{
			result = UIManager::settings()->resolve(path);
		}
	}
	return result;
}

namespace
{
	// Return whether the prefix before ':' is a URI protocol, not a Windows drive.
	bool is_url_protocol(const char* input, const char*& colon)
	{
		bool valid = false;
		colon = strchr(input, ':');
		if (colon != nullptr && colon > input + 1)
		{
			const unsigned char first = (unsigned char)input[0];
			valid = (first >= 'A' && first <= 'Z') || (first >= 'a' && first <= 'z');
			for (const char* character = input + 1; valid && character < colon; character++)
			{
				const unsigned char value = (unsigned char)*character;
				valid = (value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z') ||
					(value >= '0' && value <= '9') || value == '+' || value == '-' || value == '.';
			}
		}
		return valid;
	}

	// Split a resolved path into its directory component and final filename.
	void split_location_path(const String& full_path, bool url_path, File::Location& location)
	{
		const char* last_separator = nullptr;
		for (const char* character = full_path.c_str(); *character != '\0'; character++)
		{
			if (*character == '/' || (!url_path && *character == '\\'))
			{
				last_separator = character;
			}
		}

		if (last_separator != nullptr)
		{
			location.path = String(full_path.c_str(), (size_t)(last_separator - full_path.c_str() + 1));
			location.filename = String(last_separator + 1);
		}
		else if (url_path)
		{
			// Opaque URLs keep their scheme-specific part in path.
			location.path = full_path;
		}
		else
		{
			location.filename = full_path;
		}
	}

	// Parse query parameters, preserving repeated names and resolving each component.
	void parse_location_query(const char* start, const char* end, File::Location& location)
	{
		const char* parameter_start = start;
		while (parameter_start < end)
		{
			const char* parameter_end = parameter_start;
			while (parameter_end < end && *parameter_end != '&')
			{
				parameter_end++;
			}

			if (parameter_end > parameter_start)
			{
				const char* equals = parameter_start;
				while (equals < parameter_end && *equals != '=')
				{
					equals++;
				}

				File::QueryParameter parameter;
				parameter.name = File::resolve_path(String(parameter_start, (size_t)(equals - parameter_start)).c_str());
				if (equals < parameter_end)
				{
					parameter.value = File::resolve_path(String(equals + 1, (size_t)(parameter_end - equals - 1)).c_str());
				}
				location.query_parameters.push_back(parameter);
			}

			parameter_start = parameter_end + (parameter_end < end ? 1 : 0);
		}
	}

	// Parse URL authority, path, query, fragment, and build its resolved full path.
	void parse_url_location(const char* input, const char* colon, File::Location& location)
	{
		const char* input_end = input + strlen(input);
		location.type = File::LocationType::URL;
		location.protocol = String(input, (size_t)(colon - input));
		const char* component_start = colon + 1;
		bool has_authority = component_start + 1 < input_end && component_start[0] == '/' && component_start[1] == '/';

		if (has_authority)
		{
			const char* authority_start = component_start + 2;
			const char* authority_end = authority_start;
			while (authority_end < input_end && *authority_end != '/' && *authority_end != '?' && *authority_end != '#')
			{
				authority_end++;
			}
			location.authority = File::resolve_path(String(authority_start, (size_t)(authority_end - authority_start)).c_str());
			component_start = authority_end;
		}

		const char* fragment_start = strchr(component_start, '#');
		const char* query_start = strchr(component_start, '?');
		if (query_start != nullptr && fragment_start != nullptr && query_start > fragment_start)
		{
			query_start = nullptr;
		}

		const char* path_end = input_end;
		if (query_start != nullptr)
		{
			path_end = query_start;
		}
		if (fragment_start != nullptr && fragment_start < path_end)
		{
			path_end = fragment_start;
		}

		String resolved_url_path = File::resolve_path(String(component_start, (size_t)(path_end - component_start)).c_str());
		split_location_path(resolved_url_path, true, location);

		location.full_path = location.protocol + ":";
		if (has_authority)
		{
			location.full_path += "//";
			location.full_path += location.authority;
		}
		location.full_path += resolved_url_path;

		if (query_start != nullptr)
		{
			const char* query_end = fragment_start != nullptr ? fragment_start : input_end;
			parse_location_query(query_start + 1, query_end, location);
		}
		if (fragment_start != nullptr)
		{
			location.fragment = File::resolve_path(fragment_start + 1);
		}
	}

	// Parse a filesystem path and resolve Settings variables before splitting it.
	void parse_file_location(const char* input, const char* colon, File::Location& location)
	{
		location.type = File::LocationType::FILE_PATH;
		location.full_path = File::resolve_path(input);
		split_location_path(location.full_path, false, location);

		if (colon == input + 1 && location.full_path.size() >= 2 && input[2] != '/' && input[2] != '\\')
		{
			// Preserve drive-relative paths such as C:file.txt as drive plus filename.
			location.path = String(location.full_path.c_str(), 2);
			location.filename = String(location.full_path.c_str() + 2);
		}
	}
}

// Parse a URL or filesystem path and resolve Settings variables
bool File::parse_location(const char* input, Location& result)
{
	bool parsed = false;
	result = Location();

	if (input != nullptr && input[0] != '\0')
	{
		Location location;
		const char* colon = nullptr;
		if (is_url_protocol(input, colon))
		{
			parse_url_location(input, colon, location);
		}
		else
		{
			parse_file_location(input, colon, location);
		}

		result = location;
		parsed = true;
	}

	return parsed;
}



