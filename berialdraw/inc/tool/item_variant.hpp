#ifndef ItemVariant_hpp_INCLUDED
#define ItemVariant_hpp_INCLUDED
namespace berialdraw
{
	/** Item variant can be integer, long integer, boolean, string */
	class ItemVariant : public Item
	{
	public:
		/** Destructor */
		virtual ~ItemVariant(){}

		/** Serialize json into string */
		virtual void serialize(TextStream & out, int32_t indent) = 0;

		/** Unserialize json from string */
		virtual bool unserialize(TextStream & content) = 0;

		/** Convert to string */
		virtual operator const String() const = 0;

		/** Convert to boolean */
		virtual operator bool() const = 0;

		/** Convert to integer */
		virtual operator int() const = 0;

		/** Convert to integer */
		virtual operator unsigned int() const = 0;

		/** Convert to integer */
		virtual operator long long() const = 0;

		/** Indicates if the item is null */
		virtual bool is_null() const = 0;

		/** Indicates if the integer is accurate */
		virtual bool is_accurate() const = 0;

		/** Compares if the current variant is equal to another C-style string.
		@param other The C-style string to compare with.
		@return True if the strings are equal, false otherwise. */
		virtual bool operator==(const char * other) const = 0;

		/** Compares if the current variant is equal to another String object.
		@param other The String object to compare with.
		@return True if the strings are equal, false otherwise. */
		virtual bool operator==(const String & other) const = 0;

		/** Compares if the current variant is equal to an integer.
		@param other The integer to compare with.
		@return True if the string and integer are equal, false otherwise. */
		virtual bool operator==(int other) const = 0;

		/** Compares if the current variant is equal to an unsigned integer.
		@param other The unsigned integer to compare with.
		@return True if the string and unsigned integer are equal, false otherwise. */
		virtual bool operator==(unsigned int other) const = 0;

		/** Compares if the current variant is equal to a long long integer.
		@param other The long long integer to compare with.
		@return True if the string and long long integer are equal, false otherwise. */
		virtual bool operator==(long long other) const = 0;

		/** Compares if the current variant is equal to a boolean.
		@param other The boolean to compare with.
		@return True if the string and boolean are equal, false otherwise. */
		virtual bool operator==(bool other) const = 0;

		/** Compares if the current variant is different from another C-style string.
		@param other The C-style string to compare with.
		@return True if the strings are different, false otherwise. */
		virtual bool operator!=(const char * other) const = 0;

		/** Compares if the current variant is different from another String object.
		@param other The String object to compare with.
		@return True if the strings are different, false otherwise. */
		virtual bool operator!=(const String & other) const = 0;

		/** Compares if the current variant is different from an integer.
		@param other The integer to compare with.
		@return True if the string and integer are different, false otherwise. */
		virtual bool operator!=(int other) const = 0;

		/** Compares if the current variant is different from an unsigned integer.
		@param other The unsigned integer to compare with.
		@return True if the string and unsigned integer are different, false otherwise.  */
		virtual bool operator!=(unsigned int other) const = 0;

		/** Compares if the current variant is different from a long long integer.
		@param other The long long integer to compare with.
		@return True if the string and long long integer are different, false otherwise. */
		virtual bool operator!=(long long other) const = 0;

		/** Compares if the current variant is different from a boolean.
		@param other The boolean to compare with.
		@return True if the string and boolean are different, false otherwise. */
		virtual bool operator!=(bool other) const = 0;
	};
}
#endif
