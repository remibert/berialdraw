#pragma once
namespace berialdraw
{
	/** Enumeration for scroll direction */
	enum ScrollDirection
	{
		SCROLL_HORIZONTAL = 0,
		SCROLL_VERTICAL = 1,
		SCROLL_ALL_DIRECTIONS = 2
	};

	/** The ScrollViewStyle class specifies the appearance of scrollviews, including the scroll 
	size, position, direction and viewport size. */
	class ScrollViewStyle : public Style
	{
	public:
		/** Constructor */
		ScrollViewStyle();
	
		/** Destructor */
		~ScrollViewStyle(){}

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Copy operator */
		ScrollViewStyle& operator=(const ScrollViewStyle& other);

		/** Set properties with another */
		void set(const ScrollViewStyle & other);

		/** Create new scrollview style */
		static Style * create();


		/** Get the scroll direction */
		ScrollDirection scroll_direction() const;

		/** Set the scroll direction */
		void scroll_direction(ScrollDirection dir);

	protected:
#ifndef SWIG
/// @cond DOXYGEN_IGNORE
		Size             m_scroll_size;
		Point            m_scroll_position;
		ScrollDirection  m_scroll_direction = SCROLL_VERTICAL;
		Size             m_viewport_size;
/// @endcond
#endif
	}; 
}
