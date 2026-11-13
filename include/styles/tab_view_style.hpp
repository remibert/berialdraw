#pragma once
namespace berialdraw
{
	/** Layout and border defaults for tabs in one orientation. */
	class TabStyle
	{
	public:
		/** Get the margin around the tab bar. */
		const Margin & margin() const { return m_margin; }

		/** Set the margin around the tab bar. */
		void margin(const Margin & value);

		/** Get the spacing around each tab button. */
		const Margin & inter_margin() const { return m_inter_margin; }

		/** Set the spacing around each tab button. */
		void inter_margin(const Margin & value);

		/** Get the borders applied to tab buttons. */
		Borders borders() const { return m_borders; }

		/** Set the borders applied to tab buttons. */
		void borders(Borders value);

		/** Serialize the profile properties. */
		void serialize(JsonIterator & it) const;

		/** Restore the profile properties. */
		void unserialize(JsonIterator & it);

		/** Copy the profile properties. */
		void set(const TabStyle & other);

		/** Compare profile properties. */
		bool operator==(const TabStyle & other) const;

		/** Compare profile properties. */
		bool operator!=(const TabStyle & other) const { return !(*this == other); }

	private:
		Margin m_margin = {0,0,0,0};
		Margin m_inter_margin = {0,0,0,0};
		Borders m_borders = ALL_BORDERS;
	};

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

		/** Get a directional tab style profile. */
		TabStyle & tab_style(TabDirection direction);

		/** Get a directional tab style profile. */
		const TabStyle & tab_style(TabDirection direction) const;

		/** Get the margin around the tab bar in the active direction. */
		const Margin & tab_margin() const { return tab_style(m_tab_direction).margin(); }

		/** Set the margin around the tab bar in the active direction. */
		void tab_margin(const Margin & margin) { tab_style(m_tab_direction).margin(margin); }

		/** Get the margin between tab buttons in the active direction. */
		const Margin & tab_inter_margin() const { return tab_style(m_tab_direction).inter_margin(); }

		/** Set the margin between tab buttons in the active direction. */
		void tab_inter_margin(const Margin & margin) { tab_style(m_tab_direction).inter_margin(margin); }

		/** Get the borders for tabs in the active direction. */
		Borders tab_borders() const { return tab_style(m_tab_direction).borders(); }

		/** Set the borders for tabs in the active direction. */
		void tab_borders(Borders borders) { tab_style(m_tab_direction).borders(borders); }

		/** Get the horizontal tab profile. */
		TabStyle & horizontal_tab_style() { return m_horizontal_tab_style; }
		const TabStyle & horizontal_tab_style() const { return m_horizontal_tab_style; }

		/** Get the vertical tab profile. */
		TabStyle & vertical_tab_style() { return m_vertical_tab_style; }
		const TabStyle & vertical_tab_style() const { return m_vertical_tab_style; }

	protected:
		TabDirection m_tab_direction = TabDirection::TAB_HORIZONTAL;
		TabStyle m_horizontal_tab_style;
		TabStyle m_vertical_tab_style;
	};
}