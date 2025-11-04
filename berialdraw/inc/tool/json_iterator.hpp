#pragma once
namespace berialdraw
{
	class Item;
	class ItemCollection;
	class ItemVariant;
	class Json;

	/** Data type handled by json */
	enum class JsonType
	{
		UNDEFINED, ///< Undefined type
		INT,       ///< Integer type
		BOOL,      ///< Boolean type
		STRING,    ///< String type
		ARRAY,     ///< Array type
		OBJECT,    ///< Object type
		NONE       ///< Null or none type
	};

	/** This class allows you to iterate through a json data tree */
	class JsonIterator
	{
	public:
/// @cond DOXYGEN_IGNORE
		friend Json;
/// @endcond

		/** Init with root of json
		@param json Reference to the Json object */
		JsonIterator(Json& json);

		/** Init with not root of json
		@param json Reference to the Json object
		@param parent Pointer to the parent JsonIterator
		@param item Pointer to the current item
		@param index Index of the item */
		JsonIterator(Json& json, JsonIterator* parent, Item* item, int32_t index);

		/** Init with not root of json
		@param json Reference to the Json object
		@param parent Pointer to the parent JsonIterator
		@param item Pointer to the current item
		@param key Key of the item */
		JsonIterator(Json& json, JsonIterator* parent, Item* item, const char* key);

		/** Destructor */
		virtual ~JsonIterator();

		/** Indicates if the current node has a child
		@return true if the current node has a child, false otherwise */
		virtual bool has_child();

		/** Create iterator on child items
		@return JsonIterator for the child items */
		virtual JsonIterator child();

		/** Select the next item in node json selected
		@return true if the next item is selected, false otherwise */
		virtual bool next();

		/** Select the previous item in node json selected
		@return true if the previous item is selected, false otherwise */
		virtual bool previous();

		/** Select the last item in node json selected
		@return true if the last item is selected, false otherwise */
		virtual bool last();

		/** Select the first item in node json selected
		@return true if the first item is selected, false otherwise */
		virtual bool first();

		/** Indicates if the current node exists
		@return true if the current node exists, false otherwise */
		virtual bool exist();

		/** Get the count of items in the current node
		@return Number of items in the current node */
		virtual int32_t count();

		/** Get the key
		@return Reference to the key as a String */
		virtual const String& key() const;

		/** Get variant value in the json
		@return Reference to the ItemVariant */
		virtual ItemVariant& value();

		/** Return the type of node found in path selected
		@return JsonType of the current node */
		virtual enum JsonType type();

		/** Convert to string
		@return String representation of the current node */
		virtual operator const String() const;

		/** Convert to boolean
		@return Boolean representation of the current node */
		virtual operator bool() const;

		/** Convert to integer
		@return Integer representation of the current node */
		virtual operator int() const;

		/** Convert to unsigned integer
		@return Unsigned integer representation of the current node */
		virtual operator unsigned int() const;

		/** Convert to long long
		@return Long long representation of the current node */
		virtual operator long long() const;

		/** Indicates if the item is null
		@return true if the item is null, false otherwise */
		virtual bool is_null() const;

		/** Remove current item
		@return true if the item is removed, false otherwise */
		virtual bool remove();

		/** Provides read access to element at the given index in the vector
		@param index Index of the element
		@return JsonIterator for the element at the given index*/
		JsonIterator operator[](int index); 

		/** Provides read access to element at the given key in the vector
		@param key Key of the element
		@return JsonIterator for the element at the given key */
		JsonIterator operator[](const char* key);

		/** Convert variant to integer
		@param variant Pointer to the ItemVariant
		@return Integer representation of the variant */
		long long to_integer(const ItemVariant* variant) const;

		/** Set value in json tree
		@param value String value to set
		@return Reference to this JsonIterator */
		JsonIterator& operator=(const String& value);

		/** Set value in json tree
		@param value C-string value to set
		@return Reference to this JsonIterator */
		JsonIterator& operator=(const char* value);

		/** Set value in json tree
		@param value Integer value to set
		@return Reference to this JsonIterator */
		JsonIterator& operator=(int value);

		/** Set value in json tree
		@param value Unsigned integer value to set
		@return Reference to this JsonIterator */
		JsonIterator& operator=(unsigned int value);

		/** Set value in json tree
		@param value Long long value to set
		@return Reference to this JsonIterator */
		JsonIterator& operator=(long long value);

		/** Set value in json tree
		@param value Boolean value to set
		@return Reference to this JsonIterator */
		JsonIterator& operator=(bool value);

		/** Compare if equal to a C-string
		@param other C-string to compare
		@return true if equal, false otherwise */
		bool operator==(const char* other) const;

		/** Compare if equal to a String
		@param other String to compare
		@return true if equal, false otherwise */
		bool operator==(const String& other) const;

		/** Compare if equal to an integer
		@param other Integer to compare
		@return true if equal, false otherwise */
		bool operator==(int other) const;

		/** Compare if equal to an unsigned integer
		@param other Unsigned integer to compare
		@return true if equal, false otherwise */
		bool operator==(unsigned int other) const;

		/** Compare if equal to a long long
		@param other Long long to compare
		@return true if equal, false otherwise */
		bool operator==(long long other) const;

		/** Compare if equal to a boolean
		@param other Boolean to compare
		@return true if equal, false otherwise */
		bool operator==(bool other) const;

		/** Compare if different from a C-string
		@param other C-string to compare
		@return true if different, false otherwise */
		bool operator!=(const char* other) const;

		/** Compare if different from a String
		@param other String to compare
		@return true if different, false otherwise */
		bool operator!=(const String& other) const;

		/** Compare if different from an integer
		@param other Integer to compare
		@return true if different, false otherwise */
		bool operator!=(int other) const;

		/** Compare if different from an unsigned integer
		@param other Unsigned integer to compare
		@return true if different, false otherwise */
		bool operator!=(unsigned int other) const;

		/** Compare if different from a long long
		@param other Long long to compare
		@return true if different, false otherwise */
		bool operator!=(long long other) const;

		/** Compare if different from a boolean
		@param other Boolean to compare
		@return true if different, false otherwise */
		bool operator!=(bool other) const;

		/** Manage the default value if missing
		@param other Default C-string value
		@return Value or default if missing */
		String operator|(const char* other) const;

		/** Manage the default value if missing
		@param other Default String value
		@return Value or default if missing */
		String operator|(const String& other) const;

		/** Manage the default value if missing
		@param other Default integer value
		@return Value or default if missing */
		int operator|(int other) const;

		/** Manage the default value if missing
		@param other Default unsigned integer value
		@return Value or default if missing */
		int operator|(unsigned int other) const;

		/** Manage the default value if missing
		@param other Default long long value
		@return Value or default if missing */
		long long operator|(long long other) const;

		/** Manage the default value if missing
		@param other Default boolean value
		@return Value or default if missing */
		bool operator|(bool other) const;

		/** Copy assignment operator
		@param other Other JsonIterator to copy from
		@return Reference to this JsonIterator */
		JsonIterator& operator=(const JsonIterator& other);

		/** Copy constructor
		@param other Other JsonIterator to copy from */
		JsonIterator(const JsonIterator& other);

	protected:
#ifndef SWIG
/// @cond DOXYGEN_IGNORE
		/** Check if the key is accurate
		@param key Key to check
		@return true if the key is accurate, false otherwise */
		bool is_accurate(const char* key);

		/** Set the current item
		@param newowe New item to set */
		void set(Item* newowe);

		/** Get the current item
		@return Pointer to the current item */
		Item* get();

		/** Get the current item (const)
		@return Pointer to the current item */
		const Item* get() const;

		Json& m_json;
		int32_t m_index = 0;
		Item* m_item = 0;
		Item* m_item_tmp = 0;
		JsonIterator* m_parent = 0;
		String m_key;
		unsigned int m_with_index : 1;
		unsigned int m_destroyed : 1;
		unsigned int m_accurate : 1;
/// @endcond
#endif
	};
};
