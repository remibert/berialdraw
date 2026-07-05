#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Create widget */
List::List(Widget * parent):
	ScrollableContent("list", parent, sizeof(List)),
	m_column(nullptr)
{
	// Apply styles
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (ListStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);

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
ListItem* List::new_item()
{
	return new ListItem(m_column);
}

/** Copy all styles of the list */
void List::copy(const List& list)
{
	ScrollableContent::copy(*(ScrollableContent*)(&list));
	*((ListStyle      *)this) = *(ListStyle      *)(&list);
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
	ListStyle::serialize(it);
	ScrollableContent::serialize(it);
}

/** Unserialize the content of widget from json */
void List::unserialize(JsonIterator & it)
{
	ListStyle::unserialize(it);
	ScrollableContent::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

/** Get the style cascade mode for this widget */
StyleCascadeMode List::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}
