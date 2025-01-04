#include "berialdraw_imp.hpp"

using namespace berialdraw;


void Glyphs::clear()
{
	for (uint32_t i=0; i < m_glyphs.size(); i++)
	{
		delete m_glyphs[i];
		m_glyphs[i] = 0;
	}
	m_glyphs.clear();
}

Glyphs::Glyphs(Font * font)
{
	m_font = font;
}

Glyphs::~Glyphs()
{
	clear();
}

Glyph* Glyphs::search(wchar_t character, Coord angle)
{
	Glyph* result = 0;

	angle %= 23040;
	
	for (uint32_t i=0; i < m_glyphs.size(); i++)
	{
		Glyph * glyph = m_glyphs[i];
		if (glyph)
		{
			if(glyph->m_character == character && glyph->m_angle == angle)
			{
				result = glyph;
				break;
			}
		}
	}
	return result;
}

Glyph* Glyphs::load(wchar_t character, Coord angle)
{
	// Search glyph
	Glyph* result = search(character, angle%23040);
	if (result == 0)
	{
		// If the glyph loaded is too high
		if (m_glyphs.size() > 128)
		{
			delete m_glyphs[0];

			// Remove the older glyph
			m_glyphs.remove(0);
		}

		// Glyph not found
		result = new Glyph(m_font);
		if(result)
		{
			// Load glyph
			if(result->load(character, angle%23040) == false)
			{
				// If load failed destroy glyph
				delete result;
				result = 0;
			}
			else
			{
				// Add glyph into the list
				m_glyphs.push_back(result);
			}
		}
	}
	return result;
}
