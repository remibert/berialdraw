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
	berialdraw::serialize(StyleNames::TABVIEW_TAB_VERTICAL_ALIGN, it, m_tab_vertical_align);
	berialdraw::serialize(StyleNames::TABVIEW_TAB_VERTICAL_TEXT_ALIGN, it, m_tab_vertical_text_align);
	if (m_has_tab_selected_color)
	{
		it[StyleNames::TABVIEW_TAB_SELECTED_COLOR] = (int)m_tab_selected_color;
	}
	if (m_has_tab_selected_text_color)
	{
		it[StyleNames::TABVIEW_TAB_SELECTED_TEXT_COLOR] = (int)m_tab_selected_text_color;
	}
	if (m_has_tab_vertical_borders)
	{
		JsonIterator tab_borders = it[StyleNames::TABVIEW_TAB_VERTICAL_BORDERS];
		berialdraw::serialize(tab_borders, m_tab_vertical_borders);
	}
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
	if (it.exists(StyleNames::TABVIEW_TAB_VERTICAL_ALIGN))
	{
		berialdraw::unserialize(StyleNames::TABVIEW_TAB_VERTICAL_ALIGN, it, m_tab_vertical_align);
	}
	if (it.exists(StyleNames::TABVIEW_TAB_VERTICAL_TEXT_ALIGN))
	{
		berialdraw::unserialize(StyleNames::TABVIEW_TAB_VERTICAL_TEXT_ALIGN, it, m_tab_vertical_text_align);
	}
	if (it.exists(StyleNames::TABVIEW_TAB_SELECTED_COLOR))
	{
		m_tab_selected_color = (int)(it[StyleNames::TABVIEW_TAB_SELECTED_COLOR] | (int)m_tab_selected_color);
		m_has_tab_selected_color = true;
	}
	if (it.exists(StyleNames::TABVIEW_TAB_SELECTED_TEXT_COLOR))
	{
		m_tab_selected_text_color = (int)(it[StyleNames::TABVIEW_TAB_SELECTED_TEXT_COLOR] | (int)m_tab_selected_text_color);
		m_has_tab_selected_text_color = true;
	}
	if (it.exists(StyleNames::TABVIEW_TAB_VERTICAL_BORDERS))
	{
		JsonIterator tab_borders = it[StyleNames::TABVIEW_TAB_VERTICAL_BORDERS];
		berialdraw::unserialize(tab_borders, m_tab_vertical_borders);
		m_has_tab_vertical_borders = true;
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
		m_tab_vertical_align = other.m_tab_vertical_align;
		m_tab_vertical_text_align = other.m_tab_vertical_text_align;
		m_tab_selected_color = other.m_tab_selected_color;
		m_tab_selected_text_color = other.m_tab_selected_text_color;
		m_has_tab_selected_color = other.m_has_tab_selected_color;
		m_has_tab_selected_text_color = other.m_has_tab_selected_text_color;
		m_tab_vertical_borders = other.m_tab_vertical_borders;
		m_has_tab_vertical_borders = other.m_has_tab_vertical_borders;
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

// Set the alignment of buttons in a vertical tab bar.
void TabViewStyle::tab_vertical_align(Align align)
{
	if (m_tab_vertical_align != align)
	{
		m_tab_vertical_align = align;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

// Set the text alignment in a vertical tab bar.
void TabViewStyle::tab_vertical_text_align(Align align)
{
	if (m_tab_vertical_text_align != align)
	{
		m_tab_vertical_text_align = align;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

// Set the selected tab background color.
void TabViewStyle::tab_selected_color(uint32_t color)
{
	if (m_has_tab_selected_color == false || m_tab_selected_color != color)
	{
		m_tab_selected_color = color;
		m_has_tab_selected_color = true;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

// Set the selected tab text color.
void TabViewStyle::tab_selected_text_color(uint32_t color)
{
	if (m_has_tab_selected_text_color == false || m_tab_selected_text_color != color)
	{
		m_tab_selected_text_color = color;
		m_has_tab_selected_text_color = true;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

// Set the vertical tab border configuration.
void TabViewStyle::tab_vertical_borders(Borders borders)
{
	if (m_has_tab_vertical_borders == false || m_tab_vertical_borders != borders)
	{
		m_tab_vertical_borders = borders;
		m_has_tab_vertical_borders = true;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}
