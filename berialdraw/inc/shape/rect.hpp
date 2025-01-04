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
	static void build_polygon(const Area & area, const Point & shift, Dim radius, Dim thickness, Dim gap, uint8_t sides, uint32_t backcolor, uint32_t bordercolor);

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
	/** Generates the top-right corner of the rectangle with the specified radius.
	@param x X-coordinate of the corner.
	@param y Y-coordinate of the corner.
	@param r Radius of the corner.
	@param clockwise Specifies the drawing direction (clockwise or counter-clockwise). */
	void top_right_corner(Coord x, Coord y, Coord r, bool clockwise);

	/**
	 * Generates the top-left corner of the rectangle with the specified radius.
	 * @param x X-coordinate of the corner.
	 * @param y Y-coordinate of the corner.
	 * @param r Radius of the corner.
	 * @param clockwise Specifies the drawing direction (clockwise or counter-clockwise).
	 */
	void top_left_corner(Coord x, Coord y, Coord r, bool clockwise);

	/** Generates the bottom-left corner of the rectangle with the specified radius.
	@param x X-coordinate of the corner.
	@param y Y-coordinate of the corner.
	@param r Radius of the corner.
	@param clockwise Specifies the drawing direction (clockwise or counter-clockwise). */
	void bottom_left_corner(Coord x, Coord y, Coord r, bool clockwise);

	/**
	 * Generates the bottom-right corner of the rectangle with the specified radius.
	 * @param x X-coordinate of the corner.
	 * @param y Y-coordinate of the corner.
	 * @param r Radius of the corner.
	 * @param clockwise Specifies the drawing direction (clockwise or counter-clockwise).
	 */
	void bottom_right_corner(Coord x, Coord y, Coord r, bool clockwise);

	/** Draws a rectangle on the canvas with specified dimensions.
	@param x X-coordinate of the rectangle's top-left corner.
	@param y Y-coordinate of the rectangle's top-left corner.
	@param width Width of the rectangle.
	@param height Height of the rectangle.
	@param clockwise Specifies the drawing direction (clockwise or counter-clockwise).*/
	void draw_rect(Coord x, Coord y, Coord width, Coord height, bool clockwise);

	/**
	 * Creates a part of the rectangle's polygon representation.
	 */
	void create_part();

	/** Creates a polygon object for the rectangle with specified parameters.
	@param polygon Reference to the polygon object to be created.
	@param position Position of the rectangle.
	@param size Size of the rectangle.
	@param thickness Thickness of the border.
	@param radius Radius for rounded corners.
	@param angle Rotation angle of the rectangle. */
	static void create_polygon(Polygon & polygon, const Point & position, const Size & size, Dim thickness, Dim radius, Coord angle);

	/// Polygon representation of the rectangle's outline.
	Polygon m_polygon;
/// @endcond
};

}
#endif // Rect_hpp_INCLUDED

