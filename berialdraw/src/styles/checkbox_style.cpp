#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
CheckboxStyle::CheckboxStyle()
{
}

/** Serialize the content of widget into json */
void CheckboxStyle::serialize(JsonIterator & it)
{
	m_check_box_size.serialize("check-box-size", it);
	it["check-box-thickness_"]  = m_check_box_thickness;
	it["check-box-radius_"]     = m_check_box_radius;
	it["check-box-padding_"]    = m_check_box_padding;
	it["check-color"]           = m_check_color;
	it["check-box-color"]       = m_check_box_color;
	it["check-sketch"]          = m_check_sketch.c_str();
}

/** Unserialize the content of widget from json */
void CheckboxStyle::unserialize(JsonIterator & it)
{
	m_check_box_size.unserialize("check-box-size", it);
	berialdraw::unserialize("check-box-thickness_", it, m_check_box_thickness);
	berialdraw::unserialize("check-box-radius_", it, m_check_box_radius);
	berialdraw::unserialize("check-box-padding_", it, m_check_box_padding);
	m_check_color = (int)(it["check-color"] | (int)m_check_color);
	m_check_box_color = (int)(it["check-box-color"] | (int)m_check_box_color);
	m_check_sketch = it["check-sketch"] | m_check_sketch.c_str();
}

/** Copy operator */
CheckboxStyle& CheckboxStyle::operator=(const CheckboxStyle& other)
{
	set(other);
	return *this;
}


/** Set properties with another */
void CheckboxStyle::set(const CheckboxStyle & other)
{
	if (this != &other)
	{
		m_check_box_size        = other.m_check_box_size;
		m_check_box_thickness   = other.m_check_box_thickness;
		m_check_box_radius      = other.m_check_box_radius;
		m_check_box_padding     = other.m_check_box_padding;
		m_check_color           = other.m_check_color;
		m_check_box_color       = other.m_check_box_color;
		m_check_sketch          = other.m_check_sketch;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

Style * CheckboxStyle::create()
{
	return new CheckboxStyle;
}



// Checkbox box properties

/** Get the checkbox box size */
const Size & CheckboxStyle::check_box_size() const
{
	return m_check_box_size;
}

/** Set the checkbox box size */
void CheckboxStyle::check_box_size(const Size & s)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_check_box_size = s;
}

/** Set the checkbox box size with width and height in pixels */
void CheckboxStyle::check_box_size(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_check_box_size.set(w, h);
}

/** Set the checkbox box size with a precision of 64th of a pixel */
void CheckboxStyle::check_box_size_(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_check_box_size.set_(w, h);
}


/** Get the checkbox box thickness in pixels */
Dim CheckboxStyle::check_box_thickness() const
{
	return m_check_box_thickness >> 6;
}

/** Set the checkbox box thickness in pixels */
void CheckboxStyle::check_box_thickness(Dim thickness)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_check_box_thickness = thickness << 6;
}


/** Get the checkbox box radius in pixels */
Dim CheckboxStyle::check_box_radius() const
{
	return m_check_box_radius >> 6;
}

/** Set the checkbox box radius in pixels */
void CheckboxStyle::check_box_radius(Dim radius)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_check_box_radius = radius << 6;
}


/** Get the checkbox box padding in pixels */
Dim CheckboxStyle::check_box_padding() const
{
	return m_check_box_padding >> 6;
}

/** Set the checkbox box padding in pixels */
void CheckboxStyle::check_box_padding(Dim pad)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_check_box_padding = pad << 6;
}


/** Get the check box color */
uint32_t CheckboxStyle::check_box_color() const
{
	return m_check_box_color;
}

/** Set the check box color */
void CheckboxStyle::check_box_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_check_box_color = col;
}

/** Set the check box color with alpha */
void CheckboxStyle::check_box_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_check_box_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}



// Check mark properties

/** Get the check mark color */
uint32_t CheckboxStyle::check_color() const
{
	return m_check_color;
}

/** Set the check mark color */
void CheckboxStyle::check_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_check_color = col;
}

/** Set the check mark color with alpha */
void CheckboxStyle::check_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_check_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}


/** Get the check mark sketch (VectorScript) */
const String & CheckboxStyle::check_sketch() const
{
	return m_check_sketch;
}

/** Set the check mark sketch (VectorScript) */
void CheckboxStyle::check_sketch(const String & sketch)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_check_sketch = sketch;
}
