#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
RadioStyle::RadioStyle()
{
}

/** Serialize the content of widget into json */
void RadioStyle::serialize(JsonIterator & it)
{
	m_radio_size.serialize(StyleNames::RADIO_SIZE, it);
	it[StyleNames::RADIO_PADDING]         = m_radio_padding >> 6;
	it[StyleNames::RADIO_COLOR]           = m_radio_color;
	it[StyleNames::RADIO_SKETCH]          = m_radio_sketch.c_str();
	it[StyleNames::RADIO_GROUP]                 = m_group.c_str();
}

/** Unserialize the content of widget from json */
void RadioStyle::unserialize(JsonIterator & it)
{
	m_radio_size.unserialize(StyleNames::RADIO_SIZE, it);
	m_radio_padding = (int)(it[StyleNames::RADIO_PADDING] | (int)(m_radio_padding >> 6)) << 6;
	m_radio_color = (int)(it[StyleNames::RADIO_COLOR] | (int)m_radio_color);
	m_radio_sketch = it[StyleNames::RADIO_SKETCH] | m_radio_sketch.c_str();
	m_group = it[StyleNames::RADIO_GROUP] | m_group.c_str();
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

/** Set the radio padding in pixels */
void RadioStyle::radio_padding(Dim pad)
{
	m_radio_padding = pad << 6;
}

/** Set the radio indicator color */
void RadioStyle::radio_color(uint32_t col)
{
	m_radio_color = col;
}

/** Set the radio indicator sketch (VectorScript) */
void RadioStyle::radio_sketch(const String & sketch)
{
	m_radio_sketch = sketch;
}

/** Set the radio group name */
void RadioStyle::group(const String & g)
{
	m_group = g;
}

