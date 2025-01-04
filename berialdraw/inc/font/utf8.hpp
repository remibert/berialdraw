#ifndef Utf8_hpp_INCLUDED
#define Utf8_hpp_INCLUDED
namespace berialdraw
{
	/** A set of tools for performing UTF-8 conversions. */
	class Utf8
	{
	public:
		/** Gets the length in bytes of a character when it is encoded utf8
		@param unicode wide character
		@return length in bytes when it is encoded utf8 */
		static uint32_t length(wchar_t unicode);
		
		/** Gets the length in bytes of a begin byte when it is encoded utf8
		@param begin byte
		@return length in bytes when it is encoded utf8 */
		static int length(char begin);

		/** Converts a unicode characterto a utf8 character 
		@param unicode wide character
		@param utf8 destination buffer 
		@param size destination buffer size 
		@return returns the size written to the destination buffer */
		static uint32_t write (wchar_t unicode, char * utf8, uint32_t size);

		/** Converts a utf8 character to a unicode character
		@param utf8 data buffer containing the character utf8
		@param char_width char width decoded
		@return returns unicode character converted  */
		static wchar_t read(const char * utf8, uint32_t & char_width);

		/** Converts a utf8 character to a number
		@param utf8String string with text number
		@return returns integer number */
		static wchar_t to_int(const char * utf8String);

		/** Checks if char buffer is corrected encoded
		@param str string to check
		@return returns true string is utf8 otherwise returns false */
		static bool is_correct(const char * str);

		/** Get the capacity in utf8 for wide string */
		static uint32_t capacity(wchar_t * str);

		/** Count the number of wide char in string buffer */
		static uint32_t count(const char * string);

		/** Definition of not possible character */
		static const wchar_t not_a_char = 0xD800;
	};
}
#endif
