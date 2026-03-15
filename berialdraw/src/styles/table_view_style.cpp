#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
TableViewStyle::TableViewStyle()
{
	m_header_visible = 1;
	m_grid_visible = 1;
	m_table_view_modified = 1;
}


/** Serialize the content of widget into json */
void TableViewStyle::serialize(JsonIterator & it)
{
	it[StyleNames::TABLEVIEW_GRID_COLOR] = m_grid_color;
	it[StyleNames::TABLEVIEW_HORIZONTAL_THICKNESS] = m_horizontal_thickness;
	it[StyleNames::TABLEVIEW_VERTICAL_THICKNESS] = m_vertical_thickness;
	it[StyleNames::TABLEVIEW_GRID_VISIBLE] = m_grid_visible;
	it[StyleNames::TABLEVIEW_HEADER_VISIBLE] = m_header_visible;
	it[StyleNames::TABLEVIEW_HEADER_BACKGROUND_COLOR] = m_header_background_color;
	it[StyleNames::TABLEVIEW_ALTERNATING_ROW_COLOR_1] = m_alternating_row_color1;
	it[StyleNames::TABLEVIEW_ALTERNATING_ROW_COLOR_2] = m_alternating_row_color2;
	berialdraw::serialize(StyleNames::TABLEVIEW_SCROLL_DIRECTION, it, m_scroll_direction);
}

/** Unserialize the content of widget from json */
void TableViewStyle::unserialize(JsonIterator & it)
{
	m_grid_color = (int)(it[StyleNames::TABLEVIEW_GRID_COLOR] | (int)m_grid_color);
	berialdraw::unserialize(StyleNames::TABLEVIEW_HORIZONTAL_THICKNESS, it, m_horizontal_thickness);
	berialdraw::unserialize(StyleNames::TABLEVIEW_VERTICAL_THICKNESS, it, m_vertical_thickness);
	m_grid_visible = (bool)(it[StyleNames::TABLEVIEW_GRID_VISIBLE] | (int)m_grid_visible);
	m_header_visible = (bool)(it[StyleNames::TABLEVIEW_HEADER_VISIBLE] | (int)m_header_visible);
	m_header_background_color = (int)(it[StyleNames::TABLEVIEW_HEADER_BACKGROUND_COLOR] | (int)m_header_background_color);
	m_alternating_row_color1 = (int)(it[StyleNames::TABLEVIEW_ALTERNATING_ROW_COLOR_1] | (int)m_alternating_row_color1);
	m_alternating_row_color2 = (int)(it[StyleNames::TABLEVIEW_ALTERNATING_ROW_COLOR_2] | (int)m_alternating_row_color2);
	berialdraw::unserialize(StyleNames::TABLEVIEW_SCROLL_DIRECTION, it, m_scroll_direction);
}

/** Apply selective style properties from StyleItem (only modifies defined properties) */
void TableViewStyle::apply_style(StyleItem* item)
{
	if (!item) return;
	
	JsonIterator it = item->properties();
	this->unserialize(it);
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
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_horizontal_thickness = v << 6;
	m_table_view_modified = 1;
}

/** Set the horizontal grid thickness with a precision of 64th of a pixel */
void TableViewStyle::horizontal_thickness_(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_horizontal_thickness = v;
	m_table_view_modified = 1;
}

/** Set the vertical grid thickness in pixels */
void TableViewStyle::vertical_thickness(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_vertical_thickness = v << 6;
	m_table_view_modified = 1;
}

/** Set the vertical grid thickness with a precision of 64th of a pixel */
void TableViewStyle::vertical_thickness_(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_vertical_thickness = v;
	m_table_view_modified = 1;
}

/** Set the grid visibility state */
void TableViewStyle::grid_visible(bool visible)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_grid_visible = visible;
	m_table_view_modified = 1;
}

/** Set the header visibility state */
void TableViewStyle::header_visible(bool visible)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_header_visible = visible;
	m_table_view_modified = 1;
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
	m_table_view_modified = 1;
}
