#pragma once
namespace berialdraw
{
	/** The Canvas class provides a flexible area for custom graphics and rendering. 
	It is ideal for applications requiring charts. */
	class Canvas: public Widget
	{
	/** @image html sample_canvas_1.svg "example" width=200px height=200px
	@example sample_canvas.cpp*/
	public:
		/** Create widget */
		Canvas(Widget * parent);

		/** Destroy widget */
		virtual ~Canvas();

		/** Add shape into the canvas (all shapes added will be destroyed when the canvas destroy) */
		void add(Shape *, size_t shape_size);

		/** Remove shape from the canvas  */
		void remove(Shape *);

		/** Clear all shape on canvas */
		void clear();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Copy all styles of the canvas */
		void copy(const Canvas & canvas);

		/** Copy all styles of the canvas */
		void copy(const Canvas * canvas);

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		Canvas& operator=(const Canvas& other) = delete;
		
		/** Return the size of content without marges */
		virtual Size content_size();

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region);
	
		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout);

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

		Vector<Shape *> m_shapes;
		Size  m_content_size;
/// @endcond 
	};
}
