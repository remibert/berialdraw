#include "berialdraw_imp.hpp"

using namespace berialdraw;

Rect::Rect(Canvas * canvas) : 
	Shape(canvas, sizeof(Rect)),
	m_polygon(0)
{
	m_thickness = 0;
}

Rect::Rect(const Rect & other):
	Shape(other),
	RoundStyle(other),
	m_polygon(0)
{
}

Coord round_(Coord value)
{
	if ((value % 64) != 0)
	{
		value = ((value + 33) >> 6) << 6;
	}
	return value;
}

// Get size including margin
Size Rect::marged_size()
{
	// m_size is now the outer size, no need to add thickness
	return content_size();
}

// Render outline
void Rect::paint(const Point & shift)
{
	paint(shift,false);
}

// Render outline
void Rect::paint(const Point & shift, bool in_widget)
{
	if (m_radius == 0 && m_thickness == 0 && m_angle == 0 && 
		m_center.x_q6() == 0 && m_center.y_q6() == 0 && UIManager::exporter() == 0)
	{
		Point move(shift);
		move.move(m_position);
		UIManager::renderer()->fillrect(move.x(),move.y(), m_size.width(),m_size.height(), color());
	}
	else
	{
		Point move(shift);
		RectRenderer::create_part_outer(m_polygon, m_size, m_radius, m_thickness, m_borders);

		// Offset by half thickness so the outer edge aligns with the specified size
		UIManager::renderer()->draw(*this, move);
	}
}
