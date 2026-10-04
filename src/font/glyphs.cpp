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

// Binary search of the first glyph not lower than (character, angle)
uint32_t Glyphs::lower_bound(wchar_t character, Coord angle)
{
	uint32_t low = 0;
	uint32_t high = m_glyphs.size();
	while (low < high)
	{
		uint32_t mid = (low + high) / 2;
		Glyph * glyph = m_glyphs[mid];
		if (glyph->m_character < character || (glyph->m_character == character && glyph->m_angle < angle))
		{
			low = mid + 1;
		}
		else
		{
			high = mid;
		}
	}
	return low;
}

// Search a glyph in the sorted list
Glyph* Glyphs::search(wchar_t character, Coord angle)
{
	Glyph* result = 0;

	angle %= 23040;

	// Carriage return or tab is transformed into space
	if (character < 0x20)
	{
		character = 0x20;
	}

	uint32_t index = lower_bound(character, angle);
	if (index < m_glyphs.size())
	{
		Glyph * glyph = m_glyphs[index];
		if (glyph->m_character == character && glyph->m_angle == angle)
		{
			result = glyph;
		}
	}
	return result;
}

Glyph* Glyphs::load(wchar_t character, Coord angle)
{
	// Search glyph
	Glyph* result = search(character, angle);
	if (result == 0)
	{
		// If the glyph loaded is too high
		if (m_glyphs.size() > 128)
		{
			// Find the oldest glyph (list is sorted by key, not by age)
			uint32_t oldest = 0;
			for (uint32_t i=1; i < m_glyphs.size(); i++)
			{
				if ((int32_t)(m_glyphs[i]->m_stamp - m_glyphs[oldest]->m_stamp) < 0)
				{
					oldest = i;
				}
			}
			delete m_glyphs[oldest];
			m_glyphs[oldest] = nullptr;
			m_glyphs.remove(oldest);
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
				// Insert glyph at its sorted position
				result->m_stamp = m_stamp++;
				m_glyphs.insert(lower_bound(result->m_character, result->m_angle), result);
			}
		}
	}
	return result;
}
