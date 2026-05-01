#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
GridStyle::GridStyle()
{
	m_grid_visible = 1;
}


/** Serialize the content of widget into json */
void GridStyle::serialize(JsonIterator & it)
{
	it[StyleNames::GRIDSTYLE_GRID_COLOR] = m_grid_color;
	it[StyleNames::GRIDSTYLE_GRID_VISIBLE] = m_grid_visible;
	it[StyleNames::GRIDSTYLE_HORIZONTAL_THICKNESS] = m_horizontal_thickness;
	it[StyleNames::GRIDSTYLE_VERTICAL_THICKNESS] = m_vertical_thickness;
}

/** Unserialize the content of widget from json */
void GridStyle::unserialize(JsonIterator & it)
{
	m_grid_color = (int)(it[StyleNames::GRIDSTYLE_GRID_COLOR] | (int)m_grid_color);
	m_grid_visible = (bool)(it[StyleNames::GRIDSTYLE_GRID_VISIBLE] | (int)m_grid_visible);
	berialdraw::unserialize(StyleNames::GRIDSTYLE_HORIZONTAL_THICKNESS, it, m_horizontal_thickness);
	berialdraw::unserialize(StyleNames::GRIDSTYLE_VERTICAL_THICKNESS, it, m_vertical_thickness);
}

/** Set properties with another */
void GridStyle::set(const GridStyle & other)
{
	if (this != &other)
	{
		m_grid_color = other.m_grid_color;
		m_grid_visible = other.m_grid_visible;
		m_horizontal_thickness = other.m_horizontal_thickness;
		m_vertical_thickness = other.m_vertical_thickness;
		UIManager::invalidator()->dirty(this, Invalidator::ALL);
	}
}


GridStyle& GridStyle::operator=(const GridStyle& other)
{
	set(other);
	return *this;
}

Style * GridStyle::create()
{
	return new GridStyle;
}

/** Set the grid color */
void GridStyle::grid_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_grid_color = col;
}

/** Set the grid color with alpha */
void GridStyle::grid_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_grid_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Set the grid visibility state */
void GridStyle::grid_visible(bool visible)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_grid_visible = visible;
}

/** Set the horizontal grid thickness in pixels */
void GridStyle::horizontal_thickness(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_horizontal_thickness = v << 6;
}

/** Set the horizontal grid thickness with a precision of 64th of a pixel */
void GridStyle::horizontal_thickness_(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_horizontal_thickness = v;
}

/** Set the vertical grid thickness in pixels */
void GridStyle::vertical_thickness(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_vertical_thickness = v << 6;
}

/** Set the vertical grid thickness with a precision of 64th of a pixel */
void GridStyle::vertical_thickness_(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_vertical_thickness = v;
}
