#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor that opens a font file.
	@param fontfilename The filename of the font to open. Default is an empty string. */
FontFace::FontFace(const String & fontfilename)
{
	m_filename = fontfilename;

	if (FT_New_Face(_Font::load_ft_lib(), m_filename, 0, &m_face) == 0)
	{
		m_familly = m_face->family_name;
	}
}

/** Destructor that destroys the font instance */
FontFace::~FontFace()
{
	FT_Done_Face(m_face);
	m_face = 0;
}

/** Get the family of the font
@return The family name of the font */
const String & FontFace::familly() const
{
	return m_familly;
}

/** Get the style of the font
@return The style of the font */
enum Font::Style FontFace::style()
{
	if(m_face)
	{
		return (enum Font::Style)m_face->style_flags;
	}
	return Font::UNDEFINED; 
}

/** Get the filename of the font
@return The filename of the font */
const String & FontFace::filename()
{
	return m_filename;
}

/** Get font face true type */
FT_Face FontFace::face()
{
	return m_face;
}

/** Indicates is this font is monospaced 
@return true if monospaced else return false */
bool FontFace::is_monospaced() const
{	bool result = false;
	if (m_face)
	{
		if (FT_IS_FIXED_WIDTH(m_face))
		{
			result = true;
		}
	}
	return result;
}