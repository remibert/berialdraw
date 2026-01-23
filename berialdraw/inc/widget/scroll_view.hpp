#pragma once
namespace berialdraw
{
	/** The ScrollView class provides a scrollable container for content that exceeds the visible area. 
	It supports both vertical and horizontal scrolling. */
	class ScrollView: public Widget, public ScrollViewStyle
	{
	/** @image html sample_scroll_1.svg "example" width=200px height=200px
	@example sample_scrollview.cpp*/
	public:
		/** Create widget */
		ScrollView(Widget * parent);

		/** Destroy widget */
		virtual ~ScrollView();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Get scroll size */
		const Size & scroll_size() const;

		/** Set scroll size */
		void scroll_size(const Size & size);

		/** Set the scroll size with width and height in pixels */
		void scroll_size(Dim w, Dim h);

		/** Get scroll position */
		const Point & scroll_position() const;

		/** Set scroll position.
		@param position : A negative move on the X axis moves the content to the right.
			A positive move on the X axis moves the content to the left.
			A negative move on the Y axis moves the content down.
			A positive move on the Y axis moves the content up. */
		void scroll_position(const Point & position);

		/** Set the scroll position with x and y in pixels */
		void scroll_position(Coord x, Coord y);

		/** Get viewport size */
		const Size & viewport_size() const;

		/** Set viewport size */
		void viewport_size(const Size & size);

		/** Set the viewport size with width and height in pixels */
		void viewport_size(Dim w, Dim h);

		/** Return the size of content without marges */
		virtual Size content_size();

		/** Return the size of content with its marges */
		virtual Size marged_size();

		/** Move the scroll view to see the widget with the focus */
		void scroll_focus(Widget * widget);

		/** Compute the scroll area */
		void scroll_area(Area & area);

#ifdef _DEBUG
	public:
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
		static void test6();
		static void test7();
		static void test8();
		static void test_create_window(Window & window);
#endif
	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		ScrollView& operator=(const ScrollView& other) = delete;
		
		/** Call back on scroll */
		void on_scroll(Widget * widget, const ScrollEvent & evt);

		/** Compute the scroll area */
		virtual void space_occupied(Point & min_position, Point & max_position);

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region);

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout);
		Point compute_scroll_view(const Area & area, Point & scroll_position, Size & scroll_size);
		Coord compute_scroll(Coord & scroll_position, Coord & m_scroll_position, Dim scroll_size, Dim area, Align align);
		Coord calc_shift_focus(Coord widget_position,   Dim widget_size, Coord viewport_position, Dim viewport_size);
/// @endcond
	};
}
