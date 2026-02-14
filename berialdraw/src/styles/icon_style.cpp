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
	it["filename"] = m_filename;
	it["icon-color"]        = m_icon_color;
	int zoom  = m_zoom  != Size::MAX_SIZE && m_zoom  != Size::MAX_SIZE ? m_zoom  : m_zoom;
	it["zoom_"]  = zoom;
	berialdraw::unserialize("zoom_", it, m_zoom);

	m_icon_size.serialize("icon-size", it);
	m_icon_padding.serialize ("icon-padding",it);
}

/** Unserialize the content of widget from json */
void IconStyle::unserialize(JsonIterator & it)
{
	m_filename        = it["filename"]        | m_filename;
	m_icon_color       = (int)(it["icon-color"]        | (int)m_icon_color);
	Dim zoom  = it["zoom_"]   | Size::MAX_SIZE;

	m_icon_size.unserialize("icon-size", it);
	m_icon_padding.unserialize ("icon-padding",it);
	m_zoom    = (zoom == Size::MAX_SIZE) ? m_zoom  : zoom;
}

/** Set the zoom ratio for the icon
@param z zoom value */
void IconStyle::zoom(Dim z)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_zoom = z <<6;
}

/** Set the zoom ratio for the icon
@param z zoom value shifted by 6 bits */
void IconStyle::zoom_(Dim z)
{
	m_icon_modified = true;
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_zoom = z;
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
void IconStyle::icon_padding_(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_icon_padding.set_(top,left,bottom,right);
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

/** Create new paths */
Style * IconStyle::create()
{
	return new IconStyle;
}
