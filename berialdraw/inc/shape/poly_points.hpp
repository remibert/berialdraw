#ifndef PolyPoints_hpp_INCLUDED
#define PolyPoints_hpp_INCLUDED

namespace berialdraw
{
	/** Class representing a point in a polyline, inheriting from Point.
	Each PolyPoint can link to the next point in the polyline. */
	class PolyPoint : public Point
	{
	public:
		/** Constructor
		@param x X-coordinate of the point, default is 0
		@param y Y-coordinate of the point, default is 0
		@param pixel Boolean indicating whether coordinates are in pixel units, default is true  */
		PolyPoint(Coord x = 0, Coord y = 0, bool pixel = true) :
			Point(x, y, pixel)
		{
			next = nullptr;
		}

		/** Constructor from a Point object
		@param p Point object to initialize from */
		PolyPoint(const Point& p) : Point(p)
		{
			next = nullptr;
		}

		/** Destructor */
		virtual ~PolyPoint() {}

		/** Pointer to the next point in the polyline */
		PolyPoint* next;
	};

	/** Class to manage a collection of PolyPoint objects forming a polyline. */
	class PolyPoints
	{
	public:
		/** Constructor */
		PolyPoints();

		/** Copy constructor
		@param other PolyPoints object to copy from */
		PolyPoints(const PolyPoints & other);

		/** Destructor */
		~PolyPoints();

		/** Append a point to the end of the polyline.
		@param x X-coordinate of the point
		@param y Y-coordinate of the point */
		void append(Coord x, Coord y);

		/** Append a point with a precision of 64th of a pixel to the end of the polyline.
		@param x X-coordinate of the point shifted by 6 bits
		@param y Y-coordinate of the point shifted by 6 bits */
		void append_(Coord x, Coord y);

		/** Append a Point object to the end of the polyline.
		@param p Point object to append */
		void append(const Point & p);

		/** Prepend a point to the start of the polyline.
		@param x X-coordinate of the point
		@param y Y-coordinate of the point */
		void prepend(Coord x, Coord y);

		/** Prepend a point  with a precision of 64th of a pixel to the start of the polyline.
		@param x X-coordinate of the point shifted by 6 bits
		@param y Y-coordinate of the point shifted by 6 bits */
		void prepend_(Coord x, Coord y);

		/** Prepend a Point object to the start of the polyline.
		@param p Point object to prepend */
		void prepend(const Point & p);

		/** Clear the polyline, removing all points */
		void clear();

		/** Get the first point in the polyline.
		@param p Reference to a Point object to store the first point
		@return True if a point was found, false otherwise */
		bool get_first(Point & p);

		/** Get the next point in the polyline after a call to get_first().
		@param p Reference to a Point object to store the next point
		@return True if a point was found, false otherwise */
		bool get_next(Point & p);

		/** Get the number of points in the polyline.
		@return Number of points in the polyline */
		uint32_t count();

#ifdef _DEBUG
		static void test();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		PolyPoint* m_first_point;   /**< Pointer to the first point in the polyline */
		PolyPoint* m_last_point;    /**< Pointer to the last point in the polyline */
		PolyPoint* m_current;       /**< Pointer to the last point in the polyline */
		uint32_t m_count;           /**< Number of points in the polyline */
/// @endcond
	};
}
#endif
