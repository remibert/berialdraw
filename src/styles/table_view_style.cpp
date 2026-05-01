#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
TableViewStyle::TableViewStyle()
{
}


/** Serialize the content of widget into json */
void TableViewStyle::serialize(JsonIterator & it)
{
	it[StyleNames::TABLEVIEW_ALTERNATING_ROW_COLOR_1] = m_alternating_row_color1;
	it[StyleNames::TABLEVIEW_ALTERNATING_ROW_COLOR_2] = m_alternating_row_color2;
}

/** Unserialize the content of widget from json */
void TableViewStyle::unserialize(JsonIterator & it)
{
	m_alternating_row_color1 = (int)(it[StyleNames::TABLEVIEW_ALTERNATING_ROW_COLOR_1] | (int)m_alternating_row_color1);
	m_alternating_row_color2 = (int)(it[StyleNames::TABLEVIEW_ALTERNATING_ROW_COLOR_2] | (int)m_alternating_row_color2);
}

/** Set properties with another */
void TableViewStyle::set(const TableViewStyle & other)
{
	if (this != &other)
	{
		m_alternating_row_color1 = other.m_alternating_row_color1;
		m_alternating_row_color2 = other.m_alternating_row_color2;
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