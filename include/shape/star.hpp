#pragma once

namespace berialdraw
{
	/** Class representing a star marker with 5 branches.
	@ingroup shape */
	class Star : public Marker
	{
	public:
		/** Initializes the class with a canvas.
		@param canvas pointer to parent canvas */
		Star(Canvas * canvas);

		/** Paints the shape to a given location.
		@param shift relative position of the top-left corner of the widget  */
		virtual void paint(const Point & shift);
	};
}
