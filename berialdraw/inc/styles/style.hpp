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

		/** Get the property name for this style (e.g., "widget", "common", "text") */
		virtual const char* property_name() const = 0;
};

	/** Function pointer definition to create dedicated style */
	typedef Style * (*StyleCreator_t)();
}
