#pragma once
namespace berialdraw
{
	/** Side of the TabView on which its tab bar is placed. */
	enum class TabPosition : uint8_t
	{
		TOP,
		RIGHT,
		BOTTOM,
		LEFT
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

		/** Get the side on which the tab bar is placed. */
		TabPosition tab_position() const { return m_tab_position; }

		/** Set the side on which the tab bar is placed. */
		void tab_position(TabPosition position);

		/** Get the canonical top-position margin around the tab bar. */
		const Margin & tab_margin() const { return m_tab_margin; }

		/** Set the canonical top-position margin around the tab bar. */
		void tab_margin(const Margin & margin);

	protected:
		TabPosition m_tab_position = TabPosition::TOP;
		Margin m_tab_margin = {0,0,0,0};
	};
}