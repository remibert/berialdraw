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
		inline void width_(Dim w)
		{
			m_size.width_(w);
		}

		/** Set the height of the area with a precision of 64th of a pixel 
		@param h New height of the area */
		inline void height_(Dim h)
		{
			m_size.height_(h);
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
		inline void x_(Coord x)
		{
			m_position.x_(x);
		}

		/** Set the y coordinate of the area with a precision of 64th of a pixel 
		@param y New y coordinate of the area */
		inline void y_(Coord y)
		{
			m_position.y_(y);
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
		inline Dim width_() const
		{
			return m_size.width_();
		}

		/** Get the height of the area with a precision of 64th of a pixel 
		@return Height of the area */
		inline Dim height_() const
		{
			return m_size.height_();
		}

		/** Get the x coordinate of the area with a precision of 64th of a pixel 
		@return X coordinate of the area */
		inline Coord x_() const
		{
			return m_position.x_();
		}

		/** Get the y coordinate of the area with a precision of 64th of a pixel 
		@return Y coordinate of the area */
		inline Coord y_() const
		{
			return m_position.y_();
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


		/** Print content */
		void print(const char * name) const;

	protected:
/// @cond DOXYGEN_IGNORE
		Point m_position;
		Size m_size;
/// @endcond 
	};
}
