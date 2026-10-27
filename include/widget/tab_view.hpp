#pragma once
namespace berialdraw
{
	/** A tab container with a horizontally scrollable tab bar and Pane pages. */
	class TabView : public Widget, public TextStyle, public BorderStyle, public TabViewStyle
	{
	public:
		/** Create a tab view. */
		TabView(Widget * parent);

		/** Destroy the tab view. */
		virtual ~TabView();

		/** Create a tab and its page, returning the page owned by this TabView. */
		Pane * add_tab(const String & title);

		/** Serialize the widget and its children. */
		virtual void serialize(JsonIterator & it) override;

		/** Restore widget styles. */
		virtual void unserialize(JsonIterator & it) override;

		/** Get the style cascade mode for this widget. */
		virtual StyleCascadeMode style_cascade_mode() const override;

#ifdef _DEBUG
		static void test();
		static void test1();
#endif

	protected:
		/** Place the internal layout in the TabView area. */
		virtual void place(const Area & area, bool in_layout) override;

		/** Update tab */
		void update_tabs();
	private:
		/** Handle a focus event */
		void on_focus_event(Widget * widget, const FocusEvent & evt);

		Dim m_selected_index = UNDEFINED_INDEX;
		Grid * m_tab_layout = 0;
		ScrollView * m_scroll_tab = 0;
		Grid * m_tabs = 0;
		uint32_t m_tab_id = 0;
	};
}