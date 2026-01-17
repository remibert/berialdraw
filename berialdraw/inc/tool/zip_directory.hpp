#pragma once

namespace berialdraw
{
	/** ZIP archive directory management */
	class ZipDirectory : public DirectoryInterface
	{
	public:
		/** Constructor that initializes the directory with a given ZIP path.*/
		ZipDirectory(const char * zip_path=0);

		/** Destructor that closes the directory if opened. */
		~ZipDirectory();

		/** Opens the ZIP archive directory specified in the constructor.
		@param zip_path The path to the ZIP archive (e.g., "archive.zip@path/")
		@return True if the directory was opened successfully, false otherwise. */
		bool open(const String& zip_path);

		/** Closes the currently opened ZIP archive.*/
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

		/** Matches a string against a pattern containing wildcards
		@param pattern The pattern to match against
		@param ignore_case true to ignore uppercase and lowercase letters
		@return true if the pattern matches the string, false otherwise */ 
		bool match(const char *pattern, bool ignore_case=false);

		/** Check if a ZIP archive exists
		@param zip_path ZIP archive path to check
		@return true if archive exists */
		static bool exists(const char* zip_path);

	private:
/// @cond DOXYGEN_IGNORE
		String m_zip_path;        /**< The ZIP archive path. */
		String m_directory_path;  /**< The directory path within the archive. */
		String m_current_filename;/**< The current file name. */

		// Pointer to the ZIP archive context
		void* m_zip_file = nullptr;  // unzFile handle
		long m_current_index = -1;
/// @endcond
	};
}
