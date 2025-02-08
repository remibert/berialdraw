#ifndef ShapeStyle_hpp_INCLUDED
#define ShapeStyle_hpp_INCLUDED

namespace berialdraw
{
	/** The ShapeStyle class customizes the visual properties of drawn shapes, such as lines, curves, or polygons. */
	class ShapeStyle : public Style
	{
	public:
		/** Constructor */
		ShapeStyle();
	
		/** Destructor */	
		~ShapeStyle(){}

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Set properties with another */
		void set(const ShapeStyle & other);

		/** Create new shape style */
		static Style * create();


		/** Get the thickness */
		Dim thickness() const;

		/** Get the thickness with a precision of 64th of a pixel */
		Dim thickness_() const;
	
		/** Set the thickness in pixels */
		void thickness(Dim v);

		/** Set the thickness with a precision of 64th of a pixel */
		void thickness_(Dim v);


		/** Get the point1 */
		const Point & point1() const;
		
		/** Set the point1 */
		void point1(const Point & s);

		/** Set the point1 with x and y in pixels */
		void point1(Coord x, Coord y=0);

		/** Set the point1 with a precision of 64th of a pixel */
		void point1_(Coord x, Coord y=0);



		/** Get the point2 */
		const Point & point2() const;
		
		/** Set the point2 */
		void point2(const Point & s);

		/** Set the point2 with x and y in pixels */
		void point2(Coord x, Coord y=0);

		/** Set the point2 with a precision of 64th of a pixel */
		void point2_(Coord x, Coord y=0);


		/** Get the radius */
		Dim radius() const;

		/** Get the radius with a precision of 64th of a pixel */
		Dim radius_() const;
	
		/** Set the radius in pixels */
		void radius(Dim v);

		/** Set the radius with a precision of 64th of a pixel */
		void radius_(Dim v);

	protected:
/// @cond DOXYGEN_IGNORE
		Dim m_thickness = 0;
		Dim m_radius = 0;

		Point m_point1;
		Point m_point2;
/// @endcond 
	};
}
#endif
