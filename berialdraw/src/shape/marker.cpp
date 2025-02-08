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
		vectors_script.set('t', m_thickness);
		vectors_script.set('r', m_radius);
		vectors_script.set('x', m_center.x_());
		vectors_script.set('y', m_center.y_());
		vectors_script.parse();
	}
	UIManager::renderer()->draw(*this, shift);
}


#ifdef _DEBUG
void Marker::test()
{
	Circle::test();
	Triangle::test();
	Square::test();
	Cross::test();
}
#endif
