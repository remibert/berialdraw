#pragma once
namespace berialdraw
{
	/** Abstract interface for file implementations */
	class FileInterface
	{
	public:
		/** Destructor */
		virtual ~FileInterface() {}

		/** Open file with mode
		@param pathname path and filename
		@param mode "r" to read "w" to write 
		@return 0 success, -1 failed */
		virtual int open(const char *pathname, const char *mode) = 0;

		/** Close file
		@return 0 success, -1 failed */
		virtual int close() = 0;

		/** Read data from the stream.
		@param ptr pointer on data
		@param size size in bytes of data
		@return the number of bytes read, or -1 if an error occurred */
		virtual int read(void *ptr, uint32_t size) = 0;

		/** Writes data into the stream.
		@param data pointer on data
		@param size size in bytes of data
		@return the number of bytes written, or -1 if an error occurred */
		virtual int write(const void *data, uint32_t size) = 0;

		/** Give the position into stream.
		@return a file offset in a stream */
		virtual long tell() = 0;

		/** Repositions the offset in the stream.
		@param offset Offset in stream according to the whence definition
		@param whence SEEK_SET, SEEK_CUR, SEEK_END
		@return the offset location, or -1 if an error occurred */
		virtual int seek(long offset, int whence) = 0;

		/** Return the total size of the stream.
		@return size of data in bytes in stream */
		virtual uint32_t size() = 0;

		/** Write a string buffer to the end of the current text stream
		@param buffer text to append
		@return the number of bytes written, or 0 if an error occurred */
		virtual uint32_t write_string(const char * buffer) = 0;

		/** Write a wide character to the end of the current text stream
		@param character wide character 
		@return the number of bytes written, or 0 if an error occurred */
		virtual uint32_t write_char(wchar_t character) = 0;

		/** Clear the content of stream */
		virtual void clear() = 0;

		/** Get the current offset in buffer */
		virtual uint32_t offset() = 0;

		/** Set the offset in buffer */
		virtual void offset(uint32_t val) = 0;

		/** Read the wide character next
		@return the wide character read or null if it ends */
		virtual wchar_t read_char() = 0;

		/** Allocate temporary string
		@param length string length to allocate
		@return allocated temporary pointer */
		virtual char * tmp_alloc(uint32_t length) = 0;

		/** Deallocate temporary string
		@param tmp allocated temporary pointer 
		@param length string length to allocate */
		virtual void tmp_dealloc(char * tmp, uint32_t length) = 0;
	};
}
