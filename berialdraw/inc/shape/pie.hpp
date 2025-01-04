#ifndef Pie_hpp_INCLUDED
#define Pie_hpp_INCLUDED

namespace berialdraw
{
	/** Class representing a Pie shape */
	class Pie : public Shape, public PieStyle
	{
	public:
		/** Constructor for Pie
		@param canvas Pointer to the canvas on which the pie is drawn */
		Pie(Canvas * canvas = 0);

		/** Copy constructor for Pie
		@param other Other Pie object to copy from */
		Pie(const Pie & other);

		/** Paint the Pie on the desktop
		@param shift Point object representing the shift in position */
		virtual void paint(const Point & shift);

		/** Get the polygon representing the Pie
		@return Reference to the polygon of the Pie */
		virtual const Polygon & polygon() const
		{
			return m_polygon;
		}

	#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void dial(const Point & origin, Canvas * canvas);
	#endif

		/** Get the slice points of the Pie
		@param center Center point of the Pie
		@param radius Radius of the Pie
		@param start_angle Starting angle of the slice
		@param end_angle Sweep angle of the slice
		@param p1 First point of the slice
		@param c1 First control point of the slice
		@param c2 Second control point of the slice
		@param p2 Second point of the slice */
		static void get_slice(const Point & center, Dim radius, Coord start_angle, Coord end_angle, Point & p1, Point & c1, Point & c2, Point & p2);

	protected:
/// @cond DOXYGEN_IGNORE
		/** Build the filled Pie shape
		@param radius Radius of the Pie */
		void build_filled(Dim radius);

		/** Build the empty Pie shape */
		void build_empty();

		Polygon m_polygon; /**< Polygon representing the Pie */
/// @endcond
	};
	}

	#endif
