#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
CellsStyle::CellsStyle()
{
}

/** Destructor */
CellsStyle::~CellsStyle()
{
	m_cells.clear();
}

/** Serialize the content of widget into json */
void CellsStyle::serialize(JsonIterator & it)
{
	JsonIterator cells_iterator = it[StyleNames::CELLSYLES_CELLS];
	for (uint32_t i = 0; i < m_cells.size(); i++)
	{
		m_cells[i].serialize(cells_iterator);
		cells_iterator.next();
	}
}

/** Unserialize the content of widget from json */
void CellsStyle::unserialize(JsonIterator & it)
{
	m_cells.clear();
	JsonIterator cells_iterator(it[StyleNames::CELLSYLES_CELLS]);
	for (cells_iterator.first(); cells_iterator.exist(); cells_iterator.next())
	{
		CellStyle cell;
		cell.unserialize(cells_iterator);
		m_cells.push_back(cell);
	}
}

/** Set properties with another */
void CellsStyle::set(const CellsStyle & other)
{
	if (this != &other)
	{
		m_cells.clear();
		for (uint32_t i = 0; i < other.m_cells.size(); i++)
		{
			m_cells.push_back(other.m_cells[i]);
		}
		UIManager::invalidator()->dirty(this, Invalidator::ALL);
	}
}

CellsStyle& CellsStyle::operator=(const CellsStyle& other)
{
	set(other);
	return *this;
}

Style * CellsStyle::create()
{
	return new CellsStyle;
}

