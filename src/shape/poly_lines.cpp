#include "berialdraw_imp.hpp"

using namespace berialdraw;

PolyLines::PolyLines(Canvas * canvas):
	Shape(canvas, sizeof(PolyLines)),
	m_polygon(0),
	m_count(0)
{
}

PolyLines::PolyLines(const PolyLines & other):
	Shape(other),
	RoundStyle(other),
	m_polygon(0),
	m_count(0),
	m_shape(other.m_shape),
	m_points(other.m_points)
{
}

void PolyLines::prepend(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(m_canvas, Invalidator::REDRAW);
	Point p(x,y,true);
	m_points.prepend(p);
}

void PolyLines::prepend_(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(m_canvas, Invalidator::REDRAW);
	Point p(x,y,false);
	m_points.prepend(p);
}

void PolyLines::prepend(const Point & p)
{
	UIManager::invalidator()->dirty(m_canvas, Invalidator::REDRAW);
	m_points.prepend(p);
}

void PolyLines::append(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(m_canvas, Invalidator::REDRAW);
	Point p(x,y,true);
	m_points.append(p);
}

void PolyLines::append_(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(m_canvas, Invalidator::REDRAW);
	Point p(x,y,false);
	m_points.append(p);
}

void PolyLines::append(const Point & p)
{
	UIManager::invalidator()->dirty(m_canvas, Invalidator::REDRAW);
	m_points.append(p);
}

void PolyLines::add_right(const Point & p)
{
	m_shape.prepend(p);
}

void PolyLines::add_left(const Point & p)
{
	m_shape.append(p);
}

void PolyLines::clear()
{
	m_shape.clear();
	m_points.clear();
}

void PolyLines::get_side(const Point& p1, const Point& p2, Dim thickness, bool right, Linear& linear)
{
	Linear result;
	Coord dx;
	Coord dy;
	Line::get_thickness(p1.x_(), p1.y_(), p2.x_(), p2.y_(), thickness, dx, dy);
	int64_t angle  = Linear::cross_product(p1.x_(), p1.y_(), p2.x_(), p2.y_(), p1.x_() - dx, p1.y_() + dy);

	if ((angle < 0 && right) || (angle >= 0 && !right))
	{
		linear.set_coef(p1.x_() - dx, p1.y_() + dy, p2.x_() - dx, p2.y_() + dy);
	}
	else
	{
		linear.set_coef(p1.x_() + dx, p1.y_()- dy, p2.x_() + dx, p2.y_() - dy);
	}
}

void PolyLines::add_junction(const Point &p1, const Point &p2, const Point & p3)
{
	int64_t cross_product = Linear::cross_product(p1, p2, p3);
	
	// Parallel lines
	if (cross_product == 0 && Linear::is_in_segment(p2,p1,p3))
	{
		cross_product = cross_product;
	}
	// Crossed lines
	else
	{
		int64_t dot_product = Linear::dot_product(p1, p2, p3);
		int64_t norms1 = Linear::norms(p1,p2);
		int64_t norms2 = Linear::norms(p2,p3);
		int64_t cos = 255;

		if (norms1 != 0 && norms2 != 0)
		{
			cos = dot_product / ((norms1*norms2)>>8);
		}

		// Right angle or obtuse angle
		if (dot_product <= 0 || (cos < 200))
		{
			Point right;
			Point left;
			Linear linear1;
			Linear linear2;
			get_side(p1, p2, m_thickness, true, linear1);
			get_side(p2, p3, m_thickness, true, linear2);
			linear1.get_intersection(linear2, right);
			add_right(right);

			get_side(p1, p2, m_thickness, false, linear1);
			get_side(p2, p3, m_thickness, false, linear2);
			linear1.get_intersection(linear2, left);
			add_left(left);
		}
		// Acute angle
		else
		{
			Linear line1(p1,p2);
			Linear line2(p2,p3);
			Point right; Point left;
				
			// Add end of first line
			get_end(p1, p2,right,left);
			add_right(right); // Add right border
			add_left(left);   // Add left border

			// Add the start of second line
			get_start(p2, p3,right,left);
			add_right(right); // Add right border
			add_left(left);   // Add left border
		}
	}
}

void PolyLines::get_start(const Point& p1,const Point& p2,Point & right,Point & left)
{
	Coord dx;
	Coord dy;

	Line::get_thickness(p1.x_(), p1.y_(), p2.x_(), p2.y_(), m_thickness, dx, dy);

	int64_t angle = Linear::cross_product(p1.x_(), p1.y_(), p2.x_(), p2.y_(), p1.x_() - dx, p1.y_() + dy);

	if (angle > 0)
	{
		left.x_ (p1.x_() - dx);
		left.y_ (p1.y_() + dy);
		right.x_(p1.x_() + dx);
		right.y_(p1.y_() - dy);
	}
	else
	{
		left.x_ (p1.x_() + dx);
		left.y_ (p1.y_() - dy);
		right.x_(p1.x_() - dx);
		right.y_(p1.y_() + dy);
	}
}

void PolyLines::add_start(const Point& p1, const Point& p2)
{
	Point right;
	Point left;
	get_start(p1, p2, right, left);
	add_left(left);
	add_right(right);
	///bd_printf("start %d,%d %d,%d\n",left.x,left.y_(),right.x,right.y_());
}

void PolyLines::get_end(const Point& p1,const Point& p2,Point & right,Point & left)
{
	Coord dx;
	Coord dy;

	Line::get_thickness(p1.x_(), p1.y_(), p2.x_(), p2.y_(), m_thickness, dx, dy);

	int64_t angle = Linear::cross_product(p1.x_(), p1.y_(), p2.x_(), p2.y_(), p1.x_() - dx, p1.y_() + dy);

	if (angle > 0)
	{
		left. x_(p2.x_() - dx);
		left. y_(p2.y_() + dy);
		right.x_(p2.x_() + dx);
		right.y_(p2.y_() - dy);
	}
	else
	{
		left. x_(p2.x_() + dx);
		left. y_(p2.y_() - dy);
		right.x_(p2.x_() - dx);
		right.y_(p2.y_() + dy);
	}
}

void PolyLines::add_end(const Point& p1, const Point& p2)
{
	Point right;
	Point left;
	get_end(p1, p2, right, left);
	add_left(left);
	add_right(right);
	//bd_printf("end %d,%d %d,%d\n",left.x,left.y_(),right.x,right.y_());
}

void PolyLines::build()
{
	Point p1, p2, p3;

	m_shape.clear();
	m_polygon.clear();
	int32_t index=0;

	if (m_points.get_first(p1) && m_points.get_next(p2))
	{
		add_start(p1, p2);
		while (m_points.get_next(p3))
		{
			add_junction(p1, p2, p3);
			p1 = p2;
			p2 = p3;
		}
		add_end(p1, p2);

		m_polygon.clear();
		Point p;
		if (m_shape.get_first(p))
		{
			Point first(p);
			do
			{
				m_polygon.add_point(p);
			}
			while (m_shape.get_next(p));
			m_polygon.next_contour();
		}
	}
}

// Render outline
void PolyLines::paint(const Point & shift)
{
	if (UIManager::invalidator()->is_dirty(m_canvas))
	{
		build();
	}
	UIManager::renderer()->draw(*this, shift);
}

