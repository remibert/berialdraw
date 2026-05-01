#pragma once

struct bd_DIR;
typedef struct bd_DIR bd_DIR;

namespace berialdraw
{
	/** Local filesystem directory management */
	class LocalDirectory : public DirectoryInterface
	{
	public:
		/** Constructor that initializes the directory with a given path.*/
		LocalDirectory(const char * path=0);

		/** Destructor that closes the directory if opened. */
		~LocalDirectory();

		/** Opens the directory specified in the constructor.
		@param path The path to the directory.
		@return True if the directory was opened successfully, false otherwise. */
		bool open(const String& path);

		/** Closes the currently opened directory.*/
		void close();

		/** Resets to the first file in the directory.
		@return True if the first file was found, false otherwise. */
		bool first();

		/** Moves to the next file in the directory.
		@return True if the next file was found, false otherwise. */
		bool next();

		/** Indicates if file or directory existing
		@return True if the file existing, false otherwise. */
		bool exist() const;

		/** Gets the size of the current file.
		@return The size of the current file in bytes. */
		size_t file_size() const;

		/** Checks if the current file is a directory.
		@return True if the current file is a directory, false otherwise. */
		bool is_directory() const;

		/** Checks if the current file is a regular file.
		@return True if the current file is a regular file, false otherwise. */
		bool is_file() const;

		/** Gets the name of the current file.
		@return The name of the current file. */
		String filename() const;

		/** Gets the full path of the current file.
		@return The full path of the current file. */
		String full_path() const;

		/** Check if a directory exists
		@param path directory path to check
		@return true if directory exists */
		static bool exists(const char* path);

		/** Matches a string against a pattern containing wildcards
		@param pattern The pattern to match against
		@param ignore_case true to ignore uppercase and lowercase letters
		@return true if the pattern matches the string, false otherwise */ 
		bool match(const char *pattern, bool ignore_case=false);

	private:
/// @cond DOXYGEN_IGNORE
		String m_directory;     /**< The directory path. */
		String m_filename;      /**< The current file name. */

		// Pointer to the OS-specific directory context
		bd_DIR* m_dir = 0;
/// @endcond
	};
}
