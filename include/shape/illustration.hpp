#pragma once
namespace berialdraw
{
	/** Shape that can display either a raster image (PNG, JPEG) or a vector image (Sketch/SVG).
	Automatically detects the file type based on extension and delegates to the appropriate
	internal shape (Image or Sketch). Can be used on a Canvas or within a Widget. */
	class Illustration : public Shape
	{
	public:
		/** Constructor for illustration on a canvas
		@param canvas Pointer to the canvas on which the illustration is drawn */
		Illustration(Canvas * canvas);

		/** Constructor for illustration on a widget
		@param widget Pointer to the widget on which the illustration is drawn */
		Illustration(Widget * widget);

		/** Destructor */
		virtual ~Illustration();

		/** Paint the illustration on the desktop
		@param shift Point object representing the shift in position */
		virtual void paint(const Point & shift);

		/** Get the polygon representing the illustration
		@return Reference to the polygon */
		virtual const Polygon & polygon() const
		{
			return m_polygon;
		}

		/** Paint the illustration within a clipping area
		@param foreclip Area defining the clipping region
		@param padding Margin for padding around the illustration
		@param alpha Transparency value (0=transparent, 255=opaque)
		@param stated_color Color for vector images (ignored for raster) */
		void paint(const Area & foreclip, const Margin & padding, uint8_t alpha, uint32_t stated_color = Color::TRANSPARENT);


		/** Get the filename value */
		const String & filename() const;

		/** Set filename value with string */
		void filename(const String & s);


		/** Get the fit mode */
		ImageFitMode fit_mode() const;

		/** Set the fit mode */
		void fit_mode(ImageFitMode mode);


		/** Get the alpha value */
		uint8_t alpha() const;

		/** Set the alpha value (0=transparent, 255=opaque) */
		void alpha(uint8_t a);


		/** Get the image width in pixels (0 if not loaded) */
		uint32_t image_width() const;

		/** Get the image height in pixels (0 if not loaded) */
		uint32_t image_height() const;

		/** Check if the illustration is loaded */
		bool is_loaded() const;

		/** Check if the illustration is a vector image */
		bool is_vector() const;

		/** Load the illustration from filename */
		bool load();

		/** Get the content size of the illustration */
		virtual Size content_size();

	protected:
/// @cond DOXYGEN_IGNORE
		/** Detect the type based on file extension and create the appropriate shape */
		void detect_type();

		/** Check if filename has a vector extension (.icn, .svg) */
		static bool is_vector_extension(const String & filename);

		Polygon       m_polygon;
		String        m_filename;
		Image *       m_image = nullptr;
		Sketch *      m_sketch = nullptr;
		ImageFitMode  m_fit_mode = FIT;
		uint8_t       m_alpha = 255;
		bool          m_is_vector = false;
		bool          m_modified = true;
		void *        m_parent = 0;
/// @endcond
	};
}
