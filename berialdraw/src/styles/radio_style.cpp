#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
RadioStyle::RadioStyle()
{
}

/** Serialize the content of widget into json */
void RadioStyle::serialize(JsonIterator & it)
{
	m_radio_size.serialize("radio-size", it);
	it["radio-padding"]         = m_radio_padding >> 6;
	it["radio-color"]           = m_radio_color;
	it["radio-sketch"]          = m_radio_sketch.c_str();
	it["group"]                 = m_group.c_str();
}

/** Unserialize the content of widget from json */
void RadioStyle::unserialize(JsonIterator & it)
{
	m_radio_size.unserialize("radio-size", it);
	m_radio_padding = (int)(it["radio-padding"] | (int)(m_radio_padding >> 6)) << 6;
	m_radio_color = (int)(it["radio-color"] | (int)m_radio_color);
	m_radio_sketch = it["radio-sketch"] | m_radio_sketch.c_str();
	m_group = it["group"] | m_group.c_str();
}

/** Copy operator */
RadioStyle& RadioStyle::operator=(const RadioStyle& other)
{
	set(other);
	return *this;
}


/** Set properties with another */
void RadioStyle::set(const RadioStyle & other)
{
	if (this != &other)
	{
		m_radio_size        = other.m_radio_size;
		m_radio_padding         = other.m_radio_padding;
		m_radio_color           = other.m_radio_color;
		m_radio_sketch          = other.m_radio_sketch;
		m_group				= other.m_group;
	}
}

/** Create new radio style */
Style * RadioStyle::create()
{
	return new RadioStyle();
}

// ===== Radio box properties =====

/** Get the radio box size */
const Size & RadioStyle::radio_size() const
{
	return m_radio_size;
}

/** Set the radio box size */
void RadioStyle::radio_size(const Size & s)
{
	m_radio_size = s;
}

/** Set the radio box size with width and height in pixels */
void RadioStyle::radio_size(Dim w, Dim h)
{
	radio_size_(w << 6, h << 6);
}

/** Set the radio box size with a precision of 64th of a pixel */
void RadioStyle::radio_size_(Dim w, Dim h)
{
	m_radio_size.set_(w, h);
}

/** Get the radio padding in pixels */
Dim RadioStyle::radio_padding() const
{
	return m_radio_padding >> 6;
}

/** Set the radio padding in pixels */
void RadioStyle::radio_padding(Dim pad)
{
	m_radio_padding = pad << 6;
}


// ===== Radio indicator properties =====

/** Get the radio indicator color */
uint32_t RadioStyle::radio_color() const
{
	return m_radio_color;
}

/** Set the radio indicator color */
void RadioStyle::radio_color(uint32_t col)
{
	m_radio_color = col;
}


/** Get the radio indicator sketch (VectorScript) */
const String & RadioStyle::radio_sketch() const
{
	return m_radio_sketch;
}

/** Set the radio indicator sketch (VectorScript) */
void RadioStyle::radio_sketch(const String & sketch)
{
	m_radio_sketch = sketch;
}

/** Get the radio group name */
const String & RadioStyle::group() const
{
	return m_group;
}

/** Set the radio group name */
void RadioStyle::group(const String & g)
{
	m_group = g;
}

