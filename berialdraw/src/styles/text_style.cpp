#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
TextStyle::TextStyle()
{
	m_text_align    = ALIGN_DEFAULT;
	m_text_modified = 1;
	m_font_modified = 1;
}

/** Serialize the content of widget into json */
void TextStyle::serialize(JsonIterator & it)
{
	it["text"] = m_text;
	it["font-familly"] = m_font_familly;
	m_font_size.serialize("font-size",it);
	m_padding.serialize ("padding",it);
	berialdraw::serialize("text-align",it, (Align)m_text_align);
	it["text-color"  ] = (int)m_text_color;
}

/** Unserialize the content of widget from json */
void TextStyle::unserialize(JsonIterator & it)
{
	Align align = (Align)m_text_align;
	berialdraw::unserialize("text-align",it, align);
	m_text_align   = align;
	m_text_color   = it["text-color"]   | m_text_color;
	m_font_familly = it["font-familly"] | m_font_familly;
	m_text        = it["text"]        | m_text;
	m_font_size.unserialize("font-size",it);
	m_padding.unserialize ("padding",it);
}

/** Copy operator */
TextStyle& TextStyle::operator=(const TextStyle& other)
{
	set(other);
	return *this;
}

/** Set properties with another */
void TextStyle::set(const TextStyle & other)
{
	if (this != &other)
	{
		m_padding     = other.m_padding;
		m_text_align   = other.m_text_align;
		m_text_color   = other.m_text_color;
		m_font_familly = other.m_font_familly;
		m_font_size    = other.m_font_size;
		m_text        = other.m_text;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
		m_text_modified = 1;
		m_font_modified = 1;
	}
}

Style * TextStyle::create()
{
	return new TextStyle;
}

/** Get the text color */
uint32_t TextStyle::text_color() const
{
	return m_text_color;
}

/** Set the text color */
void TextStyle::text_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_text_color = col;
}

/** Set the text color with alpha */
void TextStyle::text_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_text_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Get the font familly selected */
const String & TextStyle::font_familly()
{
	return m_font_familly;
}

/** Set the font familly */
void TextStyle::font_familly(const char * font_familly_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_font_modified = 1;
	m_font_familly = font_familly_;
}

/** Get the font size */
const Size & TextStyle::font_size() const
{
	return m_font_size;
}

/** Set the font size */
void TextStyle::font_size(const Size & font_size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_font_modified = 1;
	m_font_size = font_size_;
}

/** Set the font size with width and height */
void TextStyle::font_size(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_font_modified = 1;
	if(h == 0)
	{
		m_font_size.set(w,w);
	}
	else
	{
		m_font_size.set(w,h);
	}
}

/** Set the font size with a precision of 64th of a pixel */
void TextStyle::font_size_(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_font_modified = 1;
	m_font_size.set_(w,h);
}

/** Select font */
void TextStyle::select_font()
{
	if(m_font.get())
	{
		if(m_font->pixel_size() != m_font_size)
		{
			m_font_modified = 1;
		}
	}
	else
	{
		m_font_modified = 1;
	}
	if(m_font_modified)
	{
		m_font = UIManager::fonts()->select(m_font_familly,m_font_size);
		m_font_modified = 0;
	}
}


/** Get the text value */
const String & TextStyle::text()
{
	return m_text;
}

/** Set text value with printf format */
void TextStyle::text(const char * text_, ...)
{
	va_list args;
	va_start(args, text_);
	m_text_modified = 1;
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_text.vprint(text_, args);
	va_end(args);
}

/** Set text value with string */
void TextStyle::text(const String & str)
{
	m_text_modified = 1;
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_text = str;
}

/** Set text value with unicode value */
void TextStyle::text(wchar_t value)
{
	m_text_modified = 1;
	m_text = value;
}


/** Get the padding */
const Margin & TextStyle::padding() const
{
	return m_padding;
}

/** Set the padding */
void TextStyle::padding(const Margin & m)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_padding = m;
}

/** Set the padding in pixels */
void TextStyle::padding(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_padding.set(top,left,bottom,right);
}

/** Set the padding with a precision of 64th of a pixel */
void TextStyle::padding_(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_padding.set_(top,left,bottom,right);
}

/** Set the padding */
void TextStyle::padding(Dim horizontal, Dim vertical)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_padding.set(vertical,horizontal,vertical,horizontal);
}

/** Set the padding */
void TextStyle::padding(Dim value)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_padding.set(value,value,value,value);
}

/** Get the text align */
Align TextStyle::text_align() const
{
	return (Align)m_text_align;
}

/** Set the text align */
void TextStyle::text_align(Align v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_text_align = (Align)v;
}