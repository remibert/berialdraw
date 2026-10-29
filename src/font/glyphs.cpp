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
	for (uint32_t i = 0; i < m_vectors.size(); i++)
	{
		delete m_vectors[i];
		m_vectors[i] = 0;
	}
	m_vectors.clear();
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

// Binary search for a vector glyph by character
uint32_t Glyphs::lower_bound_vector(wchar_t character)
{
	uint32_t low = 0;
	uint32_t high = m_vectors.size();
	while (low < high)
	{
		uint32_t mid = (low + high) / 2;
		if (m_vectors[mid]->m_character < character)
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
	if (result == 0)
	{
		uint32_t vector_index = lower_bound_vector(character);
		if (vector_index < m_vectors.size() && m_vectors[vector_index]->m_character == character)
		{
			result = m_vectors[vector_index];
		}
	}
	return result;
}

Glyph* Glyphs::load(wchar_t character, Coord angle)
{
	if (character < 0x20)
	{
		character = 0x20;
	}
	angle %= 23040;

	// Search glyph
	Glyph* result = search(character, angle);
	if (result == 0)
	{
		FontGlyph * font_glyph = new FontGlyph(m_font);
		if (font_glyph && font_glyph->load(character, angle))
		{
			if (m_glyphs.size() > 128)
			{
				uint32_t oldest = 0;
				for (uint32_t i = 1; i < m_glyphs.size(); i++)
				{
					if ((int32_t)(m_glyphs[i]->m_stamp - m_glyphs[oldest]->m_stamp) < 0)
					{
						oldest = i;
					}
				}
				delete m_glyphs[oldest];
				m_glyphs.remove(oldest);
			}
			font_glyph->m_stamp = m_stamp++;
			m_glyphs.insert(lower_bound(font_glyph->m_character, font_glyph->m_angle), font_glyph);
			result = font_glyph;
		}
		else
		{
			delete font_glyph;
		}
	}
	if (result == 0)
	{
		Fonts * fonts = UIManager::fonts();
		VectorImage * image = fonts ? fonts->copy_vector_glyph(character) : 0;
		if (image)
		{
			VectorGlyph * vector_glyph = new VectorGlyph(m_font, character, image);
			if (vector_glyph && vector_glyph->valid())
			{
				if (m_vectors.size() > 128)
				{
					uint32_t oldest = 0;
					for (uint32_t i = 1; i < m_vectors.size(); i++)
					{
						if ((int32_t)(m_vectors[i]->m_stamp - m_vectors[oldest]->m_stamp) < 0)
						{
							oldest = i;
						}
					}
					delete m_vectors[oldest];
					m_vectors.remove(oldest);
				}
				vector_glyph->m_stamp = m_stamp++;
				m_vectors.insert(lower_bound_vector(character), vector_glyph);
				result = vector_glyph;
			}
			else
			{
				delete vector_glyph;
				if (vector_glyph == 0)
				{
					delete image;
				}
			}
		}
	}
	if (result == 0)
	{
		FontGlyph * fallback_glyph = new FontGlyph(m_font);
		if (fallback_glyph && fallback_glyph->load('?', angle))
		{
			fallback_glyph->m_character = character;
			if (m_glyphs.size() > 128)
			{
				uint32_t oldest = 0;
				for (uint32_t i = 1; i < m_glyphs.size(); i++)
				{
					if ((int32_t)(m_glyphs[i]->m_stamp - m_glyphs[oldest]->m_stamp) < 0)
					{
						oldest = i;
					}
				}
				delete m_glyphs[oldest];
				m_glyphs.remove(oldest);
			}
			fallback_glyph->m_stamp = m_stamp++;
			m_glyphs.insert(lower_bound(character, fallback_glyph->m_angle), fallback_glyph);
			result = fallback_glyph;
		}
		else
		{
			delete fallback_glyph;
		}
	}
	return result;
}
