#ifndef ItemString_hpp_INCLUDED
#define ItemString_hpp_INCLUDED

namespace berialdraw
{
	/** Json string item */
	class ItemString : public ItemVariant
	{
	public:
		/** Create new item string */
		ItemString(String & string);

		/** Create new item string */
		ItemString(const char * string=0);

		/** Destructor */
		virtual ~ItemString();

		/** Serialize to json */
		virtual void serialize(TextStream & out, int32_t indent=-1);

		/** Unserialize from json */
		virtual bool unserialize(TextStream & content);

		/** Get the internal string */
		const String & get() const;

		/** Convert to string */
		virtual operator const String() const;

		/** Convert to boolean */
		virtual operator bool() const;

		/** Convert to integer */
		virtual operator int() const;

		/** Convert to integer */
		virtual operator unsigned int() const;

		/** Convert to integer */
		virtual operator long long() const;

		/** Indicates if the item is null */
		virtual bool is_null() const;

		/** Indicates if the integer is accurate */
		virtual bool is_accurate() const;


		/** Compares if the current string is equal to another C-style string.
		@param other The C-style string to compare with.
		@return True if the strings are equal, false otherwise. */
		virtual bool operator==(const char * other) const;

		/** Compares if the current string is equal to another String object.
		@param other The String object to compare with.
		@return True if the strings are equal, false otherwise. */
		virtual bool operator==(const String & other) const;

		/** Compares if the current string is equal to an integer.
		@param other The integer to compare with.
		@return True if the string and integer are equal, false otherwise. */
		virtual bool operator==(int other) const;

		/** Compares if the current string is equal to an unsigned integer.
		@param other The unsigned integer to compare with.
		@return True if the string and unsigned integer are equal, false otherwise. */
		virtual bool operator==(unsigned int other) const;

		/** Compares if the current string is equal to a long long integer.
		@param other The long long integer to compare with.
		@return True if the string and long long integer are equal, false otherwise. */
		virtual bool operator==(long long other) const;

		/** Compares if the current string is equal to a boolean.
		@param other The boolean to compare with.
		@return True if the string and boolean are equal, false otherwise. */
		virtual bool operator==(bool other) const;

		/** Compares if the current string is different from another C-style string.
		@param other The C-style string to compare with.
		@return True if the strings are different, false otherwise. */
		virtual bool operator!=(const char * other) const;

		/** Compares if the current string is different from another String object.
		@param other The String object to compare with.
		@return True if the strings are different, false otherwise. */
		virtual bool operator!=(const String & other) const;

		/** Compares if the current string is different from an integer.
		@param other The integer to compare with.
		@return True if the string and integer are different, false otherwise. */
		virtual bool operator!=(int other) const;

		/** Compares if the current string is different from an unsigned integer.
		@param other The unsigned integer to compare with.
		@return True if the string and unsigned integer are different, false otherwise.  */
		virtual bool operator!=(unsigned int other) const;

		/** Compares if the current string is different from a long long integer.
		@param other The long long integer to compare with.
		@return True if the string and long long integer are different, false otherwise. */
		virtual bool operator!=(long long other) const;

		/** Compares if the current string is different from a boolean.
		@param other The boolean to compare with.
		@return True if the string and boolean are different, false otherwise. */
		virtual bool operator!=(bool other) const;

		/** Compares if the string buffer is equal to another C-style string.
		@param other The C-style string to compare with.
		@param last_char_ignored The last character to ignore in the comparison.
		@param accurate Reference to a boolean that will be set to true if the comparison is accurate.
		@return True if the strings are equal, false otherwise. */
		bool compare(const char * other, char last_char_ignored, bool & accurate) const;

		/** Converts a C-style string to a long long integer.
		@param str The C-style string to convert.
		@return The converted long long integer. */
		static long long to_int(const char * str);
	protected:
/// @cond DOXYGEN_IGNORE
		String m_value;
/// @endcond
	};
}
#endif