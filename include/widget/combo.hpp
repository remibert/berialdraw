#pragma once

namespace berialdraw
{
	class Button;
	class List;
	class ListItem;
	class Popup;

	/** The Combo class combines an editable text field with a popup list of options. */
	class Combo : public Edit, public ComboStyle
	{
	public:
		/** Create a combo widget.
		@param parent Parent widget */
		Combo(Widget * parent);

		/** Destroy the combo widget and close its popup. */
		virtual ~Combo();

		/** Check whether the text field can be edited. */
		bool editable() const { return ComboStyle::editable(); }

		/** Enable or disable text editing.
		@param state True to allow editing, false to select existing options only */
		void editable(bool state);

		/** Open the popup list.
		@return True if the popup was opened */
		bool open();

		/** Close the popup list. */
		void close();

		/** Check whether the popup list is open. */
		bool is_open() const;

		/** Create an unconfigured item at an index.
		@param index Item position; supports negative indexing */
		ListItem * new_item(int index = UNDEFINED_INDEX);

		/** Append an item with text.
		@param text Item text
		@return The created item */
		ListItem * append(const String & text);

		/** Append an item and configure it with a callback.
		@param config Item configuration callback
		@return The created item */
		ListItem * append(std::function<void(ListItem*)> config);

		/** Prepend an item with text.
		@param text Item text
		@return The created item */
		ListItem * prepend(const String & text);

		/** Prepend an item and configure it with a callback.
		@param config Item configuration callback
		@return The created item */
		ListItem * prepend(std::function<void(ListItem*)> config);

		/** Insert an item with text at an index.
		@param index Item position; supports negative indexing
		@param text Item text
		@return The created item */
		ListItem * insert(int index, const String & text);

		/** Insert an item at an index and configure it with a callback.
		@param index Item position; supports negative indexing
		@param config Item configuration callback
		@return The created item */
		ListItem * insert(int index, std::function<void(ListItem*)> config);

		/** Get the item at an index.
		@param index Item position; supports negative indexing
		@return The item, or null if the combo is empty */
		ListItem * at(int index) const;

		/** Get the item at an index using array syntax.
		@param index Item position; supports negative indexing
		@return The item, or null if the combo is empty */
		ListItem * operator[](int index) const;

		/** Get the number of options. */
		size_t count() const;

		/** Check whether the combo has no options. */
		bool is_empty() const;

		/** Select an option and copy its text into the edit field.
		@param index Item position; supports negative indexing */
		void select(int index);

		/** Select the next option and update the edit field.
		@return The selected item, or null if there are no options */
		ListItem * select_next();

		/** Select the previous option and update the edit field.
		@return The selected item, or null if there are no options */
		ListItem * select_previous();

		/** Select the first option and update the edit field.
		@return The selected item, or null if there are no options */
		ListItem * select_first();

		/** Select the last option and update the edit field.
		@return The selected item, or null if there are no options */
		ListItem * select_last();

		/** Unselect an option.
		@param index Item position; supports negative indexing */
		void unselect(int index);

		/** Unselect all options. */
		void unselect_all();

		/** Get the selected option if its text matches the edit field.
		@return The selected item, or null if no option matches */
		ListItem * selected_item() const;

		/** Get the selected option index.
		@return The selected index, or -1 if no option matches */
		int selected_index() const;

		/** Get all selected options. */
		Vector<ListItem*> selected_items() const;

		/** Get all options in display order. */
		Vector<ListItem*> items() const;

		/** Remove an option.
		@param index Item position; supports negative indexing */
		void remove(int index);

		/** Remove all options and clear the edit field. */
		void clear() override;

		/** Serialize the combo to JSON.
		@param it JSON iterator receiving the serialized properties */
		void serialize(JsonIterator & it) override;

		/** Restore the combo from JSON.
		@param it JSON iterator containing the serialized properties */
		void unserialize(JsonIterator & it) override;

		/** Get the style cascade mode for this widget. */
		StyleCascadeMode style_cascade_mode() const override;

#ifdef _DEBUG
		/// @cond DOXYGEN_IGNORE
		static void test();
		static void test1();
		/// @endcond
#endif

	protected:
		/// @cond DOXYGEN_IGNORE
		void place(const Area & area, bool in_layout) override;
		Size content_size() override;
		Widget * hovered(const Region & parent_region, const Point & position) override;
		void on_key(Widget * widget, const KeyEvent & evt);
		void on_button_click(Widget * widget, const ClickEvent & evt);
		void on_item_click(Widget * widget, const ClickEvent & evt);
		void set_value(const String & value);
		void update_editable_state();

		Button * m_button = 0;
		Popup * m_popup = 0;
		List * m_list = 0;
		Entry::TypingMode m_editable_typing_mode = Entry::TypingMode::INSERTION;
		/// @endcond
	};
}