#pragma once
namespace berialdraw
{
	/** Json object */
	class ItemObject : public ItemCollection
	{
	public:
		/** Create json object */
		ItemObject();

		/** Destroy item json object */
		virtual ~ItemObject();

		/** Serialize to json */
		virtual void serialize(TextStream & out, int32_t indent=-1);

		/** Unserialize from json */
		virtual bool unserialize(TextStream & content);

		/** Get the child item at index */
		virtual Item * child(int32_t index);

		/** Item count in object */
		virtual int32_t count() const;

		/** Remove selected item */
		virtual bool remove(int32_t index);

		/** Remove selected item */
		bool remove(const char * key);

		/** Set item into json object */
		void set(const char * key, Item * value);

		/** Set item into json object */
		void set(int32_t index, Item * value);

		/** Get key at index */
		ItemString * key(int32_t index) const;

		/** Provides read access to element at the given index in the vector. */
		Item * operator[](int32_t index);

		/** Provides read access to element at the given index in the vector. */
		const Item * operator[](int32_t index) const;

		/** Provides read access to element at the given index in the vector. */
		Item * operator[](const char * key);

		/** Provides read access to element at the given index in the vector. */
		const Item * operator[](const char * key) const;

		/** Clear all items in object */
		void clear();

	protected:
/// @cond DOXYGEN_IGNORE
		Vector<ItemString *> m_keys;
		Vector<Item       *> m_values;
/// @endcond
	};
}
