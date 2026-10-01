#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
ListStyle::ListStyle()
{
	m_selection_mode = (int)ListSelectionMode::LIST_NO_SELECTION;
}


/** Serialize the content of list into json */
void ListStyle::serialize(JsonIterator & it)
{
	berialdraw::serialize(it, (ListSelectionMode)m_selection_mode);
}

/** Unserialize the content of list from json */
void ListStyle::unserialize(JsonIterator & it)
{
	ListSelectionMode mode = (ListSelectionMode)m_selection_mode;
	berialdraw::unserialize(it, mode);
	m_selection_mode = (int)mode;
}

/** Set properties with another */
void ListStyle::set(const ListStyle & other)
{
	if (this != &other)
	{
		m_selection_mode = other.m_selection_mode;
		UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	}
}


ListStyle& ListStyle::operator=(const ListStyle& other)
{
	set(other);
	return *this;
}

Style * ListStyle::create()
{
	return new ListStyle;
}

/** Set the selection mode */
void ListStyle::selection_mode(ListSelectionMode mode)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_selection_mode = (int)mode;
}
