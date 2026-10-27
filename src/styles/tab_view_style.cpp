#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Serialize TabView-specific style properties.
void TabViewStyle::serialize(JsonIterator & it)
{
	const char * direction = m_tab_direction == TabDirection::TAB_VERTICAL ? "vertical" : "horizontal";
	it[StyleNames::TABVIEW_TAB_DIRECTION] = direction;
}

// Unserialize TabView-specific style properties.
void TabViewStyle::unserialize(JsonIterator & it)
{
	String direction = it[StyleNames::TABVIEW_TAB_DIRECTION] | "";
	if (direction == "horizontal")
	{
		m_tab_direction = TabDirection::TAB_HORIZONTAL;
	}
	else if (direction == "vertical")
	{
		m_tab_direction = TabDirection::TAB_VERTICAL;
	}
}

// Copy TabView-specific style properties.
void TabViewStyle::set(const TabViewStyle & other)
{
	if (this != &other)
	{
		m_tab_direction = other.m_tab_direction;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

TabViewStyle& TabViewStyle::operator=(const TabViewStyle & other)
{
	set(other);
	return *this;
}

Style * TabViewStyle::create()
{
	return new TabViewStyle;
}

// Set the tab bar direction.
void TabViewStyle::tab_direction(TabDirection direction)
{
	if (m_tab_direction != direction)
	{
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
		m_tab_direction = direction;
	}
}
