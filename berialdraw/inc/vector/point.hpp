#pragma once
namespace berialdraw
{
	/** Class that defines a position in a plane */
	class Point
	{
	public:
		/** Constructor */
		Point();

		/** Constructor with individual coordinates.
		@param x X coordinate.
		@param y Y coordinate.
		@param pixel Indicates if the values are in pixel units. */
		Point(Coord x, Coord y, bool pixel=true);

		/** Copy constructor.
		@param p Point to copy. */
		Point(const Point& p);

		/** Constructor with size.
		@param s Size to initialize the point. */
		Point(const Size& s);

		/** Equality operator.
		@param other Point to compare with.
		@return True if points are equal, false otherwise. */
		inline bool operator==(const Point& other) const
		{
			return m_x == other.m_x && m_y == other.m_y;
		}

		/** Inequality operator.
		@param other Point to compare with.
		@return True if points are not equal, false otherwise. */
		inline bool operator!=(const Point& other) const
		{
			return m_x != other.m_x || m_y != other.m_y;
		}

		/** Assignment operator.
		@param other Point to assign from.
		@return Reference to this point. */
		inline Point& operator=(const Point& other)
		{
			if (this != &other)
			{
				this->m_x = other.m_x;
				this->m_y = other.m_y;
				this->m_x_undefined = other.m_x_undefined;
				this->m_y_undefined = other.m_y_undefined;
			}
			return *this;
		}

		/** Move the point by another point.
		@param p Point to move by. */
		void move(const Point& p);

		/** Modify point by subtracting from it the coordinates of another point.
		@param position Point to subtract from current coordinates. */
		void diff(const Point& position);

		/** Set the coordinates.
		@param x_ X coordinate.
		@param y_ Y coordinate. */
		inline void set(Coord x_, Coord y_)
		{
			x(x_);
			y(y_);
		}

		/** Set the y coordinate.
		@param y_ Y coordinate. */
		inline void y(Coord y_)
		{
			m_y_undefined = 0;
			m_y = y_ << 6;
		}

		/** Set the x coordinate.
		@param x_ X coordinate. */
		inline void x(Coord x_)
		{
			m_x_undefined = 0;
			m_x = x_ << 6;
		}

		/** Move the point by specified coordinates.
		@param x_ X coordinate to move by.
		@param y_ Y coordinate to move by. */
		inline void move(Coord x_, Coord y_)
		{
			m_x_undefined = 0;
			m_y_undefined = 0;
			m_x += x_ << 6;
			m_y += y_ << 6;
		}

		inline Coord y() const
		{
			return (m_y+32) >> 6;
		}

		/** Get the x coordinate.
		@return X coordinate. */
		inline Coord x() const
		{
			return (m_x+32) >> 6;
		}

		/** Set the coordinates with a precision of 64th of a pixel 
		@param x X coordinate.
		@param y Y coordinate. */
		inline void set_(Coord x, Coord y)
		{
			m_x_undefined = 0;
			m_y_undefined = 0;
			m_x = x;
			m_y = y;
		}

		/** Move the point by specified coordinates with a precision of 64th of a pixel 
		@param x_ X coordinate to move by.
		@param y_ Y coordinate to move by. */
		inline void move_(Coord x_, Coord y_)
		{
			m_x_undefined = 0;
			m_y_undefined = 0;
			m_x += x_;
			m_y += y_;
		}

		/** Get the y coordinate with a precision of 64th of a pixel 
		@return Y coordinate. */
		inline Coord y_() const
		{
			return m_y;
		}

		/** Get the x coordinate with a precision of 64th of a pixel 
		@return X coordinate. */
		inline Coord x_() const
		{
			return m_x;
		}

		/** Set the y coordinate with a precision of 64th of a pixel 
		@param y Y coordinate. */
		inline void y_(Coord y)
		{
			m_y_undefined = 0;
			m_y = y;
		}

		/** Set the x coordinate with a precision of 64th of a pixel 
		@param x X coordinate. */
		inline void x_(Coord x)
		{
			m_x_undefined = 0;
			m_x = x;
		}

		/** Serialize the point to JSON.
		@param name Name of the JSON field.
		@param it JSON iterator. */
		void serialize(const char* name, JsonIterator& it) const;

		/** Unserialize the point from JSON.
		@param name Name of the JSON field.
		@param it JSON iterator.*/
		void unserialize(const char* name, JsonIterator& it);

		/** Positions itself on the nearest pixel */
		void nearest_pixel()
		{
			m_x_undefined = 0;
			m_y_undefined = 0;
			m_x = ((m_x + 32) >> 6) << 6;
			m_y = ((m_y + 32) >> 6) << 6;
		}

		/** Indicates if x is not defined */
		bool is_x_undefined() const
		{
			return m_x_undefined;
		}

		/** Indicates if y is not defined */
		bool is_y_undefined() const
		{
			return m_y_undefined;
		}

		/** Print content */
		void print(const char * name) const;

		/** Adapt point to the UIManager scale */
		void adapt_scale();

	protected:
/// @cond DOXYGEN_IGNORE
		/** x coordinate with a precision of 64th of a pixel */
		int m_x:31;

		/** Indicates that x is not yet defined */
		unsigned int m_x_undefined:1;

		/** y coordinate with a precision of 64th of a pixel */
		int m_y:31;

		/** Indicates that y is not yet defined */
		unsigned int m_y_undefined:1;
/// @endcond
	};
}
