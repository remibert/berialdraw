#pragma once
namespace berialdraw
{
	/** Class representing a square marker.
	Inherits from Marker, which is used to place marks on curves and lines.  */
	class Square : public Marker
	{
	public:
		/** Constructor for Square.
		@param canvas Pointer to the canvas object. */
		Square(Canvas * canvas);

#ifdef _DEBUG
		/** Unitary test for the Square class. */
		static void test();
#endif

		/** Renders the outline of the square.
		@param shift Point object representing the shift. */
		virtual void paint(const Point & shift);
	};
}
