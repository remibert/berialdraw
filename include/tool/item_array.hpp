#pragma once
namespace berialdraw
{
	/** Json array */
	class ItemArray : public ItemCollection
	{
	public:
		/** Create json array */
		ItemArray();

		/** Destroy item json array */
		virtual ~ItemArray();

		/** Serialize to json */
		virtual void serialize(TextStream & out, int32_t indent=-1);

		/** Unserialize from json */
		virtual bool unserialize(TextStream & content);

		/** Get the child item at index */
		virtual Item * child(int32_t index);

		/** Item count in object */
		virtual int32_t count() const;

		/** Remove item at the index position */
		virtual bool remove(int32_t index);


		/** Append item into json array */
		void append(Item * item);

		/** Provides read access to element at the given index in the vector. */
		Item * operator[](int32_t index);

		/** Provides read access to element at the given index in the vector. */
		const Item * operator[](int32_t index) const;

		/** Set item into json object */
		void set(int32_t index, Item * value);

		/** Insert item in the array */
		void insert(int32_t index, Item * value);

		/** Replace the item at the index position by the new item */
		void replace(int32_t index, Item * item);

		/** Clear all items in array */
		void clear();

	protected:
/// @cond DOXYGEN_IGNORE
		Vector<Item *> m_array;
/// @endcond
	};
}
