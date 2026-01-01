#include "berialdraw_imp.hpp"

using namespace berialdraw;

FT_Library _Font::m_library = 0;
FTC_Manager _Font::m_manager = 0;

/** Return the handle on free type library */
FT_Library _Font::load_ft_lib()
{
	if(m_library == 0)
	{
		FT_Init_FreeType(&m_library);
	}
	return  m_library;
}

void _Font::unload_ft_lib()
{
	if (m_library)
	{
		FT_Done_FreeType(m_library);
	}
	m_library = 0;
}

/** Constructor with freetype font face */
Font::Font(FontFacePtr & font_face, const Size & sz)
{
	m_font = new _Font(font_face, this);
	size(sz);
}

void Font::clear()
{
	if (m_font)
	{
		m_font->m_glyphs.clear();
	}
}

Font::~Font()
{
	if (m_font)
	{
		m_font->m_glyphs.clear();

		if (m_font->m_ft_size)
		{
			FT_Done_Size(m_font->m_ft_size);
			m_font->m_ft_size = 0;
		}
		delete m_font;
	}
}

/** Get the selected font size
@return The pixel size of the font */
const Size & Font::pixel_size() const
{
	return m_font->m_pixel_size;
}

/** Get the selected font size
@return The real size of the font */
const Size & Font::real_size() const
{
	return m_font->m_real_size;
}

/** Get the familly */
const String & Font::familly() const
{
	return m_font->m_fontface->familly();
}

/** Get the style */
enum Font::Style Font::style()
{
	if(m_font->m_fontface.get() && m_font->m_fontface->face())
	{
		return (enum Font::Style)m_font->m_fontface->style();
	}
	return Font::UNDEFINED; 
}

/** Get the filename */
const String & Font::filename()
{
	return m_font->m_fontface->filename();
}

/** Set font size in pixels */
void Font::size(const Size & size)
{
	if (m_font->m_fontface.get() && m_font->m_fontface->face())
	{
		// If the size has changed or font not loaded
		if(m_font->m_pixel_size.height_() != size.height_() || m_font->m_pixel_size.width_() != size.width_() || m_font->m_ft_size == 0)
		{
			// If size allocated
			if (m_font->m_ft_size)
			{
				// Clean up size allocated
				FT_Done_Size(m_font->m_ft_size);
				m_font->m_ft_size = 0;
			}

			// New font size
			FT_New_Size(m_font->m_fontface->face(), &m_font->m_ft_size);
			FT_Activate_Size(m_font->m_ft_size);
			FT_Set_Pixel_Sizes(m_font->m_fontface->face(), size.width(), size.height());

			// Extract font informations
			m_font->m_pixel_size = size;
			m_font->m_real_size = size;
			m_font->m_real_size.height_(abs(m_font->m_fontface->face()->size->metrics.ascender) + abs(m_font->m_fontface->face()->size->metrics.descender));
			m_font->m_baseline = m_font->m_fontface->face()->size->metrics.ascender;
			m_font->m_glyphs.clear();
		}
	}
}

Coord Font::baseline() const
{
	return m_font->m_baseline>>6 ;
}

Coord Font::baseline_() const
{
	return m_font->m_baseline;
}


/** Indicates is this font is monospaced 
@return true if monospaced else return false */
bool Font::is_monospaced() const
{
	return m_font->m_fontface->is_monospaced();
}

void Font::draw(const String & text, const Point & position, const Point & center, const Margin & margin, Coord angle, uint32_t color)
{
	uint32_t char_count = text.count();
	if (m_font->m_fontface.get() && m_font->m_fontface->face())
	{
		Glyph * glyph;
		Point mv(position);
		Point ct(center);

		Exporter * exporter = UIManager::exporter();

		// If export svg required
		if (exporter)
		{
			Size size;
			if (m_font->m_pixel_size.width_() != m_font->m_pixel_size.height_())
			{
				size = text_size(text);
			}
			exporter->add_text(position, size, center, margin, angle, text, *this, color);
		}

		uint32_t i;
		FT_Activate_Size(m_font->m_ft_size);
		for(i = 0; i < char_count; i++)
		{
			// Get the glyph
			glyph = m_font->m_glyphs.load(text.get(i), angle);
			if(glyph)
			{
				// Draw glyph
				glyph->paint(mv, ct, margin, angle, color);
			}
		}
	}
}

Size Font::text_size(const String & text)
{
	Size result;
	if (m_font->m_fontface.get())
	{
		uint32_t char_count = text.count();
		if (char_count)
		{
			Glyph* glyph = 0;
			result = m_font->m_real_size;
			result.width(0);
			FT_Activate_Size(m_font->m_ft_size);
			for(uint32_t i = 0; i < char_count; i++)
			{
				wchar_t character = text.get(i);

				glyph = m_font->m_glyphs.load(character,0);
				if (glyph)
				{
					result.width_ (result.width_() + glyph->m_advancex);
					result.height_(result.height_()+ glyph->m_advancey);
				}
			}
		}
	}
	return result;
}

/** Get the size of the char when drawn at a specific angle */
Size Font::char_size(wchar_t character)
{
	Size result;
	if (m_font->m_fontface.get())
	{
		if (character)
		{
			Glyph* glyph = 0;
			result = m_font->m_real_size;
			result.width(0);
			FT_Activate_Size(m_font->m_ft_size);
			glyph = m_font->m_glyphs.load(character, 0);
			if (glyph)
			{
				result.width_ (result.width_() + glyph->m_advancex);
				result.height_(result.height_()+ glyph->m_advancey);
			}
		}
	}
	return result;
}


#ifdef _DEBUG
void Font::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		Window window;
			window.color(Color::WHITE);
			window.size(480,480);

		Canvas * canvas = new Canvas(&window);
			canvas->size(440,440);
			canvas->position(20,20);
			canvas->color(Color::MINT_CREAM);

		for (uint32_t angle = 0; angle < 360; angle += 30)
		{
			Text * label = new Text(canvas);
				label->color(Color::CADET_BLUE, 128);

				// Set position of text
				label->position(220,220);

				// Change the rotation center to the left side
				label->center(-100,30);

				label->angle(angle);
				label->text("|rémi %3d|",angle);
				label->font_size(20,60);
		}

		UIManager::desktop()->dispatch("${tests}/out/font_1.svg");
	}
}
#endif
