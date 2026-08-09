#include "berialdraw_imp.hpp"

using namespace berialdraw;

Marker::Marker(Canvas * canvas):
	Shape(canvas, sizeof(Marker)),
	m_polygon(0)
{
	m_thickness = 1<<6;
	m_color = Color::SHAPE_COLOR;
	m_radius = 5<<6;
}

/** Copy contructor */
Marker::Marker(const Marker & other) :
	Shape(other),
	RoundStyle(other),
	m_polygon(0)
{
}


void Marker::paint(const Point & shift, const char * filled, const char * un_filled)
{
	if (UIManager::invalidator()->is_dirty(m_canvas))
	{
		VectorsScript vectors_script(m_polygon);
		if(m_thickness > 0)
		{
			vectors_script.select(un_filled);
		}
		else
		{
			vectors_script.select(filled);
		}
		vectors_script.set('t', m_thickness > (m_radius>>1) ? (m_radius>>1) : m_thickness);
		vectors_script.set('r', m_radius);
		vectors_script.set('x', m_center.x_q6());
		vectors_script.set('y', m_center.y_q6());
		vectors_script.parse();
	}
	UIManager::renderer()->draw(*this, shift);
}

/** Get the marged size of the shape
@return Size of the shape with margin */
Size Marker::marged_size()
{
	Size result;
	result.increase_q6(m_radius, m_radius);
	result.increase_q6(m_radius, m_radius);
	result.increase_q6(m_thickness, m_thickness);
	return result;
}


