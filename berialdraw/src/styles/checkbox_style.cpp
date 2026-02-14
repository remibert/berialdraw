#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
CheckboxStyle::CheckboxStyle()
{
}

/** Serialize the content of widget into json */
void CheckboxStyle::serialize(JsonIterator & it)
{
	m_checkbox_size.serialize("checkbox-size", it);
	it["check-padding"]         = m_check_padding >> 6;
	it["check-color"]           = m_check_color;
	it["check-sketch"]          = m_check_sketch.c_str();
}

/** Unserialize the content of widget from json */
void CheckboxStyle::unserialize(JsonIterator & it)
{
	m_checkbox_size.unserialize("checkbox-size", it);
	m_check_padding = (int)(it["check-padding"] | (int)(m_check_padding >> 6)) << 6;
	m_check_color = (int)(it["check-color"] | (int)m_check_color);
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
		m_checkbox_size        = other.m_checkbox_size;
		m_check_padding         = other.m_check_padding;
		m_check_color           = other.m_check_color;
		m_check_sketch          = other.m_check_sketch;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

Style * CheckboxStyle::create()
{
	return new CheckboxStyle;
}

/** Set the checkbox box size */
void CheckboxStyle::checkbox_size(const Size & s)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_checkbox_size = s;
}

/** Set the checkbox box size with width and height in pixels */
void CheckboxStyle::checkbox_size(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_checkbox_size.set(w, h);
}

/** Set the checkbox box size with a precision of 64th of a pixel */
void CheckboxStyle::checkbox_size_(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_checkbox_size.set_(w, h);
}

/** Set the checkbox padding in pixels */
void CheckboxStyle::check_padding(Dim pad)
{
	m_check_padding = pad << 6;
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

/** Set the check mark sketch (VectorScript) */
void CheckboxStyle::check_sketch(const String & sketch)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_check_sketch = sketch;
}
