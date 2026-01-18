#include "berialdraw_imp.hpp"

using namespace berialdraw;

bool FileTools::split_path(const char* full_path, String& archive_path, String& file_path)
{
	bool result = false;
	
	if (full_path)
	{
		// Create String from input
		String path(full_path);
		
		// Remove "zip://" prefix if present
		const char* prefix = "zip://";
		if (strncmp(path.c_str(), prefix, 6) == 0)
		{
			path = String(path.c_str() + 6);
		}
		
		// Find ".zip" in the path to split archive path from file path
		int32_t zip_pos = path.find(".zip");
		if (zip_pos != INT32_MAX)
		{
			// archive_path includes everything up to and including ".zip"
			archive_path = String(path.c_str(), zip_pos + 4);
			
			// Check if there's a file path after ".zip/"
			// For "arch.zip/file.txt", extract "file.txt"
			// For "arch.zip", file_path is empty
			const char* remaining = path.c_str() + zip_pos + 4;
			if (remaining[0] == '/' && remaining[1] != '\0')
			{
				// File path exists, skip the leading '/'
				file_path = String(remaining + 1);
			}
			else
			{
				// Directory path or no file, empty file_path
				file_path = String("");
			}
			
			result = true;
		}
	}
	
	return result;
}

bool FileTools::match_pattern(const char *pattern, const char *string, bool ignore_case)
{
	bool result = false;
	
	if (pattern && string)
	{
		bool continue_match = true;
		
		while (*pattern && *string && continue_match)
		{
			if (*pattern == '*')
			{
				// Handle wildcard
				if (*(pattern + 1) == '\0')
				{
					// * at end matches everything
					result = true;
					continue_match = false;
				}
				else
				{
					// Try to match the rest of the pattern
					const char* s = string;
					bool found = false;
					while (*s && !found)
					{
						if (FileTools::match_pattern(pattern + 1, s, ignore_case))
						{
							found = true;
						}
						s++;
					}
					result = found;
					continue_match = false;
				}
			}
			else if (*pattern == '?')
			{
				// ? matches any single character
				pattern++;
				string++;
			}
			else
			{
				char p = *pattern;
				char c = *string;
				
				if (ignore_case)
				{
					p = tolower(p);
					c = tolower(c);
				}
				
				if (p != c)
				{
					continue_match = false;
				}
				else
				{
					pattern++;
					string++;
				}
			}
		}
		
		// Check if we consumed both strings or pattern ends with *
		if (continue_match)
		{
			result = (*pattern == '\0' || (*pattern == '*' && *(pattern + 1) == '\0')) && *string == '\0';
		}
	}
	
	return result;
}

