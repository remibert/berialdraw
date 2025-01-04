#ifndef String_hpp_INCLUDED
#define String_hpp_INCLUDED

namespace berialdraw
{
	/** Utf8 string management */
	class String : public TextStream
	{
	public:
		/** Constructor */
		String();

		/** Destructor */
		virtual ~String();

		/** Copy constructor
		@param string String to copy from */
		String(const String & string);
	
		/** Constructor with string buffer
		@param string String buffer to initialize from */
		String(const char * string);

		/** Assign operator redefinition
		@param other String to assign from
		@return Reference to this String */
		String& operator=(const String& other);

		/** Assign operator redefinition
		@param other String buffer to assign from
		@return Reference to this String */
		String& operator=(const char * other);

		/** Assign operator redefinition
		@param value Wide character to assign from
		@return Reference to this String */
		String& operator=(wchar_t value);

		/** Cast to char pointer
		@return Pointer to the string buffer */
		operator const char *() const;

		/** Compare if equal string
		@param other String to compare with
		@return True if equal, false otherwise */
		bool operator==(const String& other) const;
		
		/** Compare if equal string buffer
		@param other String buffer to compare with
		@return True if equal, false otherwise */
		bool operator==(const char * other) const;

		/** Compare if different string
		@param other String to compare with
		@return True if different, false otherwise */
		bool operator!=(const String& other) const;
		
		/** Compare if different string buffer
		@param other String buffer to compare with
		@return True if different, false otherwise */
		bool operator!=(const char * other) const;

		/** Concatenate two strings
		@param other String buffer to concatenate
		@return Concatenated String */
		String operator+(const char * other) const;

		/** Concatenate two strings
		@param other String to concatenate
		@return Concatenated String */
		String operator+(const String &other) const; 

		/** Concatenate two strings
		@param other Wide character to concatenate
		@return Concatenated String */
		String operator+(wchar_t other) const;

		/** Append to current string
		@param other String buffer to append
		@return Reference to this String */
		String & operator+=(const char * other);

		/** Append to current string
		@param other String to append
		@return Reference to this String */
		String & operator+=(const String &other);

		/** Append to current string
		@param other Wide character to append
		@return Reference to this String */
		String & operator+=(wchar_t other);

		/** Find string in the string
		@param str string to search
		@param pos start index in the string
		@return index in the string where the search string was found or INT32_MAX if not found */
		int32_t find(const char * str, int32_t pos = 0) const;

		/** Find string in the string in reverse
		@param str string to search
		@param pos start index in the string
		@return index in the string where the search string was found or INT32_MAX if not found */
		int32_t rfind(const char * str, int32_t pos = -1) const;

		/** Return the string buffer pointer
		@return Pointer to the string buffer */
		const char *c_str() const;

		/** Get size in bytes of the string buffer
		@return Size in bytes of the string buffer */
		uint32_t size() const;

		/** Get the quantity of wide characters in the string
		@return Number of wide characters */
		uint32_t count() const;

		/** Get the wide character at the index
		@param index Index of the character
		@return Wide character at the specified index */
		wchar_t get(int32_t index) const;

		/** Printf into string with va_list
		@param format Format string
		@param args Variable arguments list */
		void vprint(const char *format, va_list args);

		/** Printf into string clear previous string
		@param format Format string */
		void print(const char *format, ...);

		/** Insert character at position
		@param character Character to insert
		@param index Position to insert at */
		void insert(wchar_t character, int32_t index);

		/** Insert string at position
		@param string String buffer to insert
		@param index Position to insert at */
		void insert(const char * string, int32_t index);

		/** Insert string at position
		@param string String to insert
		@param index Position to insert at */
		void insert(const String & string, int32_t index);

		/** Replace character at position
		@param character Character to replace
		@param index Position to replace at */
		void replace(wchar_t character, int32_t index);

		/** Remove character at position
		@param index Position to remove from */
		void remove(int32_t index);

		/** Get a slice of string 
		@param start start position
		@param end  end position
		@param part slice extracted */
		void slice(int32_t start, int32_t end, String & part) const;

		/** Remove a slice of string 
		@param start start position
		@param end  end position */
		void remove(int32_t start, int32_t end);

		/** To integer
		@param value Reference to store the integer value
		@return True if conversion is successful, false otherwise */
		bool to_int(int32_t & value) const;

		/** To integer
		@param value Reference to store the integer value
		@return True if conversion is successful, false otherwise */
		bool to_int(int64_t & value) const;

		/** To integer 
		@param str String buffer to convert
		@param value Reference to store the integer value
		@return True if conversion is successful, false otherwise */
		static bool to_int(const char * str, int & value); 

		/** To integer 
		@param str String buffer to convert
		@param value Reference to store the integer value
		@return True if conversion is successful, false otherwise */
		static bool to_int(const char * str, long long & value); 

		/** Write a wide character to the end of the current text stream
		@param character Wide character
		@return Number of bytes written, or 0 if an error occurred */
		virtual uint32_t write_char(wchar_t character);

		/** Write a string buffer to the end of the current text stream
		@param buffer Text to append
		@return Number of bytes written, or 0 if an error occurred */
		virtual uint32_t write_string(const char * buffer);

		/** Clear the content of stream */
		virtual void clear();

		/** Get the current offset in bytes into the string buffer
		@return Current offset in buffer */
		virtual uint32_t offset();

		/** Set the offset in bytes into the string buffer
		@param val Offset to set */
		virtual void offset(uint32_t val);

		/** Read the wide character next
		@return Wide character read or null if it ends */
		virtual wchar_t read_char();

#ifdef _DEBUG
		/** Unitary test */
		static void test(); 
		static void test1(); 
		static void test2(); 
		static void test3(); 
		static void test4(); 
		static void test5(); 
		static void test6(); 
#endif
		/** Empty string */
		static const String empty;

	protected:
/// @cond DOXYGEN_IGNORE
		/** Allocate temporary string
		@param length String length to allocate
		@return Allocated temporary pointer */
		virtual char * tmp_alloc(uint32_t length);

		/** Deallocate temporary string
		@param tmp Allocated temporary pointer
		@param length String length to deallocate */
		virtual void tmp_dealloc(char * tmp, uint32_t length);

		/** Append string buffer into the current string
		@param string String buffer to append */
		void append(const char * string);

		/** Append string into the current string
		@param other String to append */
		void append(const String & other);

		/** Append wide character into the current string
		@param character Wide character to append */
		void append(wchar_t character);
	
		/** Get wide character at offset
		@param index wide char index to get character from
		@param pos Position in bytes into the string buffer
		@return Wide character at specified index */
		wchar_t offset(int32_t index, uint32_t & pos) const;

		/** Allocate string with size
		@param change_size Size to allocate */
		void alloc(uint32_t change_size);

		char * m_string = 0;   /**< Pointer on string buffer */
		uint32_t m_capacity = 0; /**< Capacity in bytes reserved for string */
		uint32_t m_size = 0;     /**< Size in bytes of the string buffer */
		uint32_t m_offset = 0;   /**< Read or write offset in bytes into the string buffer */
/// @endcond
	};
}

#endif
