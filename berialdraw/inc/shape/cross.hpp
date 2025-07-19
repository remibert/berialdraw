#pragma once
namespace berialdraw
{
	/** Class drawing a cross on a canvas */
	class Cross : public Marker
	{
	public:
		/** Constructor */
		Cross(Canvas * canvas);

		/** Render outline */
		virtual void paint(const Point & shift);

#ifdef _DEBUG
		static void test();
#endif
	};
}
