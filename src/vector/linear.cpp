#include "berialdraw_imp.hpp"

using namespace berialdraw;

Linear::Linear()
{
	a = 0;
	b = 0;
	vertical = false;
}

Linear::Linear(const Point& p1, const Point& p2)
{
	set_coef(p1.x_(), p1.y_(), p2.x_(), p2.y_());
}

bool Linear::set_coef(Coord  x1, Coord  y1, Coord  x2, Coord  y2)
{
	if (x1 != x2)
	{
		vertical = false;
		a = (float)(y2 - y1) / (float)(x2 - x1);
		b = (float)y1 - a * (float)x1;
	}
	else
	{
		a = 0;
		b = (float)x1;
		vertical = true;
	}
	return vertical;
}

Linear::Linear(Coord  x1, Coord  y1, Coord  x2, Coord  y2)
{
	set_coef(x1, y1, x2, y2);
}

bool Linear::get_intersection(Linear& line, Point& point)
{
	bool result = true;

	if (line.vertical && vertical == false)
	{
		point.x_((int32_t)(line.b));
		point.y_((int32_t)(a * point.x_() + b));
	}
	else if (line.vertical == false && vertical)
	{
		point.x_((int32_t)b);
		point.y_((int32_t)(line.a * point.x_() + line.b));
	}
	else if (line.vertical == true && vertical)
	{
		result = false;
	}
	else if ((a - line.a) != 0.)
	{
		point.x_((int32_t)((line.b - b) / (a - line.a)));
		point.y_((int32_t)(a * point.x_() + b));
	}
	else
	{
		result = false;
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
	return norms(p1.x_(), p1.y_(), p2.x_(), p2.y_());
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
	return cross_product(p1.x_(), p1.y_(), p2.x_(), p2.y_(), p3.x_(), p3.y_());
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
	return dot_product(p1.x_(), p1.y_(), p2.x_(), p2.y_(), p3.x_(), p3.y_());
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
	add_vector(p1.x_(), p1.y_(), p2.x_(), p2.y_(), p3.x_(), p3.y_(), x4, y4);
	p4.set_(x4,y4);
}

Coord Linear::fx(Coord  x)
{
	return (int32_t)(a*x + b);
}

Coord Linear::fy(Coord  y)
{
	return (int32_t)((y -b)/a);
}

bool Linear::is_in_segment(const Point& p,const Point& p1,const Point& p2)
{
	return is_in_segment(p.x_(), p.y_(), p1.x_(), p1.y_(), p2.x_(), p2.y_());
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


#ifdef _DEBUG
void Linear::test1()
{
	Point p1(0, 0);
	Point p2(100, 100);

	Point p3(100, 0);
	Point p4(0, 100);

	Linear l1(p1, p2);
	Linear l2(p3, p4);

	Point intersect1(0, 0);
	Point intersect2(0, 0);

	l1.get_intersection(l2, intersect1);
	l2.get_intersection(l1, intersect2);
}

void Linear::test2()
{
	Point p1(50, 0);
	Point p2(50, 100);

	Point p3(100, 0);
	Point p4(0, 100);

	Linear l1(p1, p2);
	Linear l2(p3, p4);

	Point intersect1(0, 0);
	Point intersect2(0, 0);

	l1.get_intersection(l2, intersect1);
	l2.get_intersection(l1, intersect2);
}
#endif
