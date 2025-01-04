#ifndef Cross_hpp_INCLUDED
#define Cross_hpp_INCLUDED

namespace berialdraw
{
	/** Class drawing a cross on a canvas */
	class Cross : public Marker
	{
	public:
		/** Constructor */
		Cross(Canvas * canvas = 0);

		/** Render outline */
		virtual void paint(const Point & shift);

#ifdef _DEBUG
		static void test();
#endif
	};
}
#endif
