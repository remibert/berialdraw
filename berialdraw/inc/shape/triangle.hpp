#ifndef Triangle_hpp_INCLUDED
#define Triangle_hpp_INCLUDED

namespace berialdraw
{
	/** Class representing a triangle marker.
	Inherits from Marker, which is used to place marks on curves and lines. */
	class Triangle : public Marker
	{
	public:
		/** Constructor for Triangle.
		@param canvas Pointer to the canvas object. */
		Triangle(Canvas * canvas = 0);
	
	#ifdef _DEBUG
		/** Unitary test for the Triangle class. */
		static void test();
	#endif
		/** Renders the outline of the triangle.
		@param shift Point object representing the shift. */
		virtual void paint(const Point & shift);
	};
}
#endif
