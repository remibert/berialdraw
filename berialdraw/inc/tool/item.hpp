#ifndef Item_hpp_INCLUDED
#define Item_hpp_INCLUDED
namespace berialdraw
{
	/** Generic json item */
	class Item
	{
	public:
		/** Destructor */
		virtual ~Item(){}

		/** Serialize json into string */
		virtual void serialize(TextStream & out, int32_t indent) = 0;

		/** Unserialize json from string */
		virtual bool unserialize(TextStream & content)=0;

		/** Return the type of node found in path selected */
		static enum JsonType type(Item * item);

		/** Convert the type of item into string */
		static const char * typestr(Item *item);
	};
}
#endif