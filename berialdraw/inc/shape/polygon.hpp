#ifndef Polygon_hpp_INCLUDED
#define Polygon_hpp_INCLUDED

struct FT_Outline_;

namespace berialdraw
{
	class Point;

	/** Class to create bezier shapes using the FreeType render engine */
	class Polygon : public Shape
	{
	public:
		/**	Constructor
		@param canvas Pointer to the Canvas object, default is nullptr */
		Polygon(Canvas * canvas = 0);

		/** Copy constructor
		@param other Polygon object to copy from */
		Polygon(const Polygon & other);

		/** Destructor */
		virtual ~Polygon();

		/** Add a point to the polygon
		@param p Point object to add */
		void add_point(const Point & p);

		/** Add a point to the polygon
		@param x X-coordinate of the point
		@param y Y-coordinate of the point */
		void add_point(Coord x, Coord y);

		/** Add a point to the polygon with a precision of 64th of a pixel 
		@param x X-coordinate
		@param y Y-coordinate */
		void add_point_(Coord x, Coord y);

		/** Add a cubic bezier curve to the polygon
		@param p Point object representing the curve control point */
		void add_cubic(const Point & p);

		/** Add a cubic bezier curve to the polygon
		@param x X-coordinate
		@param y Y-coordinate */
		void add_cubic(Coord x, Coord y);

		/** Add a cubic bezier curve to the polygon with a precision of 64th of a pixel 
		@param x X-coordinate
		@param y Y-coordinate */
		void add_cubic_(Coord x, Coord y);

		/** Add a conic bezier curve to the polygon
		@param p Point object representing the curve control point */
		void add_conic(const Point & p);

		/** Add a conic bezier curve to the polygon
		@param x X-coordinate of the curve control point
		@param y Y-coordinate of the curve control point */
		void add_conic(Coord x, Coord y);

		/** Add a conic bezier curve to the polygon with a precision of 64th of a pixel 
		@param x X-coordinate
		@param y Y-coordinate*/
		void add_conic_(Coord x, Coord y);

		/** Resize the FT_Outline buffer
		@param nb_points Number of points to resize the buffer to
		@param nb_contours Number of contours to resize the buffer to */
		void resize(uint32_t nb_points, uint32_t nb_contours);

		/** Select the next contour
		(This is required after each add operation, otherwise nothing will be rendered) */
		void next_contour();

		/** Clear the polygon, removing all points */
		void clear();

		/** Get the FreeType outline object
		@return Reference to the associated Outline object */
		Outline & outline() const
		{
			return *m_outline;
		}

		/** Set the zoom ratio for the polygon
		@param z zoom value */
		void zoom(int z);
		
		/** Get the zoom ratio for the polygon
		@return zoom zoom value */
		uint32_t zoom();

		/** Set the zoom ratio for the polygon with a precision of 64th of a pixel 
		@param z zoom value */
		void zoom_(int z);
		
		/** Get the zoom ratio for the polygon with a precision of 64th of a pixel 
		@return zoom zoom value */
		uint32_t zoom_();


		/** Render the polygon outline
		@param shift Point object representing the shift to apply */
		virtual void paint(const Point & shift);

		/** Get the polygon object
		@return Reference to the current Polygon object */
		virtual const Polygon & polygon() const
		{
			return *this;
		}

#ifdef _DEBUG
		static void test();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		Outline * m_outline; /**< Pointer to the Outline object associated with the polygon */
/// @endcond
	};
}

#endif // Polygon_hpp_INCLUDED
