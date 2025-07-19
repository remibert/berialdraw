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

		/** Destructor. */
		~Point();

		/** Equality operator.
		@param other Point to compare with.
		@return True if points are equal, false otherwise. */
		bool operator==(const Point& other) const;

		/** Inequality operator.
		@param other Point to compare with.
		@return True if points are not equal, false otherwise. */
		bool operator!=(const Point& other) const;

		/** Assignment operator.
		@param other Point to assign from.
		@return Reference to this point. */
		Point& operator=(const Point& other);

		/** Move the point by another point.
		@param p Point to move by. */
		void move(const Point& p);

		/** Set the coordinates.
		@param x X coordinate.
		@param y Y coordinate. */
		void set(Coord x, Coord y);

		/** Set the y coordinate.
		@param y_ Y coordinate. */
		void y(Coord y_);

		/** Set the x coordinate.
		@param x_ X coordinate. */
		void x(Coord x_);

		/** Move the point by specified coordinates.
		@param x_ X coordinate to move by.
		@param y_ Y coordinate to move by. */
		void move(Coord x_, Coord y_);

		/** Get the y coordinate.
		@return Y coordinate. */
		Coord y() const;

		/** Get the x coordinate.
		@return X coordinate. */
		Coord x() const;

		/** Set the coordinates with a precision of 64th of a pixel 
		@param x X coordinate.
		@param y Y coordinate. */
		void set_(Coord x, Coord y);

		/** Move the point by specified coordinates with a precision of 64th of a pixel 
		@param x_ X coordinate to move by.
		@param y_ Y coordinate to move by. */
		void move_(Coord x_, Coord y_);

		/** Get the y coordinate with a precision of 64th of a pixel 
		@return Y coordinate. */
		Coord y_() const;

		/** Get the x coordinate with a precision of 64th of a pixel 
		@return X coordinate. */
		Coord x_() const;

		/** Set the y coordinate with a precision of 64th of a pixel 
		@param y Y coordinate. */
		void y_(Coord y);

		/** Set the x coordinate with a precision of 64th of a pixel 
		@param x X coordinate. */
		void x_(Coord x);

		/** Serialize the point to JSON.
		@param name Name of the JSON field.
		@param it JSON iterator. */
		void serialize(const char* name, JsonIterator& it) const;

		/** Unserialize the point from JSON.
		@param name Name of the JSON field.
		@param it JSON iterator.*/
		void unserialize(const char* name, JsonIterator& it);

		/** Positions itself on the nearest pixel */
		void nearest_pixel();

		/** Indicates if x is not defined */
		bool is_x_undefined() const;

		/** Indicates if y is not defined */
		bool is_y_undefined() const;

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
