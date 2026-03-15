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
	it[StyleNames::BORDER_RADIUS]      = m_radius;
	it[StyleNames::BORDER_THICKNESS]   = m_thickness;
	it[StyleNames::BORDER_COLOR] = m_border_color;
	it[StyleNames::BORDER_FOCUS_COLOR]  = m_focus_color;
	it[StyleNames::BORDER_FOCUS_GAP]   = m_focus_gap;
	it[StyleNames::BORDER_FOCUS_THICKNESS] = (int)m_focus_thickness;
}

/** Unserialize the content of widget from json */
void BorderStyle::unserialize(JsonIterator & it)
{
	berialdraw::unserialize(StyleNames::BORDER_RADIUS, it, m_radius);
	berialdraw::unserialize(StyleNames::BORDER_THICKNESS, it, m_thickness);
	berialdraw::unserialize(StyleNames::BORDER_FOCUS_GAP, it, m_focus_gap);
	m_border_color = (int)(it[StyleNames::BORDER_COLOR]  | (int)m_border_color);
	m_focus_color  = (int)(it[StyleNames::BORDER_FOCUS_COLOR]  | (int)m_focus_color);
	m_focus_thickness    = (int)it[StyleNames::BORDER_FOCUS_THICKNESS] | m_focus_thickness;
}

/** Apply selective style properties from StyleItem (only modifies defined properties) */
void BorderStyle::apply_style(StyleItem* item)
{
	if (!item) return;
	
	JsonIterator it = item->properties();
	this->unserialize(it);
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

/** Set the focus gap between the widget and border in pixels */
void BorderStyle::focus_gap(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_focus_gap = v << 6;
}

/** Set the focus thickness in pixels (16 pixels max)*/
void BorderStyle::focus_thickness(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_focus_thickness = v;
}
