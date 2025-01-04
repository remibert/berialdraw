#ifndef TextStream_hpp_INCLUDED
#define TextStream_hpp_INCLUDED

namespace berialdraw
{
	class String;
	/** Class representing a text stream. */
	class TextStream
	{
	public:
		/** Destructor for TextStream. */
		virtual ~TextStream() {}

		/** Clear the content of stream */
		virtual void clear() = 0;

		/** Get the current offset in bytes in stream */
		virtual uint32_t offset() = 0;

		/** Set the offset in bytes in the stream */
		virtual void offset(uint32_t val) = 0;

		/** Write a wide character to the end of the current text stream
		@param character wide character to write
		@return the number of bytes written, or 0 if an error occurred */
		virtual uint32_t write_char(wchar_t character) = 0;

		/** Write a string buffer to the end of the current text stream
		@param buffer text to append
		@return the number of bytes written, or 0 if an error occurred */
		virtual uint32_t write_string(const char * buffer) = 0;

		/** Read the wide character next
		@return the wide character read or or 0 if an error occurred */
		virtual wchar_t read_char() = 0;

		/** Write with sprintf format to the end of the current text stream
		@param format sprintf format
		@return the number of bytes written, or 0 if an error occurred */
		uint32_t write_format(const char *format, ...);

		/** Write with sprintf format to the end of the current text stream
		@param format sprintf format
		@param args va args parameter 
		@return the number of bytes written, or 0 if an error occurred */
		uint32_t write_format(const char *format, va_list args);

		/** Repeat wide character and write to the end of the current text stream
		@param character wide character 
		@param count number of wide character
		@return the number of bytes written, or 0 if an error occurred */
		uint32_t pad(wchar_t character, int32_t count);

		/** Reads the following name (the name consists of a sequence of letters a to z, A to Z, 0 to 9 and _)
		@param name name to read
		@return the number of bytes written, or 0 if an error occurred */
		uint32_t read_name(String & name);

		/** Reads the following integer
		@param value integer read
		@param accurate if true, the string can contain a period and the result is returned as a value multiplied by 64
		@return the number of bytes written, or 0 if an error occurred */
		uint32_t read_integer(int64_t & value, bool & accurate);

#ifdef _DEBUG
		// Unitary test
		static void test(); 
		static void test1(); 
		static void test2(); 
		static void test3(); 
		static void test4(); 
		static void test5(); 
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		/** Allocate temporary string (reserved for internal implementation)
		@param length string length to allocate
		@return allocated temporary pointer */
		virtual char * tmp_alloc(uint32_t length) = 0;

		/** Deallocate temporary string (reserved for internal implementation)
		@param tmp allocated temporary pointer 
		@param length string length to allocate */
		virtual void tmp_dealloc(char * tmp, uint32_t length) = 0;
/// @endcond
	};
}
#endif
