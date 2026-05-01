#pragma once

namespace berialdraw
{
	/** Utilities for file operations */
	class FileTools
	{
	public:
		/** Split a path in the format "zip://archive.zip/path/to/file" or other protocols
		@param full_path The full path to split
		@param archive_path Output: path to the archive
		@param file_path Output: path within the archive
		@return True if split successfully, false otherwise */
		static bool split_path(const char* full_path, String& archive_path, String& file_path);

		/** Match a filename pattern with wildcards
		@param pattern Pattern with * (match any) and ? (match single char)
		@param string String to match against
		@param ignore_case Ignore case when comparing
		@return True if pattern matches string */
		static bool match_pattern(const char *pattern, const char *string, bool ignore_case = false);

	private:
		/** Private constructor to prevent instantiation */
		FileTools();
	};
}

