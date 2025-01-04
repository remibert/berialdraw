#ifndef Circle_hpp_INCLUDED
#define Circle_hpp_INCLUDED

namespace berialdraw
{
	/** Draw circle on canvas */
	class Circle : public Marker
	{
	public:
		/** Constructor */
		Circle(Canvas * canvas = 0);

#ifdef _DEBUG
		static void test();
#endif
		/** Render outline */
		virtual void paint(const Point & shift);
	};
}
#endif