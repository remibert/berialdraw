#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
BorderStyle::BorderStyle()
{
	m_radius = 0;
	m_thickness = 0;
	m_focus_gap = 0;
	m_focus_thickness = 0;
}


/** Serialize the content of widget into json */
void BorderStyle::serialize(JsonIterator & it)
{
	it["radius_"]      = m_radius;
	it["thickness_"]   = m_thickness;
	it["border-color"] = m_border_color;
	it["focus-color"]  = m_focus_color;
	it["focus-gap_"]   = m_focus_gap;
	it["focus-thickness"] = (int)m_focus_thickness;
}

/** Unserialize the content of widget from json */
void BorderStyle::unserialize(JsonIterator & it)
{
	berialdraw::unserialize("radius_"   , it, m_radius);
	berialdraw::unserialize("thickness_", it, m_thickness);
	berialdraw::unserialize("focus-gap_", it, m_focus_gap);
	m_border_color = (int)(it["border-color"]  | (int)m_border_color);
	m_focus_color  = (int)(it["focus-color"]  | (int)m_focus_color);
	m_focus_thickness    = (int)it["focus-thickness"] | m_focus_thickness;
}

/** Set properties with another */
void BorderStyle::set(const BorderStyle & other)
{
	if (this != &other)
	{
		m_radius          = other.m_radius;
		m_border_color    = other.m_border_color;
		m_focus_color     = other.m_focus_color;
		m_thickness       = other.m_thickness;
		m_focus_gap       = other.m_focus_gap;
		m_focus_thickness = other.m_focus_thickness;
		UIManager::invalidator()->dirty(this, Invalidator::ALL);
	}
}


BorderStyle& BorderStyle::operator=(const BorderStyle& other)
{
	set(other);
	return *this;
}

Style * BorderStyle::create()
{
	return new BorderStyle;
}

/** Get the radius */
Dim BorderStyle::radius() const
{
	return m_radius >> 6;
}

/** Get the radius with a precision of 64th of a pixel */
Dim BorderStyle::radius_() const
{
	return m_radius;
}

/** Set the radius in pixels */
void BorderStyle::radius(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_radius =  v << 6;
}

/** Set the radius with a precision of 64th of a pixel */
void BorderStyle::radius_(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_radius = v;
}

/** Get the thickness */
Dim BorderStyle::thickness() const
{
	return m_thickness >> 6;
}

/** Get the thickness with a precision of 64th of a pixel */
Dim BorderStyle::thickness_() const
{
	return m_thickness;
}

/** Set the thickness in pixels */
void BorderStyle::thickness(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_thickness = v << 6;
}

/** Set the thickness with a precision of 64th of a pixel */
void BorderStyle::thickness_(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_thickness = v;
}

/** Get the border color */
uint32_t BorderStyle::border_color() const
{
	return m_border_color; 
}

/** Set the border color */
void BorderStyle::border_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_border_color = col;
}

/** Set the border color with alpha */
void BorderStyle::border_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_border_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}


/** Get the focus color */
uint32_t BorderStyle::focus_color() const
{
	return m_focus_color;
}

/** Set the focus color */
void BorderStyle::focus_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_focus_color = col;
}

/** Set the focus color with alpha */
void BorderStyle::focus_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_focus_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Get the focus_gap */
Dim BorderStyle::focus_gap() const
{
	return m_focus_gap >> 6;
}

/** Set the focus_gap in pixels */
void BorderStyle::focus_gap(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_focus_gap = v << 6;
}

/** Get the focus thickness (16 pixels max)*/
Dim BorderStyle::focus_thickness() const
{
	return m_focus_thickness;
}

/** Set the focus thickness in pixels (16 pixels max)*/
void BorderStyle::focus_thickness(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_focus_thickness = v;
}
