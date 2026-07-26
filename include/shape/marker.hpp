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
		virtual void paint(const Point & shift) override{}

		/** Get the marged size of the shape
		@return Size of the shape with margin */
		virtual Size marged_size() override;

		/** Get the bezier polygon */
		virtual const Polygon & polygon() const override
		{
			return m_polygon;
		}

#ifdef _DEBUG
		static void test();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		Polygon m_polygon;
/// @endcond 
	};
}
