#pragma once
namespace berialdraw
{
	/** Class of tools for simulating a compass, which will be used to position shapes at given points */
	class Compass : public RoundStyle, public CommonStyle
	{
	public:
		/** Constructor */
		Compass();

		/** Position given by the compass as a function of the angle and the center */
		Point position();

#ifdef _DEBUG
		static void test();
#endif
	};
}
