#pragma once
namespace berialdraw
{
	/** The LineStyle class customizes the visual properties of drawn shapes, such as lines, curves, or polygons. */
	class LineStyle : public Style
	{
	public:
		/** Constructor */
		LineStyle();
	
		/** Destructor */	
		~LineStyle(){}

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Copy operator */
		LineStyle& operator=(const LineStyle& other);

		/** Set properties with another */
		void set(const LineStyle & other);

		/** Create new shape style */
		static Style * create();


		/** Get the point1 */
		const Point & point1() const { return m_point1; }
		
		/** Set the point1 */
		void point1(const Point & s);

		/** Set the point1 with x and y in pixels */
		void point1(Coord x, Coord y);

		/** Set the point1 with a precision of 64th of a pixel */
		void point1_(Coord x, Coord y);



		/** Get the point2 */
		const Point & point2() const { return m_point2; }
		
		/** Set the point2 */
		void point2(const Point & s);

		/** Set the point2 with x and y in pixels */
		void point2(Coord x, Coord y);

		/** Set the point2 with a precision of 64th of a pixel */
		void point2_(Coord x, Coord y);


	protected:
/// @cond DOXYGEN_IGNORE
		Point m_point1;
		Point m_point2;
/// @endcond 
	};
}
