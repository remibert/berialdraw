#pragma once
namespace berialdraw
{
	/** The Picture class displays images (raster or vector) with support for
	transparency, bicubic resizing, and multiple fit modes.
	It uses Image for raster files (PNG/JPEG) and Sketch for vector files (ICN/SVG). */
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
		static void test11();
		static void test12();
		static void test13();
#endif
	protected:
	/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		Picture& operator=(const Picture& other) = delete;

		/** Check if filename has a vector extension (icn, svg) */
		static bool is_vector_extension(const String & filename);

		/** Load the image or sketch from the current filename */
		void load_picture();

		/** Compute the fit size based on image dimensions, constraints and context */
		Size compute_fit_size(uint32_t img_w, uint32_t img_h, const Area & area);

		/** Apply max_size constraints to the computed size */
		void apply_max_size_constraints(Size & size);

		/** Reset scroll position and trigger re-layout */
		void reset_scroll_position();

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region) override;

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout) override;

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position) override;

		Image * m_image = nullptr;
		Sketch * m_sketch = nullptr;
		bool m_is_vector = false;

		// Computed content size after place() analysis
		Size          m_fit_content_size;
		bool          m_picture_placed = false;
	/// @endcond
	};
}
