#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
TableViewStyle::TableViewStyle()
{
}


/** Serialize the content of widget into json */
void TableViewStyle::serialize(JsonIterator & it)
{
	it["grid-color"] = m_grid_color;
	it["horizontal-thickness_"] = m_horizontal_thickness;
	it["vertical-thickness_"] = m_vertical_thickness;
	it["grid-visible"] = m_grid_visible;
	it["header-visible"] = m_header_visible;
	it["header-background-color"] = m_header_background_color;
	it["alternating-row-color1"] = m_alternating_row_color1;
	it["alternating-row-color2"] = m_alternating_row_color2;
	berialdraw::serialize("scroll-direction", it, m_scroll_direction);
}

/** Unserialize the content of widget from json */
void TableViewStyle::unserialize(JsonIterator & it)
{
	m_grid_color = (int)(it["grid-color"] | (int)m_grid_color);
	berialdraw::unserialize("horizontal-thickness_", it, m_horizontal_thickness);
	berialdraw::unserialize("vertical-thickness_", it, m_vertical_thickness);
	m_grid_visible = (bool)(it["grid-visible"] | (int)m_grid_visible);
	m_header_visible = (bool)(it["header-visible"] | (int)m_header_visible);
	m_header_background_color = (int)(it["header-background-color"] | (int)m_header_background_color);
	m_alternating_row_color1 = (int)(it["alternating-row-color1"] | (int)m_alternating_row_color1);
	m_alternating_row_color2 = (int)(it["alternating-row-color2"] | (int)m_alternating_row_color2);
	berialdraw::unserialize("scroll-direction", it, m_scroll_direction);
}

/** Set properties with another */
void TableViewStyle::set(const TableViewStyle & other)
{
	if (this != &other)
	{
		m_grid_color = other.m_grid_color;
		m_horizontal_thickness = other.m_horizontal_thickness;
		m_vertical_thickness = other.m_vertical_thickness;
		m_grid_visible = other.m_grid_visible;
		m_header_visible = other.m_header_visible;
		m_header_background_color = other.m_header_background_color;
		m_alternating_row_color1 = other.m_alternating_row_color1;
		m_alternating_row_color2 = other.m_alternating_row_color2;
		m_scroll_direction = other.m_scroll_direction;
		UIManager::invalidator()->dirty(this, Invalidator::ALL);
	}
}


TableViewStyle& TableViewStyle::operator=(const TableViewStyle& other)
{
	set(other);
	return *this;
}

Style * TableViewStyle::create()
{
	return new TableViewStyle;
}

/** Set the grid color */
void TableViewStyle::grid_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_grid_color = col;
}

/** Set the grid color with alpha */
void TableViewStyle::grid_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_grid_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Set the horizontal grid thickness in pixels */
void TableViewStyle::horizontal_thickness(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_horizontal_thickness = v << 6;
}

/** Set the horizontal grid thickness with a precision of 64th of a pixel */
void TableViewStyle::horizontal_thickness_(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_horizontal_thickness = v;
}

/** Set the vertical grid thickness in pixels */
void TableViewStyle::vertical_thickness(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_vertical_thickness = v << 6;
}

/** Set the vertical grid thickness with a precision of 64th of a pixel */
void TableViewStyle::vertical_thickness_(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_vertical_thickness = v;
}

/** Set the grid visibility state */
void TableViewStyle::grid_visible(bool visible)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_grid_visible = visible;
}

/** Set the header visibility state */
void TableViewStyle::header_visible(bool visible)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_header_visible = visible;
}

/** Set the header background color */
void TableViewStyle::header_background_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_header_background_color = col;
}

/** Set the header background color with alpha */
void TableViewStyle::header_background_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_header_background_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Set the alternating row color 1 */
void TableViewStyle::alternating_row_color1(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_alternating_row_color1 = col;
}

/** Set the alternating row color 1 with alpha */
void TableViewStyle::alternating_row_color1(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_alternating_row_color1 = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Set the alternating row color 2 */
void TableViewStyle::alternating_row_color2(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_alternating_row_color2 = col;
}

/** Set the alternating row color 2 with alpha */
void TableViewStyle::alternating_row_color2(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_alternating_row_color2 = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Set the scroll direction */
void TableViewStyle::scroll_direction(ScrollDirection dir)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_scroll_direction = dir;
}
