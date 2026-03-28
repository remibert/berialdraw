#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
CellStyle::CellStyle()
{
	m_grid_visible = 1;
}


/** Serialize the content of widget into json */
void CellStyle::serialize(JsonIterator & it)
{
	it[StyleNames::CELLSTYLE_ROW_SELECTOR] = m_row_selector;
	it[StyleNames::CELLSTYLE_COLUMN_SELECTOR] = m_column_selector;
	it[StyleNames::CELLSTYLE_GRID_COLOR] = m_grid_color;
	it[StyleNames::CELLSTYLE_GRID_VISIBLE] = m_grid_visible;
	it[StyleNames::CELLSTYLE_HORIZONTAL_THICKNESS] = m_horizontal_thickness;
	it[StyleNames::CELLSTYLE_VERTICAL_THICKNESS] = m_vertical_thickness;
}

/** Unserialize the content of widget from json */
void CellStyle::unserialize(JsonIterator & it)
{
	m_row_selector = (uint16_t)(it[StyleNames::CELLSTYLE_ROW_SELECTOR] | (int)m_row_selector);
	m_column_selector = (uint16_t)(it[StyleNames::CELLSTYLE_COLUMN_SELECTOR] | (int)m_column_selector);
	m_grid_color = (int)(it[StyleNames::CELLSTYLE_GRID_COLOR] | (int)m_grid_color);
	m_grid_visible = (bool)(it[StyleNames::CELLSTYLE_GRID_VISIBLE] | (int)m_grid_visible);
	berialdraw::unserialize(StyleNames::CELLSTYLE_HORIZONTAL_THICKNESS, it, m_horizontal_thickness);
	berialdraw::unserialize(StyleNames::CELLSTYLE_VERTICAL_THICKNESS, it, m_vertical_thickness);
}

/** Set properties with another */
void CellStyle::set(const CellStyle & other)
{
	if (this != &other)
	{
		m_row_selector = other.m_row_selector;
		m_column_selector = other.m_column_selector;
		m_grid_color = other.m_grid_color;
		m_grid_visible = other.m_grid_visible;
		m_horizontal_thickness = other.m_horizontal_thickness;
		m_vertical_thickness = other.m_vertical_thickness;
		UIManager::invalidator()->dirty(this, Invalidator::ALL);
	}
}


CellStyle& CellStyle::operator=(const CellStyle& other)
{
	set(other);
	return *this;
}

Style * CellStyle::create()
{
	return new CellStyle;
}

/** Set the row selector */
void CellStyle::row_selector(uint16_t row)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_row_selector = row;
}

/** Set the column selector */
void CellStyle::column_selector(uint16_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_column_selector = col;
}

/** Set the grid color */
void CellStyle::grid_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_grid_color = col;
}

/** Set the grid color with alpha */
void CellStyle::grid_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_grid_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Set the grid visibility state */
void CellStyle::grid_visible(bool visible)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_grid_visible = visible;
}

/** Set the horizontal grid thickness in pixels */
void CellStyle::horizontal_thickness(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_horizontal_thickness = v << 6;
}

/** Set the horizontal grid thickness with a precision of 64th of a pixel */
void CellStyle::horizontal_thickness_(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_horizontal_thickness = v;
}

/** Set the vertical grid thickness in pixels */
void CellStyle::vertical_thickness(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_vertical_thickness = v << 6;
}

/** Set the vertical grid thickness with a precision of 64th of a pixel */
void CellStyle::vertical_thickness_(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_vertical_thickness = v;
}
