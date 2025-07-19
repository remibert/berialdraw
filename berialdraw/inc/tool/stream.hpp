#pragma once
namespace berialdraw
{
	/** Class representing a stream. */
	class Stream
	{
	public:
		/** Destructor for Stream. */
		virtual ~Stream() {}

		/** Read data from the stream.
		@param ptr pointer on data
		@param size size in bytes of data
		@return the number of bytes read, or -1 if an error occurred */
		virtual int read(void *ptr, uint32_t size) = 0;

		/** Writes data into the stream.
		@param data pointer on data
		@param size size in bytes of data
		@return the number of bytes read, or -1 if an error occurred */
		virtual int write(const void *data, uint32_t size) = 0;

		/** Give the position into stream.
		@return a file offset in a stream */
		virtual long   tell() = 0;

		/** Repositions the offset in the stream.
		@param offset Offset in stream according to the whence definition
		@param whence SEEK_SET= The file offset is set to offset bytes,
			 SEEK_CUR=The file offset is set to its current location plus offset bytes,
			 SEEK_END=The file offset is set to the size of the file plus offset bytes
		@return resulting offset location as measured in bytes from the beginning of the file.
		  On error, the value -1 is returned and errno is set to indicate the error. */
		virtual int    seek(long offset, int whence) = 0;

		/** Return the total size of the stream.
		@return size of data in bytes in stream */
		virtual uint32_t size() = 0;
	};
}
