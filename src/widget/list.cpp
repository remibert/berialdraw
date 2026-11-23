#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Create widget */
List::List(Widget * parent):
	ScrollableContent("list", parent, sizeof(List)),
	m_column(nullptr)
{
	UIManager::styles()->apply(this, (ListStyle*)this);

	bind(this, &List::on_key);

	// Set default scroll direction to vertical only
	m_scroll_direction = SCROLL_VERTICAL;
	align(Align::ALIGN_TOP);

	// Create internal column container
	m_column = new Column(this);
	m_column->margin(0, 0, 0, 0);
	m_column->size_policy(SizePolicy::ENLARGE_WIDTH);
}

/** Destroy widget */
List::~List()
{
}

/** Create new list item */
ListItem* List::new_item(int index)
{
	Vector<int32_t> previous_indices = selection_indices();
	ListItem* result = new ListItem(m_column, normalize_index(index));
	notify_selection_changed(previous_indices);
	return result;
}

int List::normalize_index(int index, bool for_insert) const
{
	int result = index;
	if (index != UNDEFINED_INDEX)
	{
		// For non-insert operations, skip normalization if index is 0
		if (!for_insert && index == 0)
		{
			return result;
		}

		size_t size = count();
		if (index < 0)
		{
			// Negative index: convert to positive
			int positive = (int)size + index;
			
			// For insert operations, add 1 to shift semantics (-1 means AFTER last, not BEFORE)
			if (for_insert)
			{
				positive++;
			}

			if (positive < 0)
			{
				result = 0;
			}
			else if (positive <= (int)size)
			{
				result = positive;
			}
			else
			{
				result = UNDEFINED_INDEX;
			}
		}
		else if (index > (int)size)
		{
			result = UNDEFINED_INDEX;
		}
	}
	return result;
}

// Append item with text only
ListItem* List::append(const String & text)
{
	ListItem* item = new_item(UNDEFINED_INDEX);
	item->text(text);
	return item;
}

// Append item with custom configuration via callback
ListItem* List::append(std::function<void(ListItem*)> config)
{
	ListItem* item = new_item(UNDEFINED_INDEX);
	if (config)
	{
		config(item);
	}
	return item;
}

// Prepend item with text only
ListItem* List::prepend(const String & text)
{
	ListItem* item = new_item(0);
	item->text(text);
	return item;
}

// Prepend item with custom configuration via callback
ListItem* List::prepend(std::function<void(ListItem*)> config)
{
	ListItem* item = new_item(0);
	if (config)
	{
		config(item);
	}
	return item;
}

// Insert item at specific index with text only
ListItem* List::insert(int index, const String & text)
{
	ListItem* item = new_item(normalize_index(index, true));
	item->text(text);
	return item;
}

// Insert item at specific index with custom configuration via callback
ListItem* List::insert(int index, std::function<void(ListItem*)> config)
{
	ListItem* item = new_item(normalize_index(index, true));
	if (config)
	{
		config(item);
	}
	return item;
}

// Get item at index (supports negative indexing like Python)
ListItem* List::at(int index) const
{
	ListItem* result = nullptr;
	size_t size = count();
	if (size > 0)
	{
		int searched_index = normalize_index(index);
		int current_index = 0;

		Widget* current = m_column->children();
		Widget* previous = current;

		while (current)
		{
			if (dynamic_cast<ListItem*>(current))
			{
				if (searched_index == current_index)
				{
					result = dynamic_cast<ListItem*>(current);
					break;
				}

				if (current->next() == 0)
				{
					result = dynamic_cast<ListItem*>(current);
					break;
				}
				current_index++;
			}
			current = current->next();
		}
	}
	return result;
}

/** Unselect all item in list */
void List::unselect_all()
{
	Vector<int32_t> previous_indices = selection_indices();
	Widget* current = m_column->children();
	while (current)
	{
		if (dynamic_cast<ListItem*>(current))
		{
			current->selected(false);
		}
		current = current->next();
	}
	notify_selection_changed(previous_indices);
}

/** Select all items when multi-selection is enabled */
void List::select_all()
{
	Vector<int32_t> previous_indices = selection_indices();
	if ((ListSelectionMode)m_selection_mode == ListSelectionMode::LIST_MULTI_SELECTION)
	{
		Widget* current = m_column->children();
		while (current)
		{
			ListItem* item = dynamic_cast<ListItem*>(current);
			if (item)
			{
				item->selected(true);
			}
			current = current->next();
		}
	}
	notify_selection_changed(previous_indices);
}

/** Select one item in list
@param index Item position (supports negative indexing) */
void List::select(int index, bool state)
{
	Vector<int32_t> previous_indices = selection_indices();
	if ((ListSelectionMode)m_selection_mode == ListSelectionMode::LIST_SINGLE_SELECTION ||
		(ListSelectionMode)m_selection_mode == ListSelectionMode::LIST_NO_SELECTION)
	{
		Widget* current = m_column->children();
		while (current)
		{
			if (dynamic_cast<ListItem*>(current))
			{
				current->selected(false);
			}
			current = current->next();
		}
	}

	if ((ListSelectionMode)m_selection_mode != ListSelectionMode::LIST_NO_SELECTION)
	{
		int searched_index = normalize_index(index);
		int current_index = 0;

		Widget* current = m_column->children();
		Widget* previous = current;

		while (current)
		{
			if (searched_index == current_index)
			{
				if (dynamic_cast<ListItem*>(current))
				{
					current->selected(state);
				}
				break;
			}
			current = current->next();
			current_index++;
		}
	}
	notify_selection_changed(previous_indices);
}

// Get the selected indices in display order.
Vector<int32_t> List::selection_indices() const
{
	Vector<int32_t> result;
	int32_t item_index = 0;
	Widget* current = m_column ? m_column->children() : nullptr;
	while (current)
	{
		if (dynamic_cast<ListItem*>(current))
		{
			if (current->selected())
			{
				result.push_back(item_index);
			}
			item_index++;
		}
		current = current->next();
	}
	return result;
}

// Notify listeners if the selected indices changed.
void List::notify_selection_changed(const Vector<int32_t> & previous_indices)
{
	Vector<int32_t> current_indices = selection_indices();
	bool changed = previous_indices.size() != current_indices.size();
	for (uint32_t index = 0; changed == false && index < current_indices.size(); index++)
	{
		changed = previous_indices[index] != current_indices[index];
	}
	if (changed)
	{
		UIManager::notifier()->push_event(new SelectionEvent(previous_indices, current_indices, this));
	}
}

/** Select all item in list
@param index Item position (supports negative indexing) */
void List::select(int index)
{
	select(index, true);
}

/** Unselect one item in list
@param index Item position (supports negative indexing) */
void List::unselect(int index)
{
	select(index, false);
}

// Array access operator (delegates to at())
ListItem* List::operator[](int index) const
{
	return at(index);
}

// Get number of items in the list
size_t List::count() const
{
	size_t result = 0;
	Widget* child = m_column->children();
	while (child)
	{
		if (dynamic_cast<ListItem*>(child))
		{
			result++;
		}
		child = child->next();
	}
	return result;
}

// Remove item at index (supports negative indexing like Python)
void List::remove(int index)
{
	ListItem* item = at(index);
	if (item)
	{
		Vector<int32_t> previous_indices = selection_indices();
		// Delete from column - m_column will handle cleanup via Widget destructor chain
		delete item;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
		notify_selection_changed(previous_indices);
	}
}

// Remove all items from the list
void List::clear()
{
	Vector<int32_t> previous_indices = selection_indices();
	// Clear all children of m_column
	m_column->clear();
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	notify_selection_changed(previous_indices);
}

// Get all selected items from the list
Vector<ListItem*> List::selected_items() const
{
	Vector<ListItem*> result;
	Widget* current = m_column->children();
	while (current)
	{
		if (current->selected())
		{
			result.push_back(dynamic_cast<ListItem*>(current));
		}
		current = current->next();
	}
	return result;
}

/** Get the selected item in case of only one item can be selected 
@return the selected item or null if no item selected */
ListItem * List::selected_item() const
{
	ListItem * result = nullptr;

	if ((ListSelectionMode)m_selection_mode == ListSelectionMode::LIST_SINGLE_SELECTION)
	{
		Widget* current = m_column->children();
		while (current)
		{
			ListItem * item = dynamic_cast<ListItem*>(current);
			if (item)
			{
				if (item->selected())
				{
					result = item;
					break;
				}
			}
			current = current->next();
		}
	}
	return result;
}

/** Select the next item in case of only one item can be selected 
@return the selected item or null if no item selected */
ListItem * List::select_next()
{
	ListItem * result = nullptr;

	if ((ListSelectionMode)m_selection_mode == ListSelectionMode::LIST_SINGLE_SELECTION)
	{
		ListItem * selected = selected_item();

		// If no item selected
		if (selected == nullptr)
		{
			result = select_first();
		}
		else
		{
			int selected_index = -1;
			int index = 0;
			Widget* current = m_column->children();
			while (current)
			{
				if (dynamic_cast<ListItem*>(current))
				{
					if (current == selected)
					{
						selected_index = index;
						break;
					}
					index++;
				}
				current = current->next();
			}
			if (selected_index >= 0 && selected_index + 1 < (int)count())
			{
				select(selected_index + 1);
				result = at(selected_index + 1);
			}
		}
	}

	// Move the scrollview to selected item
	scroll_focus(result);
	return result;
}

/** Select the previous item in case of only one item can be selected
@return the selected item or null if no item selected */
ListItem * List::select_previous()
{
	ListItem * result = nullptr;

	if ((ListSelectionMode)m_selection_mode == ListSelectionMode::LIST_SINGLE_SELECTION)
	{
		ListItem * sel = selected_item();

		// If no item selected
		if (sel == nullptr)
		{
			result = select_last();
		}
		// Select the previous item
		else
		{
			int selected_index = -1;
			int index = 0;
			Widget* current = m_column->children();
			while (current)
			{
				if (dynamic_cast<ListItem*>(current))
				{
					if (current == sel)
					{
						selected_index = index;
						break;
					}
					index++;
				}
				current = current->next();
			}
			if (selected_index > 0)
			{
				select(selected_index - 1);
				result = at(selected_index - 1);
			}
		}
	}

	// Move the scrollview to selected item
	scroll_focus(result);
	return result;
}

/** Select the first item in case of only one item can be selected
@return the selected item or null if no item selected */
ListItem * List::select_first()
{
	ListItem * result = nullptr;
	if ((ListSelectionMode)m_selection_mode == ListSelectionMode::LIST_SINGLE_SELECTION)
	{
		if (count() > 0)
		{
			select(0);
			result = at(0);
		}
	}
	return result;
}

/** Select the last item in case of only one item can be selected
@return the selected item or null if no item selected */
ListItem * List::select_last()
{
	ListItem * result = nullptr;
	if ((ListSelectionMode)m_selection_mode == ListSelectionMode::LIST_SINGLE_SELECTION)
	{
		if (count() > 0)
		{
			select((int)count() - 1);
			result = at((int)count() - 1);
		}
	}
	return result;
}

// Get all items from the list
Vector<ListItem*> List::items() const
{
	Vector<ListItem*> result;
	Widget* current = m_column->children();
	while (current)
	{
		result.push_back(dynamic_cast<ListItem*>(current));
		current = current->next();
	}
	return result;
}

/** Copy all styles of the list */
void List::copy(const List& list)
{
	ScrollableContent::copy(*(ScrollableContent*)(&list));
	*((ListStyle*)this) = *(ListStyle*)(&list);
}

/** Copy all styles of the list */
void List::copy(const List* list)
{
	if (list)
	{
		copy(*list);
	}
}

/** Serialize the content of widget into json */
void List::serialize(JsonIterator & it)
{
	it[StyleNames::WIDGET_TYPE] = m_classname;
	ScrollableContent::serialize(it);
	ListStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void List::unserialize(JsonIterator & it)
{
	ScrollableContent::unserialize(it);
	ListStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

/** Get the style cascade mode for this widget */
StyleCascadeMode List::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}

/** Call back on key */
void List::on_key(Widget * widget, const KeyEvent & evt)
{
	if (m_focused)
	{
		if ((ListSelectionMode)m_selection_mode == ListSelectionMode::LIST_SINGLE_SELECTION)
		{
			if (evt.state() == KeyEvent::KEY_DOWN)
			{
				if (evt.key() == (wchar_t)ReservedKey::KEY_DOWN)
				{
					select_next();
				}
				else if (evt.key() == (wchar_t)ReservedKey::KEY_UP)
				{
					select_previous();
				}
				else if (evt.key() == (wchar_t)ReservedKey::KEY_ENTER)
				{
					
				}
			}
		}
	}
}