#pragma once
namespace berialdraw
{
	/** Class representing a generic marker, which is used to place marks on curves and lines.  */
	class Marker : public Shape, public RoundStyle
	{
	public:
		/** Create a marker
		@param canvas canvas used to draw marker */
		Marker(Canvas * canvas);

		/** Copy contructor */
		Marker(const Marker & other);

		/** Refresh the marker */
		void paint(const Point & shift, const char * filled, const char * un_filled);

		/** Refresh the marker */
		virtual void paint(const Point & shift){}

		/** Get the bezier polygon */
		virtual const Polygon & polygon() const
		{
			return m_polygon;
		}

#ifdef _DEBUG
		static void test();
#endif

	protected:
#ifndef SWIG
/// @cond DOXYGEN_IGNORE
		Polygon m_polygon;
/// @endcond 
#endif
	};
}
