#pragma once

namespace berialdraw
{
	/** Wrapper for managing ZIP archive handles with consistent file I/O */
	class ZipArchive
	{
	public:
		/** Constructor - creates an unopened archive handle */
		ZipArchive();

		/** Destructor - closes the archive if open */
		~ZipArchive();

		/** Open a ZIP archive using custom file callbacks for consistent I/O
		@param archive_path Path to the ZIP file
		@return unzFile handle if successful, nullptr otherwise */
		void* open(const char* archive_path);

		/** Close the currently open archive */
		void close();

		/** Check if archive is currently open */
		bool is_open() const;

		/** Get the current archive handle */
		void* get_handle() const;

		/** Locate a file in the archive
		@param filename Filename to locate
		@param case_sensitive Whether to perform case-sensitive search
		@return true if file found and positioned, false otherwise */
		bool locate_file(const char* filename, bool case_sensitive = false) const;

		/** Get information about current file in archive
		@param info Output structure for file info
		@param filename Output buffer for filename
		@param filename_size Size of filename buffer
		@return true if successful, false otherwise */
		bool get_current_file_info(unz_file_info& info, char* filename = nullptr, uint32_t filename_size = 0) const;

		/** Open current file for reading
		@return true if successful, false otherwise */
		bool open_current_file();

		/** Close current file
		@return true if successful, false otherwise */
		bool close_current_file();

		/** Read from current file
		@param buf Buffer to read into
		@param size Number of bytes to read
		@return Number of bytes read, -1 on error */
		int read_current_file(void* buf, uint32_t size);

		/** Go to first file in archive
		@return true if successful, false otherwise */
		bool go_first_file() const;

		/** Go to next file in archive
		@return true if successful, false otherwise */
		bool go_next_file() const;

		/** Get current filename from archive position
		@return String with filename, empty string if no file or error */
		String get_current_filename() const;

		/** Find a file in the archive with flexible matching
		Handles different encodings (UTF-8, CP437) and case sensitivity
		@param filename Filename to find
		@return true if file found and positioned, false otherwise */
		bool find_file(const char* filename) const;

	private:
		void* m_handle;
		bool m_current_file_open;
	};
}

