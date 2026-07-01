#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
PictureStyle::PictureStyle()
{
}

/** Destructor */
PictureStyle::~PictureStyle()
{
}

/** Serialize the content of style into json */
void PictureStyle::serialize(JsonIterator & it)
{
	it[StyleNames::PICTURE_FILENAME]  = m_filename;
	it[StyleNames::PICTURE_ALPHA]     = (int)m_alpha;
	it[StyleNames::ICON_COLOR]        = m_icon_color;
	m_icon_padding.serialize (StyleNames::ICON_PADDING,it);
	berialdraw::serialize(StyleNames::PICTURE_FIT_MODE, it, m_fit_mode);
}

/** Unserialize the content of style from json */
void PictureStyle::unserialize(JsonIterator & it)
{
	String new_filename = it[StyleNames::PICTURE_FILENAME] | m_filename;
	m_icon_color       = (int)(it[StyleNames::ICON_COLOR]        | (int)m_icon_color);
	m_icon_padding.unserialize (StyleNames::ICON_PADDING,it);
	if (new_filename != m_filename)
	{
		m_filename = new_filename;
		m_image_modified = true;
	}
	m_alpha    = (uint8_t)((int)(it[StyleNames::PICTURE_ALPHA]    | (int)m_alpha));
	berialdraw::unserialize(StyleNames::PICTURE_FIT_MODE, it, m_fit_mode);
}

/** Set filename value with string */
void PictureStyle::filename(const String & s)
{
	if (m_filename != s)
	{
		m_image_modified = true;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
		m_filename = s;
	}
}

/** Set the alpha transparency */
void PictureStyle::alpha(uint8_t a)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_alpha = a;
}

/** Set the image fit mode */
void PictureStyle::fit_mode(ImageFitMode mode)
{
	m_image_modified = true;
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_fit_mode = mode;
}

/** Create new style instance */
Style * PictureStyle::create()
{
	return new PictureStyle;
}

/** Set the icon_padding */
void PictureStyle::icon_padding(const Margin & m)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_icon_padding = m;
}

/** Set the icon_padding in pixels */
void PictureStyle::icon_padding(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_icon_padding.set(top,left,bottom,right);
}

/** Set the icon_padding with a precision of 64th of a pixel */
void PictureStyle::icon_padding_q6(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_icon_padding.set_q6(top,left,bottom,right);
}

/** Set the back icon_color */
void PictureStyle::icon_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_icon_color = col;
}

/** Set the back icon_color with alpha */
void PictureStyle::icon_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_icon_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}


