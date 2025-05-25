#ifndef Rect_hpp_INCLUDED
#define Rect_hpp_INCLUDED

namespace berialdraw
{
/** This class represents a rectangle shape that can be drawn on a canvas.
It extends the Shape class and provides methods for rendering and 
defining the rectangle's outline. */
class Rect : public Shape 
{
public:
	/** Constructs a Rect object linked to a specific canvas.
	@param canvas Pointer to the canvas where the rectangle will be drawn. */
	Rect(Canvas * canvas);

#ifdef _DEBUG
	/** Unit test method to validate functionality. Each test corresponds to
	a different aspect or feature of the Rect class. */
	static void test();   ///< Test case 1
	static void test1();  ///< Test case 2
	static void test2();  ///< Test case 3
	static void test3();  ///< Test case 4
	static void test4();  ///< Test case 5
	static void test5();  ///< Test case 6
	static void test6();  ///< Test case 7
#endif

	/** Copy constructor for the Rect class.
	@param other The Rect object to copy from. */
	Rect(const Rect & other);

	/** Builds a polygon representation of a rectangle with customizable parameters.
	@param area Defines the rectangle's position and size.
	@param shift Offset to apply to the rectangle.
	@param radius Radius for rounded corners.
	@param thickness Thickness of the border.
	@param gap Gap between the border and the fill.
	@param sides Number of sides of the polygon (approximating a rounded rectangle).
	@param backcolor Background color of the rectangle.
	@param bordercolor Border color of the rectangle. */
	static void build_polygon(const Area & area, const Point & shift, Dim radius, Dim thickness, Dim gap, uint8_t sides, uint32_t backcolor, uint32_t bordercolor, Dim focus_thickness=0);

	/** Renders the outline of the rectangle on the canvas.
	@param shift Offset to apply while rendering. */
	virtual void paint(const Point & shift);

	/** Returns the polygon representation of the rectangle.
	@return A reference to the polygon object. */
	virtual const Polygon & polygon() const
	{
		return m_polygon;
	}

protected:
/// @cond DOXYGEN_IGNORE
	/** Creates a part of the rectangle's polygon representation. */
	void create_part();

	inline void add_corner    (Coord x, Coord y, Coord radius, Dim thickness, uint32_t flags);

	
	// Render outline
	void paint(const Point & shift, bool in_widget);


	/// Polygon representation of the rectangle's outline.
	Polygon m_polygon;
/// @endcond
};

}
#endif // Rect_hpp_INCLUDED

