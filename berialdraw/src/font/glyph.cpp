#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Destructor */
Glyph::~Glyph()
{
	FT_Done_Glyph(m_glyph);
}

/** Constructor */
Glyph::Glyph(Font * font) : m_font(font)
{
}

/** Load character */
bool Glyph::load(wchar_t character, Coord angle)
{
	bool result = false;

	// If character defined
	if (character)
	{
		FT_UInt glyph_index= FT_Get_Char_Index(m_font->m_font->m_fontface->face(), character);
		if (glyph_index != 0)
		{
			// Load glyph
			if (!FT_Load_Glyph(m_font->m_font->m_fontface->face(), glyph_index, FT_LOAD_DEFAULT))
			{
				m_character = character;
				m_slot = m_font->m_font->m_fontface->face()->glyph;

				// Get glyph informations
				if (!FT_Get_Glyph(m_font->m_font->m_fontface->face()->glyph, &m_glyph))
				{
					// Rotate glyph according to angle
					FT_Matrix matrix = vector_matrix(- (angle << 10));
				
					m_width = m_glyph->advance.x >> 10;

					// Rotate glyph according to angle
					FT_Glyph_Transform(m_glyph, &matrix, 0);

					// Get glyph advance
					m_advancex =  (m_glyph->advance.x >> 10);
					m_advancey = -(m_glyph->advance.y >> 10);
					m_angle = angle;

					result = true;
				}
			}
		}
	}
	return result;
}

void Glyph::paint(Point & position, const Point & center, const Margin & margin, Coord angle, uint32_t color)
{
	int32_t xx = 0;
	int32_t yy = 0;

	// If glyph bitmap not yet loaded
	if (m_bitmap == 0)
	{
		Dim scale = UIManager::renderer()->scale_();

		if (scale != (1<<6))
		{
			FT_Matrix matrix;
			matrix.xx = (FT_Fixed)scale << 10;
			matrix.xy = 0;
			matrix.yx = 0;
			matrix.yy = (FT_Fixed)scale << 10;
			FT_Glyph_Transform(m_glyph, &matrix, 0);
		}

		//FT_Outline_Embolden(&glyph,-64);
		// Create bitmap with destruction of outline and glyph
		if (!FT_Glyph_To_Bitmap(&m_glyph, FT_RENDER_MODE_NORMAL, 0, 1))
		{
			m_bitmap = &(((FT_BitmapGlyph)m_glyph)->bitmap);
		}
	}

	if (m_bitmap)
	{
		FT_Vector      vec;
		FT_Matrix      matrix = vector_matrix((angle << 10));
		FT_BitmapGlyph bitmap = (FT_BitmapGlyph)m_glyph;
		int32_t        x_left;
		int32_t        y_top;

		// Compute the position of glyph
		Dim scale = UIManager::renderer()->scale_();
		if (scale != 1<<6)
		{
			x_left = (int32_t)((int64_t)(bitmap->left<<12)/(int64_t)scale);
			y_top  = (int32_t)((int64_t)(bitmap->top <<12)/(int64_t)scale);
		}
		else
		{
			x_left = (bitmap->left<<6);;
			y_top  = (bitmap->top<<6);
		}

		xx = position.x_()+x_left;
		yy = position.y_()-y_top;

		// Rotation of the text line according to the center of rotation
		if (center.x_() != 0 || center.y_() != 0)
		{
			vec.x = -center.x_();
			vec.y = -center.y_();
			FT_Vector_Transform(&vec, &matrix);
			xx += vec.x;
			yy += vec.y;
		}

		//Set the origin of text in top left corner
		vec.x = margin.left_();
		vec.y = margin.top_() + m_font->baseline_();
		FT_Vector_Transform(&vec, &matrix);
		m_baselinex = vec.x;
		m_baseliney = vec.y;

		// Draw glyph
		UIManager::renderer()->draw_buffer((xx+m_baselinex), (yy+m_baseliney), m_bitmap->buffer, m_bitmap->width, m_bitmap->rows, color);
	}

	// Move position to next character
	position.x_(position.x_() + m_advancex);
	position.y_(position.y_() + m_advancey);
}
