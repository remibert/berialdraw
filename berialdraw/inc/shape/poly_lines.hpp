#pragma once
namespace berialdraw
{
	/** Class to represent and manipulate a series of connected lines */
	class PolyLines : public Shape, public RoundStyle
	{
	public:
		/** Constructor
		@param canvas Pointer to the Canvas object, default is nullptr */
		PolyLines(Canvas * canvas);

		/** Copy constructor
		@param other PolyLines object to copy from */
		PolyLines(const PolyLines & other);

		/** Prepend a point to the polyline
		@param x X-coordinate of the point
		@param y Y-coordinate of the point */
		void prepend(Coord x, Coord y);

		/** Prepend a point to the polyline with a precision of 64th of a pixel 
		@param x X-coordinate
		@param y Y-coordinate */
		void prepend_(Coord x, Coord y);

		/** Prepend a point to the polyline
		@param p Point object to prepend */
		void prepend(const Point & p);

		/** Append a point to the polyline
		@param x X-coordinate of the point
		@param y Y-coordinate of the point */
		void append(Coord x, Coord y);

		/** Append a point to the polyline with a precision of 64th of a pixel 
		@param x X-coordinate 
		@param y Y-coordinate */
		void append_(Coord x, Coord y);

		/** Append a point to the polyline
		@param p Point object to append */
		void append(const Point & p);

		/** Render the polyline outline
		@param shift Point object representing the shift to apply */
		virtual void paint(const Point & shift);

		/** Get the polygon object associated with the polyline
		@return Reference to the associated Polygon object */
		virtual const Polygon & polygon() const
		{
			return m_polygon;
		}

		/** Get the FreeType outline object associated with the polygon
		@return Reference to the Outline object */
		Outline & outline()
		{
			return m_polygon.outline();
		}

		/** Clear the polyline, removing all points */
		void clear();

		/** Build the polyline structure from the added points */
		void build();

#ifdef _DEBUG
		static void test();
		static void test0();
		static void test1();
		static void test2();
		static void test3();
#endif

	protected:
#ifndef SWIG
/// @cond DOXYGEN_IGNORE
		/** Add a point to the right side of the polyline
		@param p Point object to add */
		void add_right(const Point & p);

		/** Add a point to the left side of the polyline
		@param p Point object to add */
		void add_left(const Point & p);

		/** Calculate the side points for a segment
		@param p1 First point of the segment
		@param p2 Second point of the segment
		@param thickness Thickness of the polyline
		@param right Boolean indicating whether to calculate the right side
		@param linear Linear object to store the result */
		void get_side(const Point& p1, const Point& p2, Dim thickness, bool right, Linear& linear);

		/** Calculate the end points for a segment
		@param p1 First point of the segment
		@param p2 Second point of the segment
		@param right Point to store the right end
		@param left Point to store the left end */
		void get_end(const Point& p1, const Point& p2, Point & right, Point & left);

		/** Calculate the start points for a segment
		@param p1 First point of the segment
		@param p2 Second point of the segment
		@param right Point to store the right start
		@param left Point to store the left start */
		void get_start(const Point& p1, const Point& p2, Point & right, Point & left);

		/** Add the end points of a segment to the polyline
		@param p1 First point of the segment
		@param p2 Second point of the segment */
		void add_end(const Point& p1, const Point& p2);

		/** Add the start points of a segment to the polyline
		@param p1 First point of the segment
		@param p2 Second point of the segment */
		void add_start(const Point& p1, const Point& p2);

		/** Add a junction between segments to the polyline
		@param p1 First point of the junction
		@param p2 Second point of the junction
		@param p3 Third point of the junction */
		void add_junction(const Point& p1, const Point& p2, const Point& p3);

		Polygon    m_polygon;  /**< Polygon object representing the polyline */
		PolyPoints m_points;   /**< Points that make up the polyline */
		PolyPoints m_shape;    /**< Shape of the polyline */
		uint32_t     m_count = 0; /**< Number of points in the polyline */
/// @endcond
#endif
	};
}
