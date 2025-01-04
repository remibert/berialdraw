#ifndef Directory_hpp_INCLUDED
#define Directory_hpp_INCLUDED

	struct bd_DIR;
	typedef struct bd_DIR bd_DIR;

namespace berialdraw
{
	/** Class to manage directory operations, scanning files one by one.*/
	class Directory
	{
	public:
		/** Constructor that initializes the directory with a given path.*/
		Directory(const char * path=0);

		/** Destructor that closes the directory if opened. */
		~Directory();

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
		bool exist();

		/** Matches a string against a pattern containing wildcards
		This function matches a string against a pattern that may contain wildcards.
		The pattern may contain the following wildcards:
		- * : Matches zero or more characters
		- ? : Matches a single character
		@param pattern The pattern to match against
		@param ignore_case true to ignore uppercase and lowercase letters
		@return true if the pattern matches the string, 0 otherwise */ 
		bool match(const char *pattern, bool ignore_case=false);

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

	private:
/// @cond DOXYGEN_IGNORE
		String m_directory;               /**< The directory path. */
		String m_filename;      /**< The current file name. */

		// Pointer to the OS-specific directory context
		bd_DIR* m_dir = 0;
/// @endcond
	};
}

#endif