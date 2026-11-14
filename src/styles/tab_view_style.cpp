#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Serialize TabView-specific style properties.
void TabViewStyle::serialize(JsonIterator & it)
{
	const char * position = "top";
	switch (m_tab_position)
	{
	case TabPosition::RIGHT: position = "right"; break;
	case TabPosition::BOTTOM: position = "bottom"; break;
	case TabPosition::LEFT: position = "left"; break;
	case TabPosition::TOP: default: position = "top"; break;
	}
	it[StyleNames::TABVIEW_TAB_POSITION] = position;
	Margin tab_margin_pixels(m_tab_margin.top(), m_tab_margin.left(), m_tab_margin.bottom(), m_tab_margin.right());
	tab_margin_pixels.serialize(StyleNames::TABVIEW_TAB_MARGIN, it);
}

// Unserialize TabView-specific style properties.
void TabViewStyle::unserialize(JsonIterator & it)
{
	String position = it[StyleNames::TABVIEW_TAB_POSITION] | "";
	if (position == "top" || position == "horizontal")
	{
		m_tab_position = TabPosition::TOP;
	}
	else if (position == "right")
	{
		m_tab_position = TabPosition::RIGHT;
	}
	else if (position == "bottom")
	{
		m_tab_position = TabPosition::BOTTOM;
	}
	else if (position == "left" || position == "vertical")
	{
		m_tab_position = TabPosition::LEFT;
	}
	Margin tab_margin_pixels;
	tab_margin_pixels.unserialize(StyleNames::TABVIEW_TAB_MARGIN, it);
	if (it.exists(StyleNames::TABVIEW_TAB_MARGIN))
	{
		m_tab_margin = Margin(tab_margin_pixels.top(), tab_margin_pixels.left(), tab_margin_pixels.bottom(), tab_margin_pixels.right());
	}
}

// Copy TabView-specific style properties.
void TabViewStyle::set(const TabViewStyle & other)
{
	if (this != &other)
	{
		m_tab_position = other.m_tab_position;
		m_tab_margin = other.m_tab_margin;
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

// Set the tab bar position.
void TabViewStyle::tab_position(TabPosition position)
{
	if (m_tab_position != position)
	{
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
		m_tab_position = position;
	}
}

// Set the canonical top-position margin around the tab bar.
void TabViewStyle::tab_margin(const Margin & margin)
{
	if (m_tab_margin != margin)
	{
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
		m_tab_margin = margin;
	}
}

