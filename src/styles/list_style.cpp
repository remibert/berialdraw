#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
ListStyle::ListStyle()
{
}

/** Serialize the content of style into json */
void ListStyle::serialize(JsonIterator & it)
{
	it["separator-color"]       = (int)m_separator_color;
	it["separator-thickness"]   = (int)separator_thickness();
	it["show-arrow"]            = m_show_arrow;
	it["arrow-right"]           = m_arrow_right;
	it["disabled-text-color"]   = (int)m_disabled_text_color;
}

/** Unserialize the content of style from json */
void ListStyle::unserialize(JsonIterator & it)
{
	m_separator_color     = (int)(it["separator-color"] | (int)m_separator_color);
	m_disabled_text_color = (int)(it["disabled-text-color"] | (int)m_disabled_text_color);
	
	int thickness = (int)(it["separator-thickness"] | (int)separator_thickness());
	m_separator_thickness = thickness << 6;
	
	m_show_arrow  = (bool)(it["show-arrow"] | m_show_arrow);
	m_arrow_right = (bool)(it["arrow-right"] | m_arrow_right);
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
	m_disabled_text_color = other.m_disabled_text_color;
	m_separator_thickness = other.m_separator_thickness;
	m_show_arrow          = other.m_show_arrow;
	m_arrow_right         = other.m_arrow_right;
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

/** Set whether to show arrow indicator */
void ListStyle::show_arrow(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_show_arrow = v;
}

/** Set the arrow direction */
void ListStyle::arrow_right(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_arrow_right = v;
}

/** Get the disabled text color */
uint32_t ListStyle::disabled_text_color() const
{
	return UIManager::colors()->color(m_disabled_text_color, false);
}

/** Set the disabled text color */
void ListStyle::disabled_text_color(uint32_t c)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_disabled_text_color = c;
}
