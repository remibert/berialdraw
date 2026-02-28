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
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

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

	protected:
/// @cond DOXYGEN_IGNORE
		Size             m_scroll_size;
		Point            m_scroll_position;
		ScrollDirection  m_scroll_direction = SCROLL_VERTICAL;
		Size             m_viewport_size;
/// @endcond
	}; 
}
