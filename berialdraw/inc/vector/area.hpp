#ifndef Area_hpp_INCLUDED
#define Area_hpp_INCLUDED

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
		Area(const Area& other);

		/** Set the content of area with other area
		@param other Other area to copy from
		@return Reference to this Area */
		Area& operator=(const Area& other);

		/** Move the point by specified coordinates with a precision of 64th of a pixel 
		@param shift shift modification */
		void move(const Point & shift);

		/**	Set the position and size of the area
		@param position New position of the area
		@param size New size of the area */
		void set(const Point& position, const Size& size);

		/** Set the width of the area with a precision of 64th of a pixel 
		@param w New width of the area */
		void width_(Dim w);

		/** Set the height of the area with a precision of 64th of a pixel 
		@param h New height of the area */
		void height_(Dim h);

		/** Set the width of the area in pixels
		@param w New width of the area */
		void width(Dim w);

		/** Set the height of the area in pixels
		@param h New height of the area */
		void height(Dim h);


		/** Set the x coordinate of the area with a precision of 64th of a pixel 
		@param x New x coordinate of the area */
		void x_(Coord x);

		/** Set the y coordinate of the area with a precision of 64th of a pixel 
		@param y New y coordinate of the area */
		void y_(Coord y);

		/** Set the x coordinate of the area in pixels
		@param x New x coordinate of the area */
		void x(Coord x);

		/** Set the y coordinate of the area in pixels
		@param y New y coordinate of the area */
		void y(Coord y);


		/** Get the width of the area with a precision of 64th of a pixel 
		@return Width of the area */
		Dim width_() const;

		/** Get the height of the area with a precision of 64th of a pixel 
		@return Height of the area */
		Dim height_() const;

		/** Get the x coordinate of the area with a precision of 64th of a pixel 
		@return X coordinate of the area */
		Coord x_() const;

		/** Get the y coordinate of the area with a precision of 64th of a pixel 
		@return Y coordinate of the area */
		Coord y_() const;

		/** Get the width of the area in pixels
		@return Width of the area */
		Dim width() const;

		/** Get the height of the area in pixels
		@return Height of the area */
		Dim height() const;

		/** Get the x coordinate of the area in pixels
		@return X coordinate of the area */
		Coord x() const;

		/** Get the y coordinate of the area in pixels
		@return Y coordinate of the area */
		Coord y() const;

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
		const Point& position() const;

		/** Return size of area
		@return Size of the area */
		const Size& size() const;

		/** Return position of area according to the shift
		@return Position of the area */
		Point& position();

		/** Return size of area
		@return Size of the area */
		Size& size();

		/** Print content */
		void print(const char * name) const;

	protected:
/// @cond DOXYGEN_IGNORE
		Point m_position;
		Size m_size;
/// @endcond 
	};
}
#endif