#pragma once
namespace berialdraw
{
	/** Json boolean */
	class ItemBool : public ItemVariant
	{
	public:
		/** Create new item boolean */
		ItemBool(bool value=false);

		/** Destructor */
		virtual ~ItemBool();

		/** Serialize to json */
		virtual void serialize(TextStream & out, int32_t indent=-1);

		/** Unserialize from json */
		virtual bool unserialize(TextStream & content);

		/** Convert to string */
		// virtual operator const char *() const;

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


		/** Compares if the current boolean is equal to another C-style string.
		@param other The C-style string to compare with.
		@return True if the strings are equal, false otherwise. */
		virtual bool operator==(const char * other) const;

		/** Compares if the current boolean is equal to another String object.
		@param other The String object to compare with.
		@return True if the strings are equal, false otherwise. */
		virtual bool operator==(const String & other) const;

		/** Compares if the current boolean is equal to an integer.
		@param other The integer to compare with.
		@return True if the string and integer are equal, false otherwise. */
		virtual bool operator==(int other) const;

		/** Compares if the current boolean is equal to an unsigned integer.
		@param other The unsigned integer to compare with.
		@return True if the string and unsigned integer are equal, false otherwise. */
		virtual bool operator==(unsigned int other) const;

		/** Compares if the current boolean is equal to a long long integer.
		@param other The long long integer to compare with.
		@return True if the string and long long integer are equal, false otherwise. */
		virtual bool operator==(long long other) const;

		/** Compares if the current boolean is equal to a boolean.
		@param other The boolean to compare with.
		@return True if the string and boolean are equal, false otherwise. */
		virtual bool operator==(bool other) const;

		/** Compares if the current boolean is different from another C-style string.
		@param other The C-style string to compare with.
		@return True if the strings are different, false otherwise. */
		virtual bool operator!=(const char * other) const;

		/** Compares if the current boolean is different from another String object.
		@param other The String object to compare with.
		@return True if the strings are different, false otherwise. */
		virtual bool operator!=(const String & other) const;

		/** Compares if the current boolean is different from an integer.
		@param other The integer to compare with.
		@return True if the string and integer are different, false otherwise. */
		virtual bool operator!=(int other) const;

		/** Compares if the current boolean is different from an unsigned integer.
		@param other The unsigned integer to compare with.
		@return True if the string and unsigned integer are different, false otherwise.  */
		virtual bool operator!=(unsigned int other) const;

		/** Compares if the current boolean is different from a long long integer.
		@param other The long long integer to compare with.
		@return True if the string and long long integer are different, false otherwise. */
		virtual bool operator!=(long long other) const;

		/** Compares if the current boolean is different from a boolean.
		@param other The boolean to compare with.
		@return True if the string and boolean are different, false otherwise. */
		virtual bool operator!=(bool other) const;

		/** Check if true */
		static bool is_true(const char * other);

	protected:
/// @cond DOXYGEN_IGNORE
		bool m_value;
/// @endcond
	};
}
