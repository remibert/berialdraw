#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Create widget */
List::List(Widget * parent):
	ScrollableContent("list", parent, sizeof(List)),
	m_column(nullptr)
{
	UIManager::styles()->apply(this, (ListStyle*)this);

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
	return new ListItem(m_column, normalize_index(index));
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
	size_t sz = count();
	if (sz > 0)
	{
		int searched_index = normalize_index(index);
		int current_index = 0;

		Widget* current = m_column->children();
		Widget* previous = current;

		while (current)
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
			current = current->next();
			current_index++;
		}
	}
	return result;
}

/** Unselect all item in list */
void List::unselect_all()
{
	Widget* current = m_column->children();
	while (current) 
	{
		current->selected(false);
		current = current->next();
	}
}

/** Select one item in list
@param index Item position (supports negative indexing) */
void List::select(int index, bool state)
{
	if ((ListSelectionMode)m_selection_mode == ListSelectionMode::LIST_SINGLE_SELECTION ||
		(ListSelectionMode)m_selection_mode == ListSelectionMode::LIST_NO_SELECTION)
	{
		unselect_all();
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
				current->selected(state);
				break;
			}
			current = current->next();
			current_index++;
		}
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
		child = child->next();
		result++;
	}
	return result;
}

// Remove item at index (supports negative indexing like Python)
void List::remove(int index)
{
	ListItem* item = at(index);
	if (item)
	{
		// Delete from column - m_column will handle cleanup via Widget destructor chain
		delete item;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

// Remove all items from the list
void List::clear()
{
	// Clear all children of m_column
	m_column->clear();
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
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
	it["type"] = m_classname;
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
