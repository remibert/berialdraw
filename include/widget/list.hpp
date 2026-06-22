#pragma once

namespace berialdraw
{
	/** Forward declaration for list item */
	class ListItem;

	/** The List class provides a scrollable list container for displaying items with optional icons and arrows. It supports selection, enabled/disabled state, and menu-style arrows. */
	class List : public ScrollableContent, public ListStyle, public BorderStyle, public TextStyle
	{
	public:
		/** Create widget */
		List(Widget * parent);

		/** Destroy widget */
		virtual ~List();

		/** Add an item to the list
		@param text The text to display
		@param icon Optional icon filename (can be null)
		@param enabled Whether the item is enabled (default: true)
		@return The index of the added item */
		uint16_t add(const String & text, const char * icon = nullptr, bool enabled = true);

		/** Insert an item at a specific index
		@param index The position to insert at
		@param text The text to display
		@param icon Optional icon filename (can be null)
		@param enabled Whether the item is enabled (default: true) */
		void insert(uint16_t index, const String & text, const char * icon = nullptr, bool enabled = true);

		/** Remove an item at a specific index
		@param index The index of the item to remove */
		void remove(uint16_t index);

		/** Clear all items from the list */
		virtual void clear() override;

		/** Get the number of items in the list */
		uint16_t count() const;

		/** Get the currently selected item index (0xFFFF if none) */
		uint16_t selected() const { return m_selected; }

		/** Set the selected item index */
		void select(uint16_t index);

		/** Enable or disable an item
		@param index The item index
		@param enabled The enabled state */
		void set_enabled(uint16_t index, bool enabled);

		/** Check if an item is enabled
		@param index The item index
		@return true if enabled */
		bool is_enabled(uint16_t index) const;

		/** Get the text of an item
		@param index The item index
		@return The item text (empty string if invalid index) */
		String text(uint16_t index) const;

		/** Set the text of an item
		@param index The item index
		@param text The new text */
		void set_text(uint16_t index, const String & text);

		/** Get the icon of an item
		@param index The item index
		@return The icon filename (empty string if none) */
		String icon(uint16_t index) const;

		/** Set the icon of an item
		@param index The item index
		@param icon The icon filename (can be null to remove) */
		void set_icon(uint16_t index, const char * icon);

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it) override;

		/** Get the style cascade mode for this widget */
		virtual StyleCascadeMode style_cascade_mode() const override;

		/** Return the size of content without marges */
		virtual Size content_size() override;

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
		static void test6();
		static void test7();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		List& operator=(const List& other) = delete;

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region) override;

		/** Paint separators between items */
		void paint_separators();

		/** Paint the border around the list */
		void paint_border();

		/** Event handler for item clicks */
		void on_item_click(Widget * widget, const ClickEvent & evt);

		/** Event handler for focus changes to skip disabled items */
		void on_item_focus(Widget * widget, const FocusEvent & evt);

		/** Create a list item button */
		Button * create_item(const String & text, const char * icon, bool enabled);

		/** Update item appearance based on enabled state */
		void update_item_appearance(Button * button, bool enabled);

		/** Get button at index */
		Button * get_button(uint16_t index) const;

		/** Internal column container */
		Column * m_column;

		/** Selected item index (0xFFFF = none) */
		uint16_t m_selected = 0xFFFF;

		/** Vector of list items (Button pointers) */
		Vector<Button*> m_items;
/// @endcond
	};
}
