#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
CellStyle::CellStyle()
{
}


/** Serialize the content of widget into json */
void CellStyle::serialize(JsonIterator & it)
{
	it[StyleNames::CELLSTYLE_ROW_SELECTOR] = m_row_selector;
	it[StyleNames::CELLSTYLE_COLUMN_SELECTOR] = m_column_selector;
}

/** Unserialize the content of widget from json */
void CellStyle::unserialize(JsonIterator & it)
{
	m_row_selector = (uint16_t)(it[StyleNames::CELLSTYLE_ROW_SELECTOR] | (int)m_row_selector);
	m_column_selector = (uint16_t)(it[StyleNames::CELLSTYLE_COLUMN_SELECTOR] | (int)m_column_selector);
}

/** Set properties with another */
void CellStyle::set(const CellStyle & other)
{
	if (this != &other)
	{
		m_row_selector = other.m_row_selector;
		m_column_selector = other.m_column_selector;
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
