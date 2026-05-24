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
	it[StyleNames::PICTURE_FIT_MODE]  = (int)m_fit_mode;
}

/** Unserialize the content of style from json */
void PictureStyle::unserialize(JsonIterator & it)
{
	String new_filename = it[StyleNames::PICTURE_FILENAME] | m_filename;
	if (new_filename != m_filename)
	{
		m_filename = new_filename;
		m_image_modified = true;
	}
	m_alpha    = (uint8_t)((int)(it[StyleNames::PICTURE_ALPHA]    | (int)m_alpha));
	m_fit_mode = (ImageFitMode)(int)(it[StyleNames::PICTURE_FIT_MODE] | (int)m_fit_mode);
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
