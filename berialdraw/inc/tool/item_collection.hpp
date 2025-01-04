#ifndef ItemCollection_hpp_INCLUDED
#define ItemCollection_hpp_INCLUDED
namespace berialdraw
{
	/** Generic item collection */
	class ItemCollection : public Item
	{
	public:
		/** Destructor */
		virtual ~ItemCollection(){}

		/** Serialize json into string */
		virtual void serialize(TextStream & out, int32_t indent) = 0;

		/** Unserialize json from string */
		virtual bool unserialize(TextStream & content)=0;

		/** Item count in object */
		virtual int32_t count() const = 0;

		/** Provides read access to element at the given index in the vector. */
		virtual Item * operator[](int32_t index) = 0;

		/** Remove selected item */
		virtual bool remove(int32_t index) = 0;

		/** Get the child item at index */
		virtual Item * child(int32_t index) = 0;
	};
}
#endif