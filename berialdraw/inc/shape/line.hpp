#ifndef Line_hpp_INCLUDED
#define Line_hpp_INCLUDED

namespace berialdraw
{
	/** Class drawing a line on a canvas */
	class Line: public Shape
	{
	public:
		/** Constructor */
		Line(Canvas * canvas=0);

		/** Refresh the line */
		virtual void paint(const Point & shift);

		/** Get the bezier polygon */
		virtual const Polygon & polygon() const
		{
			return m_polygon;
		}

		/** Get all points of the line tickness */
		static void get_thickness(Coord x1, Coord y1, Coord x2, Coord y2, Dim thickness, Coord & dx, Coord & dy);

#ifdef _DEBUG
		static void test();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		/** Compute the line tickness */
		void compute_thickness(Coord & dx, Coord & dy);
		Polygon m_polygon;
/// @endcond
	};
}
#endif