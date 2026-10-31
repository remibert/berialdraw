#pragma once
namespace berialdraw
{
	/** A transient, anchored container displayed above the normal window content. */
	class Popup : public Pane
	{
	public:
		/** Create a popup owned by a window. */
		Popup(Window * parent);

		/** Destroy the popup. */
		virtual ~Popup();

		/** Open the popup below or above an anchor widget. */
		bool open(Widget * anchor);

		/** Open the popup at a point in the same coordinate space as widget positions and click events. */
		bool open_at(const Point & position);

		/** Close the popup. */
		void close();

		/** Check whether the popup is open. */
		bool is_open() const;

		/** Check whether a widget belongs to this popup's subtree. */
		bool contains(Widget * widget) const;

		/** Transient widgets are omitted from persistent widget serialization. */
		virtual bool is_transient() const override { return true; }

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
#endif

	protected:
		virtual void place(const Area & area, bool in_layout) override;
		virtual Widget * hovered(const Region & parent_region, const Point & position) override;

	private:
		friend class Window;

		/** Check whether the anchor is visible within its clipping ancestors. */
		bool anchor_is_visible() const;

		/** Fit the popup size to the available area. */
		void fit_size_to_area(const Area & area, Dim & width, Dim & height);

		/** Position the popup relative to its anchor. */
		void position_popup(const Area & area, Dim width, Dim height);

		/** Place an open popup and restore its size constraints. */
		void place_open_popup(const Area & area, bool & in_layout);

		/** Activate the popup in its parent window. */
		void activate(Window * window);

		/** Close the popup and optionally keep focus transfer from scrolling its target. */
		void close_internal(bool scroll_to_focus);

		Widget * m_anchor = 0;
		Point m_anchor_position;
		bool m_open = false;
		bool m_point_anchor = false;
	};
}