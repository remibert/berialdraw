#ifndef Shape_hpp_included
#define Shape_hpp_included

namespace berialdraw
{
	class Renderer;
	class Polygon;
	class Outline;
	class Canvas;
	class Desktop;	

	/** Abstract class for shared by every shapes */
	class Shape : public ShapeStyle, public CommonStyle
	{
	public:
		/** Constructor
		@param canvas Pointer to the Canvas object */
		Shape(Canvas * canvas);

		/** Copy constructor
		@param other Reference to another Shape object */
		Shape(const Shape & other);

		/** Destructor */
		virtual ~Shape();

		/** Paints the shape on the desktop with a shift
		@param shift Shift point for painting */
		virtual void paints(const Point & shift);

		/** Pure virtual function to paint the shape on the desktop with a shift
		@param shift Shift point for painting */
		virtual void paint(const Point & shift) = 0;

		/** Get the content size of the shape
		@return Size of the content */
		virtual Size content_size();

		/** Get the polygon of the shape
		@return Reference to the Polygon object */
		virtual const Polygon & polygon() const = 0;

		/** Check if the shape is dirty
		@return True if the shape is dirty, otherwise false */
		bool is_dirty();

		/** Get the marged size of the shape
		@return Size of the shape with margin */
		virtual Size marged_size();

		/** Repetition type */
		enum Repetition
		{
			REPEAT_NONE,       /**< No repetition */
			REPEAT_ANGLE,      /**< Repeat by angle */
			REPEAT_POSITION_X, /**< Repeat by X position */
			REPEAT_POSITION_Y, /**< Repeat by Y position */
		};

		/** Repeats the display of the shape according to the selected information
		This makes it easy to display graduations for example
		@param type Repetition type
		@param start Start value for repetition
		@param end End value for repetition
		@param step Step value for repetition */
		void repeat(Repetition type, int start, int end, int step);

		/** Repeats the display of the shape according to the selected information with a precision of 64th.
		This makes it easy to display graduations for example
		@param type Repetition type
		@param start Start value for repetition
		@param end End value for repetition
		@param step Step value for repetition */
		void repeat_(Repetition type, int start, int end, int step);

	protected:
/// @cond DOXYGEN_IGNORE
		Repetition m_repetition = REPEAT_NONE; /**< Repetition type */
		Coord      m_start = 0;                /**< Start value for repetition */
		Coord      m_end = 0;                  /**< End value for repetition */
		Coord      m_step = 0;                 /**< Step value for repetition */
		Canvas *   m_canvas = 0;               /**< Pointer to the Canvas object */
/// @endcond
	};
}
#endif 
