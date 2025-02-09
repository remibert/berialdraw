#include "berialdraw_imp.hpp"

using namespace berialdraw;

PolyPoints::PolyPoints() :
	m_points(0),
	m_current(0),
	m_count(0)
{
}

PolyPoints::PolyPoints(const PolyPoints & other):
	m_points(0),
	m_current(0),
	m_count(0)
{
	PolyPoint* current = other.m_points;
	while (current != 0)
	{
		append_(current->x_(), current->y_());
		current = current->next;
	}
}

PolyPoints::~PolyPoints()
{
	clear();
}

bool PolyPoints::get_first(Point& p)
{
	if (m_count > 0)
	{
		m_current = m_points;
		p.x_(m_current->x_());
		p.y_(m_current->y_());
		return true;
	}
	return false;
}

bool PolyPoints::get_next(Point& p)
{
	if (m_current)
	{
		m_current = m_current->next;
	}
	if (m_current)
	{
		p.x_(m_current->x_());
		p.y_(m_current->y_());
		return true;
	}
	return false;
}

void PolyPoints::prepend(const Point & p)
{
	PolyPoint* point = new PolyPoint(p);
	PolyPoint* current = m_points;

	if (current)
	{
		m_points = point;
		point->next = current;
	}
	else
	{
		m_points = point;
	}
	m_count++;
}

void PolyPoints::prepend(Coord x, Coord y)
{
	prepend(Point(x,y,true));
}
void PolyPoints::prepend_(Coord x, Coord y)
{
	prepend(Point(x,y,false));
}

void PolyPoints::append(const Point& p)
{
	PolyPoint* point = new PolyPoint(p);
	PolyPoint* current = m_points;
	if (current)
	{
		while (current->next != 0)
		{
			current = current->next;
		}
		current->next = point;
	}
	else
	{
		m_points = point;
	}
	m_count++;
}

void PolyPoints::append(Coord x, Coord y)
{
	append(Point(x,y,true));
}

void PolyPoints::append_(Coord x, Coord y)
{
	append(Point(x,y,false));
}

void PolyPoints::clear()
{
	PolyPoint* current = m_points;
	PolyPoint* destroy;
	while (current != 0)
	{
		destroy = current;
		current = current->next;
		delete destroy;
	}

	m_points  = 0;
	m_current = 0;
	m_count   = 0;
}

uint32_t PolyPoints::count()
{
	return m_count;
}

#ifdef _DEBUG
void PolyPoints::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
	}
}
#endif

