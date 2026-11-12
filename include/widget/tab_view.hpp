#pragma once
namespace berialdraw
{
	/** A tab container with a scrollable tab bar and Pane pages. */
	class TabView : public Widget, public TextStyle, public BorderStyle, public TabViewStyle
	{
	public:
		/** Create a tab view. */
		TabView(Widget * parent);

		/** Destroy the tab view. */
		virtual ~TabView();

		/** Create a tab and its page, returning the page owned by this TabView. */
		Pane * add_tab(const String & title);

		/** Get the number of tabs. */
		Dim tab_count() const { return m_tab_id; }

		/** Get the selected tab index. */
		Dim selected_index() const { return m_selected_index; }

		/** Get the selected page. */
		Pane * selected_page() const;

		/** Get a page by its tab index. */
		Pane * page(Dim index) const;

		/** Select a tab by its index. */
		bool select(Dim index);

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
		/** Get a tab button by its index. */
		Button * tab_button(Dim index) const;

		/** Select a tab and optionally move focus to its button. */
		bool select_tab(Dim index, bool focus_tab);

		/** Check whether a widget or one of its descendants has focus. */
		bool contains_focus(Widget * widget) const;

		/** Handle a click on a tab button. */
		void on_click_tab(Widget * widget, const ClickEvent & evt);

		/** Handle keyboard navigation while the active tab button has focus. */
		void on_key_tab(Widget * widget, const KeyEvent & evt);

		Dim m_selected_index = UNDEFINED_INDEX;
		Grid * m_tab_layout = 0;
		ScrollView * m_scroll_tab = 0;
		Grid * m_tabs = 0;
		uint32_t m_tab_id = 0;
		Margin m_previous_tab_margin;
		Margin m_previous_tab_inter_margin;
		Align m_previous_tab_align = Align::ALIGN_DEFAULT;
		Align m_previous_tab_text_align = Align::ALIGN_DEFAULT;
		Align m_previous_vertical_tab_align = Align::ALIGN_DEFAULT;
		Align m_previous_vertical_tab_text_align = Align::ALIGN_DEFAULT;
		uint32_t m_previous_tab_color = 0;
		uint32_t m_previous_tab_text_color = 0;
		uint32_t m_previous_tab_selected_color = 0;
		uint32_t m_previous_tab_selected_text_color = 0;
		Borders m_previous_tab_vertical_borders = ALL_BORDERS;
		bool m_previous_tab_selected_color_defined = false;
		bool m_previous_tab_selected_text_color_defined = false;
		bool m_previous_tab_vertical_borders_defined = false;
		bool m_tab_button_style_initialized = false;
	};
}