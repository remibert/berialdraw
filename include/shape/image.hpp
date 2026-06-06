#pragma once
namespace berialdraw
{
	/** Shape that displays a raster image (PNG, JPEG) on a Canvas or within a Widget.
	Handles image loading via ImageCache, bicubic resizing, fit modes and alpha blending.
	This is the raster counterpart of the Sketch shape (which handles vector images). */
	class Image : public Shape
	{
	public:
		/** Constructor for image on a canvas
		@param canvas Pointer to the canvas on which the image is drawn */
		Image(Canvas * canvas);

		/** Constructor for image on a widget
		@param widget Pointer to the widget on which the image is drawn */
		Image(Widget * widget);

		/** Copy constructor
		@param other Other image object to copy from */
		Image(const Image & other);

		/** Destructor */
		virtual ~Image();

		/** Paint the image on the desktop
		@param shift Point object representing the shift in position */
		virtual void paint(const Point & shift);

		/** Get the polygon representing the image (empty polygon, no vector data)
		@return Reference to the polygon */
		virtual const Polygon & polygon() const
		{
			return m_polygon;
		}

		/** Paint the image within a clipping area
		@param foreclip Area defining the clipping region
		@param padding Margin for padding around the image
		@param alpha Transparency value (0=transparent, 255=opaque) */
		void paint(const Area & foreclip, const Margin & padding, uint8_t alpha);


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

		/** Check if the image is loaded */
		bool is_loaded() const;

		/** Load the image from filename */
		bool load();

		/** Get the content size of the image */
		Size content_size();

	#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
		static void test6();
	#endif

	protected:
/// @cond DOXYGEN_IGNORE
		/** Free rotated image cache */
		void clear_cache();

		/** Ensure the rotated cache exists */
		void ensure_cache();

		Polygon               m_polygon;
		String                m_filename;
		const ImageCacheEntry* m_cache_entry = nullptr;
		ImageItem*            m_rotated_cache = nullptr;
		ImageFitMode          m_fit_mode = FIT;
		uint8_t               m_alpha = 255;
		bool                  m_image_modified = true;
		void *                m_parent = 0;
/// @endcond
	};
}
