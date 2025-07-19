#pragma once
namespace berialdraw
{
	class Widget;

	/** Rendering motor, use freetype to show all vector shapes and widgets */
	class Renderer
	{
	public:
		/** Constructor
		@param width The width of the renderer in points
		@param height The height of the renderer in points
		@param scale display scale, a value less than or equal to 15 will be multiplied by 64, otherwise the value will be taken into account, scale 0 will be set to 1
		@param pool Size of the memory pool (default is 8192) */
		Renderer(Dim width, Dim height, Dim scale=1, uint32_t pool=8192);

		/** Destructor */
		~Renderer();

		/** Draw a shape with a shift
		@param shape Reference to the Shape object
		@param shift Shift point for drawing */
		void draw(const Shape & shape, const Point & shift);

		/** Paint spans on the framebuffer
		@param y Y-coordinate
		@param count Number of spans
		@param spans Pointer to the FT_Span array */
		void paint(int y, int count, const FT_Span* spans);

		/** Clear the framebuffer */
		void clear();

		/** Draw font bitmap buffer
		@param x_ X-coordinate with a precision of 64th of a pixel .
		@param y_ Y-coordinate with a precision of 64th of a pixel .
		@param buffer Pointer to the buffer
		@param width Width of the buffer
		@param height Height of the buffer
		@param color Color for drawing */
		void draw(Coord x_, Coord y_, const uint8_t * buffer, Dim width, Dim height, uint32_t color);

		/** Draw freetype outline object
		@param position Position for drawing
		@param margin Margin for drawing
		@param shift Shift point for drawing
		@param center Center point for drawing
		@param color Color for drawing
		@param angle_ Angle for drawing
		@param outline Reference to the Outline object */
		void draw(const Point & position, const Margin & margin, const Point & shift, const Point & center, uint32_t color, Coord angle_, Outline & outline);

		/** Fill a rectangle
		@param x X-coordinate
		@param y Y-coordinate
		@param width Width of the rectangle
		@param height Height of the rectangle
		@param color Color for filling */
		void fillrect(Coord x, Coord y, Dim width, Dim height, uint32_t color);

		/** Draw a line
		@param x X-coordinate
		@param y Y-coordinate
		@param length Length of the line
		@param coverage Coverage of the line
		@param alpha Alpha value for drawing
		@param color Color for drawing */
		void draw_line(Coord x, Coord y, uint32_t length, uint32_t coverage, uint32_t alpha, uint32_t color);

		/** Get the current color
		@return Current color */
		uint32_t color();

		/** Set the drawing region
		@param region Reference to the Region object */
		void region(Region & region);

		/** Get the drawing region
		@return Reference to the Region object */
		Region & region();

		/** Get the scale factor for screen resolution. A value of 64 corresponds to a scale factor of 1 (1 << 6).
		The scale is defined in the class constructor.
		@return The current scale factor (<< 6). */
		Dim scale_();

		/** Get the scale factor for screen resolution. The scale is defined in the class constructor.
		@return The current scale factor. */
		Dim scale();

		/** Return the renderer size 
		@return size */
		Size size() const;

	protected:
/// @cond DOXYGEN_IGNORE
		uint32_t         m_color = 0;     /**< Current color */
		uint8_t*         m_pool = 0;      /**< Pointer to the memory pool */
		uint32_t         m_pool_size = 0; /**< Size of the memory pool */
		uint32_t         m_x_min = 0;     /**< Minimum X-coordinate */
		uint32_t         m_y_min = 0;     /**< Minimum Y-coordinate */
		uint32_t         m_x_max = 0;     /**< Maximum X-coordinate */
		uint32_t         m_y_max = 0;     /**< Maximum Y-coordinate */
		FT_Raster        m_raster;        /**< FreeType raster object */
		FT_Raster_Params m_params;        /**< FreeType raster parameters */
		Region *         m_region = 0;    /**< Pointer to the Region object */
		Dim              m_scale = 1*64;    /**< Scale factor for screen resolution. A value of 64 corresponds to a scale factor of 1 (1 << 6). */
		Size             m_size;          /**< Renderer size in points */
/// @endcond
	};
}
