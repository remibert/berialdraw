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

		/** Get the default borders for subsequently created horizontal tabs. */
		Borders tab_horizontal_borders() const { return m_tab_horizontal_borders; }

		/** Set the default borders for subsequently created horizontal tabs. */
		void tab_horizontal_borders(Borders borders);

		/** Get the default borders for subsequently created vertical tabs. */
		Borders tab_vertical_borders() const { return m_tab_vertical_borders; }

		/** Set the default borders for subsequently created vertical tabs. */
		void tab_vertical_borders(Borders borders);

	protected:
		TabDirection m_tab_direction = TabDirection::TAB_HORIZONTAL;
		Margin m_tab_margin = {0,0,0,0};
		Margin m_tab_inter_margin = {0,0,0,0};
		Borders m_tab_horizontal_borders = ALL_BORDERS;
		Borders m_tab_vertical_borders = ALL_BORDERS;
	};
}