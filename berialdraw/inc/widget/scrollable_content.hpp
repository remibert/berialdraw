#pragma once
namespace berialdraw
{
	/** Base class for scrollable content (ScrollView and TableView).
	Provides common scrolling behavior and rendering for widgets that need to scroll their content. */
	class ScrollableContent : public Widget, public ScrollViewStyle
	{
	public:
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

		/** Move the scroll view to see the widget with the focus */
		void scroll_focus(Widget * widget);

		/** Compute the scroll area */
		void scroll_area(Area & area);

	protected:
		/** Constructor for subclasses */
		ScrollableContent(const char * classname, Widget * parent, size_t size_of_widget);

		/** Destructor */
		virtual ~ScrollableContent();

		/** Remove operator = */
		ScrollableContent& operator=(const ScrollableContent& other) = delete;
		
		/** Call back on scroll */
		void on_scroll(Widget * widget, const ScrollEvent & evt);

		/** Compute the scroll area */
		virtual void space_occupied(Point & min_position, Point & max_position);

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region);

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout);

		/** Helper methods for scroll computation */
		Point compute_scroll_view(const Area & area, Point & scroll_position, Size & scroll_size);
		Coord compute_scroll(Coord & scroll_position, Coord & m_scroll_position, Dim scroll_size, Dim area, Align align);
		Coord calc_shift_focus(Coord widget_position, Dim widget_size, Coord viewport_position, Dim viewport_size);
/// @endcond
	};
}
