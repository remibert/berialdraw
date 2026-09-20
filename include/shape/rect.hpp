#pragma once
namespace berialdraw
{
	class ClipMask;
	class RectRenderer;

/** This class represents a rectangle shape that can be drawn on a canvas.
It extends the Shape class and provides methods for rendering and 
defining the rectangle's outline. */
class Rect : public Shape, public RoundStyle
{
	friend class RectRenderer;

public:
	/** Constructs a Rect object linked to a specific canvas.
	@param canvas Pointer to the canvas where the rectangle will be drawn. */
	Rect(Canvas * canvas);

#ifdef _DEBUG
	/** Unit test method to validate functionality. Each test corresponds to
	a different aspect or feature of the Rect class. */
	static void test();   ///< All tests case 
	static void test1();
	static void test2();
	static void test3();
	static void test4();
	static void test5();
	static void test6();
	static void test7();
	static void test8();
	static void test9();
#endif

	/** Copy constructor for the Rect class.
	@param other The Rect object to copy from. */
	Rect(const Rect & other);

	/** Renders the outline of the rectangle on the canvas.
	@param shift Offset to apply while rendering. */
	virtual void paint(const Point & shift) override;

	/** Returns the polygon representation of the rectangle.
	@return A reference to the polygon object. */
	virtual const Polygon & polygon() const override
	{
		return m_polygon;
	}

	/** Get the marged size of the shape
	@return Size of the shape with margin */
	virtual Size marged_size() override;

private:
/// @cond DOXYGEN_IGNORE
	// Render outline
	void paint(const Point & shift, bool in_widget);

	/// Polygon representation of the rectangle's outline.
	Polygon m_polygon;
/// @endcond
};

}
