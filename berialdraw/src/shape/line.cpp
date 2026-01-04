#include "berialdraw_imp.hpp"

using namespace berialdraw;

Line::Line(Canvas * canvas) : 
	Shape(canvas, sizeof(Line)),
	m_polygon(0)
{
	m_thickness = 1<<6;
	m_color = Color::SHAPE_COLOR;
}

/** Copy contructor */
Line::Line(const Line & other) :
	Shape(other),
	LineStyle(other),
	RoundStyle(other),
	m_polygon(0)
{
}


void Line::compute_thickness(Coord &dx, Coord &dy)
{
	get_thickness(m_point1.x_(), m_point1.y_(), m_point2.x_(), m_point2.y_(), m_thickness, dx, dy);
}

void Line::get_thickness(Coord x1, Coord y1, Coord x2, Coord y2, Dim thickness, Coord & dx, Coord & dy)
{
	dx = 0;
	dy = 0;
	if (x1 == x2 && y1 == y2)
	{
		dx = (thickness >> 1);
		dy = (thickness >> 1);
	}
	else if (x1 == x2)
	{
		dx = (thickness >> 1);
	}
	else if (y1 == y2)
	{
		dy = (thickness >> 1);
	}
	else
	{
		uint64_t width  = abs(x2 - x1);
		uint64_t height = abs(y2 - y1);
		uint64_t hypotenuse = sqr((width * width + height * height));
		dx = (Coord)(((height * thickness) / hypotenuse) >> 1);
		dy = (Coord)(((width  * thickness) / hypotenuse) >> 1);

		if (x2 < x1)
		{
			dx = -dx;
		}

		if (y2 < y1)
		{
			dy = -dy;
		}
	}
}

void Line::paint(const Point & shift)
{
	if (UIManager::invalidator()->is_dirty(m_canvas) && m_thickness > 0)
	{
		Coord dx = 0;
		Coord dy = 0;
		compute_thickness(dx,dy);
		m_polygon.clear();
	
		m_polygon.add_point_(m_point1.x_() - dx, m_point1.y_() + dy);
		m_polygon.add_point_(m_point1.x_() + dx, m_point1.y_() - dy);
		m_polygon.add_point_(m_point2.x_() + dx, m_point2.y_() - dy);
		m_polygon.add_point_(m_point2.x_() - dx, m_point2.y_() + dy);
		m_polygon.next_contour();
	}
	UIManager::renderer()->draw(*this, shift);
}

#ifdef _DEBUG
void Line::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		Window window;
			window.position(0,0);
			window.size(480,480);
			window.color(Color::WHITE);

		Canvas * canvas = new Canvas(&window);
			canvas->position(0,0);
			canvas->size(window.size());
			canvas->color(Color::YELLOW,32);

		Compass compass;
			compass.center(240,240);
			compass.radius(220);

		Line * line;

		for (int32_t angle = 0; angle < 180; angle += 6)
		{
			line = new Line(canvas);
				line->color(Color::RED,32);
				line->thickness(20);

			compass.angle(angle);
			line->point1(compass.position());
			compass.angle(angle+180);
			line->point2(compass.position());
		}

		UIManager::desktop()->dispatch("$(ui.tests)/out/line1.svg");
	}
}
#endif
