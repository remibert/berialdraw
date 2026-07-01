#pragma once
namespace berialdraw
{
	/** Class that defines an area with size and position */
	class Area
	{
	public:
		/** Create area with position and size
		@param position Position of the area
		@param size Size of the area */
		Area(const Point& position, const Size& size);

		/** Create area with coordinates
		@param x X coordinate of the area
		@param y Y coordinate of the area
		@param w Width of the area
		@param h Height of the area
		@param pixel Indicates if the coordinates are in pixels */
		Area(Coord x = 0, Coord y = 0, Dim w = 0, Dim h = 0, bool pixel = true);

		/** Create a copy of area
		@param other Other area to copy from */
		inline Area(const Area& other)
		{
			m_size     = other.m_size;
			m_position = other.m_position;
		}

		/** Set the content of area with other area
		@param other Other area to copy from
		@return Reference to this Area */
		inline Area& operator=(const Area& other)
		{
			if (this != &other)
			{
				this->m_position = other.m_position;
				this->m_size     = other.m_size;
			}
			return *this;
		}

		/** Move the point by specified coordinates with a precision of 64th of a pixel 
		@param shift shift modification */
		inline void move(const Point & shift)
		{
			m_position.move(shift);
		}

		/**	Set the position and size of the area
		@param position New position of the area
		@param size New size of the area */
		inline void set(const Point& position, const Size& size)
		{
			m_position = position;
			m_size = size;
		}

		/** Set the width of the area with a precision of 64th of a pixel 
		@param w New width of the area */
		inline void width_q6(Dim w)
		{
			m_size.width_q6(w);
		}

		/** Set the height of the area with a precision of 64th of a pixel 
		@param h New height of the area */
		inline void height_q6(Dim h)
		{
			m_size.height_q6(h);
		}

		/** Set the width of the area in pixels
		@param w New width of the area */
		inline void width(Dim w)
		{
			m_size.width(w);
		}

		/** Set the height of the area in pixels
		@param h New height of the area */
		inline void height(Dim h)
		{
			m_size.height(h);
		}

		/** Set the x coordinate of the area with a precision of 64th of a pixel 
		@param x New x coordinate of the area */
		inline void x_q6(Coord x)
		{
			m_position.x_q6(x);
		}

		/** Set the y coordinate of the area with a precision of 64th of a pixel 
		@param y New y coordinate of the area */
		inline void y_q6(Coord y)
		{
			m_position.y_q6(y);
		}

		/** Set the x coordinate of the area in pixels
		@param x New x coordinate of the area */
		inline void x(Coord x)
		{
			m_position.x(x);
		}

		/** Set the y coordinate of the area in pixels
		@param y New y coordinate of the area */
		inline void y(Coord y)
		{
			m_position.y(y);
		}

		/** Get the width of the area with a precision of 64th of a pixel 
		@return Width of the area */
		inline Dim width_q6() const
		{
			return m_size.width_q6();
		}

		/** Get the height of the area with a precision of 64th of a pixel 
		@return Height of the area */
		inline Dim height_q6() const
		{
			return m_size.height_q6();
		}

		/** Get the x coordinate of the area with a precision of 64th of a pixel 
		@return X coordinate of the area */
		inline Coord x_q6() const
		{
			return m_position.x_q6();
		}

		/** Get the y coordinate of the area with a precision of 64th of a pixel 
		@return Y coordinate of the area */
		inline Coord y_q6() const
		{
			return m_position.y_q6();
		}

		/** Get the width of the area in pixels
		@return Width of the area */
		inline Dim width() const
		{
			return m_size.width();
		}

		/** Get the height of the area in pixels
		@return Height of the area */
		inline Dim height() const
		{
			return m_size.height();
		}

		/** Get the x coordinate of the area in pixels
		@return X coordinate of the area */
		inline Coord x() const
		{
			return m_position.x();
		}

		/** Get the y coordinate of the area in pixels
		@return Y coordinate of the area */
		inline Coord y() const
		{
			return m_position.y();
		}

		/** Resizes and move itself on the nearest pixel */
		void nearest_pixel();

		/** Destructor */
		~Area();

		/** Decrease size and position of area with margin
		@param margin Margin to decrease by */
		void decrease(const Margin& margin);

		/** Increase size and position of area with margin
		@param margin Margin to increase by */
		void increase(const Margin& margin);

		/** Decrease the thickness border of the size and position in the area
		@param thickness thickess to remove */
		void decrease_thickness(Dim thickness);


		/** Return position of area according to the shift
		@return Position of the area */
		inline const Point& position() const
		{
			return m_position;
		}

		/** Return size of area
		@return Size of the area */
		inline const Size& size() const
		{
			return m_size;
		}

		/** Return position of area according to the shift
		@return Position of the area */
		inline Point& position()
		{
			return m_position;
		}

		/** Return size of area
		@return Size of the area */
		inline Size& size()
		{
			return m_size;
		}

		/**	Set the size of the area
		@param size New size of the area */
		inline void size(const Size& size)
		{
			m_size = size;
		}

		/**	Set the position of the area
		@param position New position of the area */
		inline void position(const Point& position)
		{
			m_position = position;
		}

		/** Clip this area against another area (clip_area)
		@param clip_area The clipping area that defines the visible portion
		This function adjusts position and size:
		- If position x,y is less than clip_area's position, it becomes clip_area's position
		- If position + size exceeds clip_area's bounds, size is reduced to fit within clip_area */
		void clip(const Area& clip_area);

		/** Check if area is not empty (width > 0 and height > 0)
		@return true if area has non-zero width and height */
		inline bool is_not_empty() const
		{
			return m_size.width_q6() > 0 && m_size.height_q6() > 0;
		}

		/** Fast check if this area is completely outside another area (no overlap)
		@param other The area to check against
		@return true if this area is completely outside (no visible portion), false otherwise */
		inline bool is_outside(const Area& other) const
		{
			// Get coordinates and bounds for this area
			Coord this_x1 = x_q6();
			Coord this_y1 = y_q6();
			Coord this_x2 = this_x1 + width_q6();
			Coord this_y2 = this_y1 + height_q6();
			
			// Get coordinates and bounds for other area
			Coord other_x1 = other.x_q6();
			Coord other_y1 = other.y_q6();
			Coord other_x2 = other_x1 + other.width_q6();
			Coord other_y2 = other_y1 + other.height_q6();
			
			// Check if this area is completely to the right, left, below or above the other area
			// This handles negative coordinates correctly
			return (this_x2 <= other_x1) ||		// This is to the right of other
			       (this_x1 >= other_x2) ||		// This is to the left of other
			       (this_y2 <= other_y1) ||		// This is below other
			       (this_y1 >= other_y2);		// This is above other
		}

		/** Print content */
		void print(const char * name) const;

	protected:
/// @cond DOXYGEN_IGNORE
		Point m_position;
		Size m_size;
/// @endcond 
	};
}


