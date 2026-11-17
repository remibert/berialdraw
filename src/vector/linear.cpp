#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Initialize an empty line
Linear::Linear()
{
	m_x1 = 0;
	m_y1 = 0;
	m_dx = 0;
	m_dy = 0;
}

Linear::Linear(const Point& p1, const Point& p2)
{
	set_coef(p1.x_q6(), p1.y_q6(), p2.x_q6(), p2.y_q6());
}

// Set the line using two points
bool Linear::set_coef(Coord  x1, Coord  y1, Coord  x2, Coord  y2)
{
	m_x1 = x1;
	m_y1 = y1;
	m_dx = (int64_t)x2 - (int64_t)x1;
	m_dy = (int64_t)y2 - (int64_t)y1;
	return m_dx == 0;
}

Linear::Linear(Coord  x1, Coord  y1, Coord  x2, Coord  y2)
{
	set_coef(x1, y1, x2, y2);
}

// Get the intersection point of two lines
bool Linear::get_intersection(Linear& line, Point& point)
{
	bool result = false;
	int64_t x = 0;
	int64_t y = 0;

	if (m_dx == 0 && line.m_dx != 0)
	{
		x = m_x1;
		y = (line.m_y1 * line.m_dx + line.m_dy * (x - line.m_x1)) / line.m_dx;
		result = true;
	}
	else if (m_dx != 0 && line.m_dx == 0)
	{
		x = line.m_x1;
		y = (m_y1 * m_dx + m_dy * (x - m_x1)) / m_dx;
		result = true;
	}
	else if (m_dx != 0 && line.m_dx != 0)
	{
		int64_t denominator = m_dx * line.m_dy - m_dy * line.m_dx;
		if (denominator != 0)
		{
			int64_t delta_x = line.m_x1 - m_x1;
			int64_t delta_y = line.m_y1 - m_y1;
			int64_t numerator = delta_x * line.m_dy - delta_y * line.m_dx;

			x = (m_x1 * denominator + m_dx * numerator) / denominator;
			y = (m_y1 * denominator + m_dy * numerator) / denominator;
			result = true;
		}
	}

	if (result)
	{
		point.x_q6((Coord)x);
		point.y_q6((Coord)y);
	}
	return result;
}

int64_t Linear::norms(Coord  x1, Coord  y1, Coord  x2, Coord  y2)
{
	int64_t ux = x2 - x1;
	int64_t uy = y2 - y1;
	int64_t result = sqr(ux*ux + uy*uy);
	return result;
}

int64_t Linear::norms(const Point& p1, const Point& p2)
{
	return norms(p1.x_q6(), p1.y_q6(), p2.x_q6(), p2.y_q6());
}

int64_t Linear::cross_product(Coord  x1, Coord  y1, Coord  x2, Coord  y2, Coord  x3, Coord  y3)
{
	int64_t ux = x2 - x1;
	int64_t uy = y2 - y1;
	int64_t vx = y3 - y1;
	int64_t vy = x3 - x1;
	int64_t result = ux * vx - uy * vy;
	return result;
}

int64_t Linear::cross_product(const Point & p1, const Point & p2, const Point & p3)
{
	return cross_product(p1.x_q6(), p1.y_q6(), p2.x_q6(), p2.y_q6(), p3.x_q6(), p3.y_q6());
}

int64_t  Linear::dot_product(Coord  x1, Coord  y1, Coord  x2, Coord  y2, Coord  x3, Coord  y3)
{
	int64_t ux = x2 - x1;
	int64_t uy = y2 - y1;
	int64_t vx = x2 - x3;
	int64_t vy = y2 - y3;
	int64_t result = ux * vx + uy * vy;
	return result;
}

int64_t Linear::dot_product(const Point& p1, const Point& p2, const Point& p3)
{
	return dot_product(p1.x_q6(), p1.y_q6(), p2.x_q6(), p2.y_q6(), p3.x_q6(), p3.y_q6());
}

void Linear::add_vector(Coord  x1, Coord  y1, Coord  x2, Coord  y2, Coord  x3, Coord  y3, Coord & x4, Coord & y4)
{
	x4 = x3 + (x2 - x1);
	y4 = y3 + (y2 - y1);
}

void Linear::add_vector(const Point& p1, const Point& p2, const Point& p3, Point & p4)
{
	Coord x4;
	Coord y4;
	add_vector(p1.x_q6(), p1.y_q6(), p2.x_q6(), p2.y_q6(), p3.x_q6(), p3.y_q6(), x4, y4);
	p4.set_q6(x4,y4);
}

// Get the y coordinate for a given x coordinate
Coord Linear::fx(Coord  x)
{
	Coord result = (Coord)m_x1;
	if (m_dx != 0)
	{
		int64_t numerator = m_y1 * m_dx + m_dy * ((int64_t)x - m_x1);
		result = (Coord)(numerator / m_dx);
	}
	return result;
}

// Get the x coordinate for a given y coordinate
Coord Linear::fy(Coord  y)
{
	Coord result = (Coord)m_x1;
	if (m_dy != 0)
	{
		int64_t numerator = m_x1 * m_dy + m_dx * ((int64_t)y - m_y1);
		result = (Coord)(numerator / m_dy);
	}
	return result;
}

bool Linear::is_in_segment(const Point& p,const Point& p1,const Point& p2)
{
	return is_in_segment(p.x_q6(), p.y_q6(), p1.x_q6(), p1.y_q6(), p2.x_q6(), p2.y_q6());
}

bool Linear::is_in_segment(Coord  x, Coord  y, Coord  x1, Coord  y1, Coord  x2, Coord  y2)
{
	bool in_x = false;
	bool in_y = false;
	if(x1 < x2)
	{
		if(x >= x1 && x <= x2)
		{
			in_x = true;
		}
	}
	else
	{
		if(x >= x2 && x <= x1)
		{
			in_x = true;
		}
	}

	if(y1 < y2)
	{
		if(y >= y1 && y <= y2)
		{
			in_y = true;
		}
	}
	else
	{
		if(y >= y2 && y <= y1)
		{
			in_y = true;
		}
	}
	return in_x && in_y;
}



