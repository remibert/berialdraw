#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
IconStyle::IconStyle() 
{
}

/** Destructor */
IconStyle::~IconStyle()
{
}

/** Serialize the content of widget into json */
void IconStyle::serialize(JsonIterator & it)
{
	it[StyleNames::ICON_FILENAME]     = m_filename;
	it[StyleNames::ICON_COLOR]        = m_icon_color;
	it[StyleNames::ICON_TEXT_PADDING] = m_text_padding >> 6;
	m_icon_frame_size.serialize (StyleNames::ICON_FRAME_SIZE,it);
	m_icon_padding.serialize    (StyleNames::ICON_PADDING,it);
}

/** Unserialize the content of widget from json */
void IconStyle::unserialize(JsonIterator & it)
{
	String new_filename = it[StyleNames::PICTURE_FILENAME] | m_filename;
	m_icon_color       = (int)(it[StyleNames::ICON_COLOR]        | (int)m_icon_color);
	m_icon_padding.unserialize (StyleNames::ICON_PADDING,it);
	if (new_filename != m_filename)
	{
		m_filename = new_filename;
		m_icon_modified = true;
	}
	berialdraw::unserialize(StyleNames::ICON_TEXT_PADDING, it, m_text_padding);
	m_icon_frame_size.unserialize(StyleNames::ICON_FRAME_SIZE,it);
}

/** Copy operator */
IconStyle& IconStyle::operator=(const IconStyle& other)
{
	set(other);
	return *this;
}

/** Set properties with another */
void IconStyle::set(const IconStyle& other)
{
	if (this != &other)
	{
		m_icon_padding    = other.m_icon_padding;
		m_icon_color      = other.m_icon_color;
		m_icon_frame_size = other.m_icon_frame_size;
		m_filename        = other.m_filename;
		m_icon_modified   = other.m_icon_modified;
		m_text_padding    = other.m_text_padding;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

/** Set the size */
void IconStyle::icon_frame_size(const Size & size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_icon_frame_size = size_;
}

/** Set the size with width and height in pixels */
void IconStyle::icon_frame_size(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_icon_frame_size.set(w,h);
}

/** Set the size with a precision of 64th of a pixel */
void IconStyle::icon_frame_size_q6(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_icon_frame_size.set_q6(w,h);
}

/** Set filename value with string */
void IconStyle::filename(const String & s)
{
	m_icon_modified = true;
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_filename = s;
}

/** Set the icon_padding */
void IconStyle::icon_padding(const Margin & m)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_icon_padding = m;
}

/** Set the icon_padding in pixels */
void IconStyle::icon_padding(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_icon_padding.set(top,left,bottom,right);
}

/** Set the icon_padding with a precision of 64th of a pixel */
void IconStyle::icon_padding_q6(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_icon_padding.set_q6(top,left,bottom,right);
}

/** Set the back icon_color */
void IconStyle::icon_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_icon_color = col;
}

/** Set the back icon_color with alpha */
void IconStyle::icon_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_icon_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Set the text padding in pixels */
void IconStyle::text_padding(Dim pad)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_text_padding = pad << 6;
}

/** Create new paths */
Style * IconStyle::create()
{
	return new IconStyle;
}

