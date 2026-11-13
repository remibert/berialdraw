#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Set the margin around the tab bar.
void TabStyle::margin(const Margin & value)
{
	if (m_margin != value)
	{
		m_margin = value;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

// Set the spacing around each tab button.
void TabStyle::inter_margin(const Margin & value)
{
	if (m_inter_margin != value)
	{
		m_inter_margin = value;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

// Set the borders applied to tab buttons.
void TabStyle::borders(Borders value)
{
	if (m_borders != value)
	{
		m_borders = value;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

// Serialize the profile properties.
void TabStyle::serialize(JsonIterator & it) const
{
	Margin margin_pixels(m_margin.top(), m_margin.left(), m_margin.bottom(), m_margin.right());
	Margin inter_margin_pixels(m_inter_margin.top(), m_inter_margin.left(), m_inter_margin.bottom(), m_inter_margin.right());
	margin_pixels.serialize(StyleNames::TABSTYLE_MARGIN, it);
	inter_margin_pixels.serialize(StyleNames::TABSTYLE_INTER_MARGIN, it);
	berialdraw::serialize(StyleNames::WIDGET_BORDERS, it, m_borders);
}

// Restore the profile properties.
void TabStyle::unserialize(JsonIterator & it)
{
	Margin margin_pixels;
	Margin inter_margin_pixels;
	margin_pixels.unserialize(StyleNames::TABSTYLE_MARGIN, it);
	inter_margin_pixels.unserialize(StyleNames::TABSTYLE_INTER_MARGIN, it);
	if (it.exists(StyleNames::TABSTYLE_MARGIN))
	{
		m_margin = Margin(margin_pixels.top(), margin_pixels.left(), margin_pixels.bottom(), margin_pixels.right());
	}
	if (it.exists(StyleNames::TABSTYLE_INTER_MARGIN))
	{
		m_inter_margin = Margin(inter_margin_pixels.top(), inter_margin_pixels.left(), inter_margin_pixels.bottom(), inter_margin_pixels.right());
	}
	if (it.exists(StyleNames::WIDGET_BORDERS))
	{
		berialdraw::unserialize(StyleNames::WIDGET_BORDERS, it, m_borders);
	}
}

// Copy the profile properties.
void TabStyle::set(const TabStyle & other)
{
	if (this != &other)
	{
		m_margin = other.m_margin;
		m_inter_margin = other.m_inter_margin;
		m_borders = other.m_borders;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

// Compare profile properties.
bool TabStyle::operator==(const TabStyle & other) const
{
	return m_margin == other.m_margin && m_inter_margin == other.m_inter_margin &&
		m_borders == other.m_borders;
}

// Serialize TabView-specific style properties.
void TabViewStyle::serialize(JsonIterator & it)
{
	const char * direction = m_tab_direction == TabDirection::TAB_VERTICAL ? "vertical" : "horizontal";
	it[StyleNames::TABVIEW_TAB_DIRECTION] = direction;
	JsonIterator horizontal_style = it[StyleNames::TABVIEW_HORIZONTAL_STYLE];
	JsonIterator vertical_style = it[StyleNames::TABVIEW_VERTICAL_STYLE];
	m_horizontal_tab_style.serialize(horizontal_style);
	m_vertical_tab_style.serialize(vertical_style);
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
	if (it.exists(StyleNames::TABVIEW_HORIZONTAL_STYLE))
	{
		JsonIterator horizontal_style = it[StyleNames::TABVIEW_HORIZONTAL_STYLE];
		m_horizontal_tab_style.unserialize(horizontal_style);
	}
	if (it.exists(StyleNames::TABVIEW_VERTICAL_STYLE))
	{
		JsonIterator vertical_style = it[StyleNames::TABVIEW_VERTICAL_STYLE];
		m_vertical_tab_style.unserialize(vertical_style);
	}
}

// Copy TabView-specific style properties.
void TabViewStyle::set(const TabViewStyle & other)
{
	if (this != &other)
	{
		m_tab_direction = other.m_tab_direction;
		m_horizontal_tab_style.set(other.m_horizontal_tab_style);
		m_vertical_tab_style.set(other.m_vertical_tab_style);
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

// Get a directional tab style profile.
TabStyle & TabViewStyle::tab_style(TabDirection direction)
{
	return direction == TabDirection::TAB_VERTICAL ? m_vertical_tab_style : m_horizontal_tab_style;
}

// Get a directional tab style profile.
const TabStyle & TabViewStyle::tab_style(TabDirection direction) const
{
	return direction == TabDirection::TAB_VERTICAL ? m_vertical_tab_style : m_horizontal_tab_style;
}

