#pragma once
namespace berialdraw
{
	// Forward declaration to avoid circular dependency
	class StyleItem;

	/** The Style class serves as the foundational structure for all specific style definitions, 
	such as BorderStyle, SliderStyle, and TextStyle. It provides a common interface and shared 
	properties for customizing widget appearance. This class is designed to be extended, allowing 
	derived classes to focus on styling attributes specific to their respective widgets. */
	class Style
	{
	public:
		/** Destructor */
		virtual ~Style(){}

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it) = 0;

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it) = 0;

		/** Apply selective style properties from a StyleItem 
		Unlike unserialize which loads from JSON with defaults, 
		apply_style only modifies properties that are explicitly defined in the StyleItem.
		Properties not present in the source style remain unchanged. */
		virtual void apply_style(StyleItem* item) = 0;
	};

	/** Function pointer definition to create dedicated style */
	typedef Style * (*StyleCreator_t)();
}
