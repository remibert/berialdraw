#pragma once
namespace berialdraw
{
	/** The Picture class displays a raster image (PNG or JPEG) with support for
	transparency, bicubic resizing, and multiple fit modes. 
	It supports reading images from files including ZIP archives via the File abstraction. */
	class Picture: public Widget, public BorderStyle, public PictureStyle
	{
	public:
		/** Create widget */
		Picture(Widget * parent);

		/** Destroy widget */
		virtual ~Picture();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it) override;

		/** Get the style cascade mode for this widget */
		virtual StyleCascadeMode style_cascade_mode() const override;

		/** Return the size of content without marges */
		virtual Size content_size() override;

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
		static void test6();
		static void test7();
		static void test8();
		static void test9();
		static void test10();
#endif
	protected:
	/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		Picture& operator=(const Picture& other) = delete;

		/** Load and decode the image file */
		void load_image();

		/** Rebuild the resized cache if needed */
		void rebuild_cache(uint32_t area_width, uint32_t area_height);

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region) override;

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout) override;

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position) override;

		const ImageCacheEntry * m_cache_entry = nullptr;
		uint32_t*     m_cached_pixels = nullptr;
		uint32_t      m_cached_width = 0;
		uint32_t      m_cached_height = 0;

		// Computed content size after place() analysis
		Size          m_fit_content_size;
		bool          m_picture_placed = false;
	/// @endcond
	};
}
