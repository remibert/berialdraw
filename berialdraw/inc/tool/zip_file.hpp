#pragma once

namespace berialdraw
{
	/** ZIP archive file management with random access support */
	class ZipFile : public FileInterface
	{
	public:
		ZipFile();
		virtual ~ZipFile();

		/** Open file in ZIP archive with mode
		@param zip_path path to the ZIP file followed by @file_path (e.g., "archive.zip@path/file.txt")
		@param mode "r" to read, "w" to write 
		@return 0 success, -1 failed */
		int open(const char *zip_path, const char *mode);

		/** Close file
		@return 0 success, -1 failed */
		int close();

		/** Read data from the stream.
		@param ptr pointer on data
		@param size size in bytes of data
		@return the number of bytes read, or -1 if an error occurred */
		virtual int read(void *ptr, uint32_t size);

		/** Writes data into the stream.
		@param data pointer on data
		@param size size in bytes of data
		@return the number of bytes written, or -1 if an error occurred */
		virtual int write(const void *data, uint32_t size);

		/** Give the position into stream.
		@return a file offset in a stream */
		virtual long tell();

		/** Repositions the offset in the stream.
		@param offset Offset in stream according to the whence definition
		@param whence SEEK_SET= The file offset is set to offset bytes,
			 SEEK_CUR=The file offset is set to its current location plus offset bytes,
			 SEEK_END=The file offset is set to the size of the file plus offset bytes
		@return the offset location, or -1 if an error occurred */
		virtual int seek(long offset, int whence);

		/** Return the total size of the stream.
		@return size of data in bytes in stream */
		virtual uint32_t size();

		/** Write a string buffer to the end of the current text stream
		@param buffer text to append
		@return the number of bytes written, or 0 if an error occurred */
		virtual uint32_t write_string(const char * buffer);

		/** Write a wide character to the end of the current text stream
		@param character wide character 
		@return the number of bytes written, or 0 if an error occurred */
		virtual uint32_t write_char(wchar_t character);

		/** Clear the content of stream */
		virtual void clear();

		/** Get the current offset in buffer */
		virtual uint32_t offset();

		/** Set the offset in buffer */
		virtual void offset(uint32_t val);

		/** Read the wide character next
		@return the wide character read or null if it ends */
		virtual wchar_t read_char();

		/** Indicates if the file exists in the ZIP archive
		@param zip_path ZIP archive path with file (e.g., "archive.zip@path/file.txt")
		@return True if file existing in archive. */
		static bool exists(const char* zip_path);

		/** Set sequential mode for reduced memory consumption
		In sequential mode, seek is not possible but memory usage is minimal.
		By default, files are opened in buffered (non-sequential) mode. */
		void set_sequential(bool sequential = true);

	protected:
/// @cond DOXYGEN_IGNORE
		/** Allocate temporary string
		@param length string length to allocate
		@return allocated temporary pointer */
		virtual char * tmp_alloc(uint32_t length);

		/** Deallocate temporary string
		@param tmp allocated temporary pointer 
		@param length string length to allocate */
		virtual void tmp_dealloc(char * tmp, uint32_t length);

	private:
		/** Decompress the entire file into buffer for random access
		Used when sequential mode is disabled (default) */
		void uncompress();

		void* m_zip_file = nullptr;    // unzFile handle
		Buffer m_buffer;               // Cache for decompressed data
		uint32_t m_file_size = 0;
		bool m_sequential = false;     // Sequential vs buffered mode
		bool m_zip_opened = false;     // Is file open in ZIP
/// @endcond
	};
}
