#pragma once
namespace berialdraw
{
	/** Class representing a triangle marker.
	Inherits from Marker, which is used to place marks on curves and lines. */
	class Triangle : public Marker
	{
	public:
		/** Constructor for Triangle.
		@param canvas Pointer to the canvas object. */
		Triangle(Canvas * canvas);
	
	#ifdef _DEBUG
		/** Unitary test for the Triangle class. */
		static void test();
	#endif
		/** Renders the outline of the triangle.
		@param shift Point object representing the shift. */
		virtual void paint(const Point & shift);
	};
}
