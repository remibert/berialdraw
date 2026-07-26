#pragma once
namespace berialdraw
{
	/** Image fit mode enumeration. Defines how the image adapts to the widget area. */
	enum ImageFitMode
	{
		STRETCH          = 0, /**< Stretch the image to fill the entire area (ignores aspect ratio) */
		FIT              = 1, /**< Preserve aspect ratio, fit to available area */
	};

	/** Serialize the image fit mode to JSON.
	@param name Name of the JSON field.
	@param it JSON iterator. */
	void serialize(const char * name, JsonIterator & it, ImageFitMode value);

	/** Unserialize the image fit mode from JSON.
	@param name Name of the JSON field.
	@param it JSON iterator.*/
	void unserialize(const char * name, JsonIterator & it, ImageFitMode & value);

}
