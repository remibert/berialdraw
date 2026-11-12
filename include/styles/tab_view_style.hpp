#pragma once
namespace berialdraw
{
	/** Style properties specific to TabView layout. */
	class TabViewStyle : public Style
	{
	public:
		/** Serialize style properties. */
		void serialize(JsonIterator & it) override;

		/** Unserialize style properties. */
		void unserialize(JsonIterator & it) override;

		/** Copy style properties. */
		TabViewStyle& operator=(const TabViewStyle & other);

		/** Set style properties. */
		void set(const TabViewStyle & other);

		/** Create a TabViewStyle instance for the style system. */
		static Style * create();

		/** Get the style property name. */
		const char * property_name() const override { return "tabview"; }

		enum class TabDirection : uint8_t
		{
			TAB_HORIZONTAL,
			TAB_VERTICAL
		};

		/** Get the tab bar direction. */
		TabDirection tab_direction() const { return m_tab_direction; }

		/** Set the tab bar direction. */
		void tab_direction(TabDirection direction);

		/** Get the margin around the tab bar. */
		const Margin & tab_margin() const { return m_tab_margin; }

		/** Set the margin around the tab bar. */
		void tab_margin(const Margin & margin);

		/** Get the margin between tab buttons. */
		const Margin & tab_inter_margin() const { return m_tab_inter_margin; }

		/** Set the margin between tab buttons. */
		void tab_inter_margin(const Margin & margin);

		/** Get the alignment of buttons in a vertical tab bar. */
		Align tab_vertical_align() const { return m_tab_vertical_align; }

		/** Set the alignment of buttons in a vertical tab bar. */
		void tab_vertical_align(Align align);

		/** Get the text alignment in a vertical tab bar. */
		Align tab_vertical_text_align() const { return m_tab_vertical_text_align; }

		/** Set the text alignment in a vertical tab bar. */
		void tab_vertical_text_align(Align align);

		/** Check whether a vertical tab border configuration is defined. */
		bool has_tab_vertical_borders() const { return m_has_tab_vertical_borders; }

		/** Get the vertical tab border configuration. */
		Borders tab_vertical_borders() const { return m_tab_vertical_borders; }

		/** Set the vertical tab border configuration. */
		void tab_vertical_borders(Borders borders);

		/** Check whether a selected tab background color is defined. */
		bool has_tab_selected_color() const { return m_has_tab_selected_color; }

		/** Get the selected tab background color. */
		uint32_t tab_selected_color() const { return m_tab_selected_color; }

		/** Set the selected tab background color. */
		void tab_selected_color(uint32_t color);

		/** Check whether a selected tab text color is defined. */
		bool has_tab_selected_text_color() const { return m_has_tab_selected_text_color; }

		/** Get the selected tab text color. */
		uint32_t tab_selected_text_color() const { return m_tab_selected_text_color; }

		/** Set the selected tab text color. */
		void tab_selected_text_color(uint32_t color);

	protected:
		TabDirection m_tab_direction = TabDirection::TAB_HORIZONTAL;
		TabDirection m_tab_previous_direction = TabDirection::TAB_HORIZONTAL;
		Margin m_tab_margin = {0,0,0,0};
		Margin m_tab_inter_margin = {0,0,0,0};
		Align m_tab_vertical_align = Align::ALIGN_LEFT;
		Align m_tab_vertical_text_align = Align::CENTER;
		uint32_t m_tab_selected_color = 0;
		uint32_t m_tab_selected_text_color = 0;
		bool m_has_tab_selected_color = false;
		bool m_has_tab_selected_text_color = false;
		Borders m_tab_vertical_borders = ALL_BORDERS;
		bool m_has_tab_vertical_borders = false;
	};
}