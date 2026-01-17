#pragma once
namespace berialdraw
{
	/** Local filesystem file management */
	class LocalFile : public FileInterface
	{
	public:
		LocalFile();
		virtual ~LocalFile();

		/** Open file with mode
		@param pathname path and filename
		@param mode "r" to read "w" to write 
		@return 0 success, -1 failed */
		int open(const char *pathname, const char *mode);

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
		@return the number of bytes read, or -1 if an error occurred */
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

		/** Indicates if the files exists
		@param file_name Name of the file.
		@return True if file existing. */
		static bool exists(const char* file_name);

		/** Select the directory existing in the list and return it
		@param dir directory 
		@return the directory existing */
		static String resolve(const String & dir);

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

		FILE * m_file = 0;
/// @endcond
	};
}
