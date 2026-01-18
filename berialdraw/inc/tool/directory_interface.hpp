#pragma once

namespace berialdraw
{
	/** Abstract interface for directory implementations */
	class DirectoryInterface
	{
	public:
		/** Destructor */
		virtual ~DirectoryInterface() {}

		/** Opens the directory.
		@param path The path to the directory.
		@return True if the directory was opened successfully, false otherwise. */
		virtual bool open(const String& path) = 0;

		/** Closes the currently opened directory.*/
		virtual void close() = 0;

		/** Resets to the first file in the directory.
		@return True if the first file was found, false otherwise. */
		virtual bool first() = 0;

		/** Moves to the next file in the directory.
		@return True if the next file was found, false otherwise. */
		virtual bool next() = 0;

		/** Indicates if file or directory existing
		@return True if the file existing, false otherwise. */
		virtual bool exist() const = 0;

		/** Gets the size of the current file.
		@return The size of the current file in bytes. */
		virtual size_t file_size() const = 0;

		/** Checks if the current file is a directory.
		@return True if the current file is a directory, false otherwise. */
		virtual bool is_directory() const = 0;

		/** Checks if the current file is a regular file.
		@return True if the current file is a regular file, false otherwise. */
		virtual bool is_file() const = 0;

		/** Gets the name of the current file.
		@return The name of the current file. */
		virtual String filename() const = 0;

		/** Gets the full path of the current file.
		@return The full path of the current file. */
		virtual String full_path() const = 0;

		/** Matches a string against a pattern containing wildcards
		@param pattern The pattern to match against
		@param ignore_case true to ignore uppercase and lowercase letters
		@return true if the pattern matches the string, false otherwise */ 
		virtual bool match(const char *pattern, bool ignore_case=false) = 0;
	};
}
