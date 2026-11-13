#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Serialize TabView-specific style properties.
void TabViewStyle::serialize(JsonIterator & it)
{
	const char * direction = m_tab_direction == TabDirection::TAB_VERTICAL ? "vertical" : "horizontal";
	it[StyleNames::TABVIEW_TAB_DIRECTION] = direction;
	Margin tab_margin_pixels(m_tab_margin.top(), m_tab_margin.left(), m_tab_margin.bottom(), m_tab_margin.right());
	Margin tab_inter_margin_pixels(m_tab_inter_margin.top(), m_tab_inter_margin.left(), m_tab_inter_margin.bottom(), m_tab_inter_margin.right());
	tab_margin_pixels.serialize(StyleNames::TABVIEW_TAB_MARGIN, it);
	tab_inter_margin_pixels.serialize(StyleNames::TABVIEW_TAB_INTER_MARGIN, it);
	berialdraw::serialize(StyleNames::TABVIEW_TAB_HORIZONTAL_BORDERS, it, m_tab_horizontal_borders);
	berialdraw::serialize(StyleNames::TABVIEW_TAB_VERTICAL_BORDERS, it, m_tab_vertical_borders);
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
	Margin tab_margin_pixels;
	Margin tab_inter_margin_pixels;
	tab_margin_pixels.unserialize(StyleNames::TABVIEW_TAB_MARGIN, it);
	tab_inter_margin_pixels.unserialize(StyleNames::TABVIEW_TAB_INTER_MARGIN, it);
	if (it.exists(StyleNames::TABVIEW_TAB_MARGIN))
	{
		m_tab_margin = Margin(tab_margin_pixels.top(), tab_margin_pixels.left(), tab_margin_pixels.bottom(), tab_margin_pixels.right());
	}
	if (it.exists(StyleNames::TABVIEW_TAB_INTER_MARGIN))
	{
		m_tab_inter_margin = Margin(tab_inter_margin_pixels.top(), tab_inter_margin_pixels.left(), tab_inter_margin_pixels.bottom(), tab_inter_margin_pixels.right());
	}
	if (it.exists(StyleNames::TABVIEW_TAB_HORIZONTAL_BORDERS))
	{
		berialdraw::unserialize(StyleNames::TABVIEW_TAB_HORIZONTAL_BORDERS, it, m_tab_horizontal_borders);
	}
	if (it.exists(StyleNames::TABVIEW_TAB_VERTICAL_BORDERS))
	{
		berialdraw::unserialize(StyleNames::TABVIEW_TAB_VERTICAL_BORDERS, it, m_tab_vertical_borders);
	}
}

// Copy TabView-specific style properties.
void TabViewStyle::set(const TabViewStyle & other)
{
	if (this != &other)
	{
		m_tab_direction = other.m_tab_direction;
		m_tab_margin = other.m_tab_margin;
		m_tab_inter_margin = other.m_tab_inter_margin;
		m_tab_horizontal_borders = other.m_tab_horizontal_borders;
		m_tab_vertical_borders = other.m_tab_vertical_borders;
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

// Set the margin around the tab bar.
void TabViewStyle::tab_margin(const Margin & margin)
{
	if (m_tab_margin != margin)
	{
		m_tab_margin = margin;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

// Set the margin between tab buttons.
void TabViewStyle::tab_inter_margin(const Margin & margin)
{
	if (m_tab_inter_margin != margin)
	{
		m_tab_inter_margin = margin;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

// Set the default borders for subsequently created horizontal tabs.
void TabViewStyle::tab_horizontal_borders(Borders borders)
{
	m_tab_horizontal_borders = borders;
}

// Set the default borders for subsequently created vertical tabs.
void TabViewStyle::tab_vertical_borders(Borders borders)
{
	m_tab_vertical_borders = borders;
}

