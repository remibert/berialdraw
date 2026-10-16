#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Destructor */
Glyph::~Glyph()
{
	if (m_bitmap_glyph)
	{
		FT_Done_Glyph(m_bitmap_glyph);
	}
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
		switch(character)
		{
		case '\t':
		case '\n':
		case '\v':
		case '\f':
		case '\r':
			character = ' ';
			break;
		}
		FT_UInt glyph_index;
		
		glyph_index = FT_Get_Char_Index(m_font->m_font->m_fontface->face(), character);
		if (glyph_index == 0)
		{
			character = '?';
			glyph_index = FT_Get_Char_Index(m_font->m_font->m_fontface->face(), character);
		}
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
			else
			{
				result = false;
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
		Dim scale = UIManager::renderer()->scale_q6();

		FT_Glyph bitmap_glyph = 0;
		if (!FT_Glyph_Copy(m_glyph, &bitmap_glyph))
		{
			if (scale != (1<<6))
			{
				FT_Matrix matrix;
				matrix.xx = (FT_Fixed)scale << 10;
				matrix.xy = 0;
				matrix.yx = 0;
				matrix.yy = (FT_Fixed)scale << 10;
				FT_Glyph_Transform(bitmap_glyph, &matrix, 0);
			}

			// Convert the scaled copy so the original outline remains in logical coordinates
			if (!FT_Glyph_To_Bitmap(&bitmap_glyph, FT_RENDER_MODE_NORMAL, 0, 1))
			{
				m_bitmap_glyph = bitmap_glyph;
				m_bitmap = &(((FT_BitmapGlyph)m_bitmap_glyph)->bitmap);
			}
			else if (bitmap_glyph)
			{
				FT_Done_Glyph(bitmap_glyph);
			}
		}
	}

	if (m_bitmap)
	{
		FT_Vector      vec;
		FT_Matrix      matrix = vector_matrix((angle << 10));
		FT_BitmapGlyph bitmap = (FT_BitmapGlyph)m_bitmap_glyph;
		int32_t        x_left;
		int32_t        y_top;

		// Compute the position of glyph
		Dim scale = UIManager::renderer()->scale_q6();
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

		xx = position.x_q6()+x_left;
		yy = position.y_q6()-y_top;

		// Rotation of the text line according to the center of rotation
		if (center.x_q6() != 0 || center.y_q6() != 0)
		{
			vec.x = -center.x_q6();
			vec.y = -center.y_q6();
			FT_Vector_Transform(&vec, &matrix);
			xx += vec.x;
			yy += vec.y;
		}

		//Set the origin of text in top left corner
		vec.x = margin.left_q6();
		vec.y = margin.top_q6() + m_font->baseline_q6();
		FT_Vector_Transform(&vec, &matrix);
		m_baselinex = vec.x;
		m_baseliney = vec.y;

		// If export svg required
		if (UIManager::exporter())
		{
			if (UIManager::exporter()->is_text_vectorized())
			{
				if (m_glyph && m_glyph->format == FT_GLYPH_FORMAT_OUTLINE)
				{
					FT_OutlineGlyph outline_glyph = (FT_OutlineGlyph)m_glyph;
					Outline out(outline_glyph->outline);
					FT_Outline & outline = out.get();
					FT_Matrix mirror;
					mirror.xx = 1L << 16;
					mirror.xy = 0;
					mirror.yx = 0;
					mirror.yy = -(1L << 16);
					FT_Outline_Transform(&outline, &mirror);

					FT_Vector path_position;
					path_position.x = position.x_q6() + m_baselinex;
					path_position.y = position.y_q6() + m_baseliney;
					if (center.x_q6() != 0 || center.y_q6() != 0)
					{
						vec.x = -center.x_q6();
						vec.y = -center.y_q6();
						FT_Vector_Transform(&vec, &matrix);
						path_position.x += vec.x;
						path_position.y += vec.y;
					}
					FT_Outline_Translate(&outline, path_position.x, path_position.y);
					UIManager::exporter()->add_path(out, color);
				}
			}
		}

		// Draw glyph
		UIManager::renderer()->draw_buffer((xx+m_baselinex), (yy+m_baseliney), m_bitmap->buffer, m_bitmap->width, m_bitmap->rows, color);
	}

	// Move position to next character
	position.x_q6(position.x_q6() + m_advancex);
	position.y_q6(position.y_q6() + m_advancey);
}


