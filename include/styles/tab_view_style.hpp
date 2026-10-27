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

	protected:
		TabDirection m_tab_direction = TabDirection::TAB_HORIZONTAL;
		TabDirection m_tab_previous_direction = TabDirection::TAB_HORIZONTAL;
	};
}