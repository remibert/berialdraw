#pragma once
#include "../vector/scroll_direction.hpp"

namespace berialdraw
{
	/** The ScrollViewStyle class specifies the appearance of scrollviews, including the scroll 
	size, position, direction and viewport size. */
	class ScrollViewStyle : public Style
	{
	public:
		/** Constructor */
		ScrollViewStyle();


		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it) override;

		/** Copy operator */
		ScrollViewStyle& operator=(const ScrollViewStyle& other);

		/** Copy properties from another without invalidation */
		void copy_from(const ScrollViewStyle & other);

		/** Set properties with another */
		void set(const ScrollViewStyle & other);

		/** Create new scrollview style */
		static Style * create();


		/** Get the scroll direction */
		ScrollDirection scroll_direction() const { return m_scroll_direction; }

		/** Set the scroll direction */
		void scroll_direction(ScrollDirection dir);

		/** Get the padding */
		const Margin& padding() const { return m_padding; }

		/** Set the padding */
		void padding(const Margin& m);

		/** Set the padding in pixels */
		void padding(Dim top, Dim left, Dim bottom, Dim right);

		/** Set the padding with a precision of 64th of a pixel */
		void padding_(Dim top, Dim left, Dim bottom, Dim right);

		/** Set the padding */
		void padding(Dim horizontal, Dim vertical);

		/** Set the padding */
		void padding(Dim value);


		/** Get the property name for this style */
		const char* property_name() const override { return "scrollview"; }

	protected:
/// @cond DOXYGEN_IGNORE
		Size             m_scroll_size;
		Point            m_scroll_position;
		ScrollDirection  m_scroll_direction = SCROLL_VERTICAL;
		Margin m_padding = { 0,0,0,0 };
/// @endcond
	}; 
}
