#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
ListStyle::ListStyle()
{
}

/** Serialize the content of style into json */
void ListStyle::serialize(JsonIterator & it)
{
	it[StyleNames::SEPARATOR_COLOR]       = (int)m_separator_color;
	it[StyleNames::SEPARATOR_THICKNESS]   = (int)separator_thickness();
}

/** Unserialize the content of style from json */
void ListStyle::unserialize(JsonIterator & it)
{
	m_separator_color     = (int)(it[StyleNames::SEPARATOR_COLOR] | (int)m_separator_color);
	int thickness = (int)(it[StyleNames::SEPARATOR_THICKNESS] | (int)separator_thickness());
	m_separator_thickness = thickness << 6;
	
}

/** Copy operator */
ListStyle& ListStyle::operator=(const ListStyle& other)
{
	if (this != &other)
	{
		set(other);
	}
	return *this;
}

/** Copy properties from another without invalidation */
void ListStyle::copy_from(const ListStyle & other)
{
	m_separator_color     = other.m_separator_color;
	m_separator_thickness = other.m_separator_thickness;
}

/** Set properties with another */
void ListStyle::set(const ListStyle & other)
{
	copy_from(other);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

/** Create new list style */
Style * ListStyle::create()
{
	return new ListStyle;
}

/** Get the separator color */
uint32_t ListStyle::separator_color() const
{
	return UIManager::colors()->color(m_separator_color, false);
}

/** Set the separator color */
void ListStyle::separator_color(uint32_t c)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_separator_color = c;
}

/** Set the separator thickness in pixels */
void ListStyle::separator_thickness(Dim t)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_separator_thickness = t << 6;
}

/** Set the separator thickness in 64ths of pixels */
void ListStyle::separator_thickness_(Dim t)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_separator_thickness = t;
}
