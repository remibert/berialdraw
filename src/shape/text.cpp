#include "berialdraw_imp.hpp"

using namespace berialdraw;

Text::Text(Canvas * canvas):
	Shape(canvas, sizeof(Text))
{
	m_font_modified = 1;
	m_text_modified = 1;
	m_text_color = Color::SHAPE_COLOR;
}

Text::Text(const Text & other):
	Shape(other)
{
	*((TextStyle*)this) = *(const TextStyle*)&other;
	m_text_modified = 1;
	m_font_modified = 1;
}

Text::~Text()
{
}

void Text::paint(const Point & shift)
{
	content_size();
	m_text_box.paint(shift, *m_font.get(), m_text, m_position, m_center, m_margin, m_angle, color(), 0, 0, true);
}

Size Text::content_size()
{
	if (m_font_modified || m_text_modified || m_angle_modified)
	{
		Area area;
		select_font();
		
		if (m_font.get())
		{
			m_text_box.parse(area, *m_font, m_text, UINT32_MAX, UINT32_MAX, UINT32_MAX, (Align)m_text_align);
		}
		m_text_modified = m_font_modified = m_angle_modified = 0;
		m_text_size = m_text_box.content_size();
	}
	return m_text_size;
}

