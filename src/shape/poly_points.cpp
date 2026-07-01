#include "berialdraw_imp.hpp"

using namespace berialdraw;

PolyPoints::PolyPoints() :
	m_first_point(0),
	m_last_point(0),
	m_current(0),
	m_count(0)
{
}

PolyPoints::PolyPoints(const PolyPoints & other):
	m_first_point(0),
	m_last_point(0),
	m_current(0),
	m_count(0)
{
	PolyPoint* current = other.m_first_point;
	while (current != 0)
	{
		append_q6(current->x_q6(), current->y_q6());
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
		m_current = m_first_point;
		p.x_q6(m_current->x_q6());
		p.y_q6(m_current->y_q6());
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
		p.x_q6(m_current->x_q6());
		p.y_q6(m_current->y_q6());
		return true;
	}
	return false;
}

void PolyPoints::prepend(const Point & p)
{
	PolyPoint* point = new PolyPoint(p);
	PolyPoint* current = m_first_point;

	if (current)
	{
		point->next = current;
		m_first_point = point;
	}
	else
	{
		m_first_point = point;
		m_last_point = point;
	}
	m_count++;
}

void PolyPoints::prepend(Coord x, Coord y)
{
	prepend(Point(x,y,true));
}
void PolyPoints::prepend_q6(Coord x, Coord y)
{
	prepend(Point(x,y,false));
}

void PolyPoints::append(const Point& p)
{
	PolyPoint* point = new PolyPoint(p);
	PolyPoint* current = m_last_point;
	if (current)
	{
		current->next = point;
		m_last_point = point;
	}
	else
	{
		m_first_point = point;
		m_last_point = point;
	}
	m_count++;
}

void PolyPoints::append(Coord x, Coord y)
{
	append(Point(x,y,true));
}

void PolyPoints::append_q6(Coord x, Coord y)
{
	append(Point(x,y,false));
}

void PolyPoints::clear()
{
	PolyPoint* current = m_first_point;
	PolyPoint* destroy;
	while (current != 0)
	{
		destroy = current;
		current = current->next;
		delete destroy;
	}

	m_first_point  = 0;
	m_last_point = 0;
	m_current = 0;
	m_count   = 0;
}

uint32_t PolyPoints::count()
{
	return m_count;
}




