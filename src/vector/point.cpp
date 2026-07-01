#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
Point::Point()
{
	m_x = 0;
	m_y = 0;
	m_x_undefined = 1;
	m_y_undefined = 1;
}

Point::Point(const Size & s)
{
	set_q6((Coord)s.width_q6(), (Coord)s.height_q6());
}

Point::Point(Coord x, Coord y, bool pixel)
{
	if(pixel)
	{
		set(x,y);
	}
	else
	{
		set_q6(x,y);
	}
}

Point::Point(const Point& p) : 
	m_x(p.m_x), 
	m_y(p.m_y), 
	m_x_undefined(p.m_x_undefined),
	m_y_undefined(p.m_y_undefined)
{
}

void Point::move(const Point & p)
{
	move_q6(p.x_q6(),p.y_q6());
}

void Point::diff(const Point & position)
{
	m_x_undefined = 0;
	m_y_undefined = 0;
	m_x = position.m_x - m_x;
	m_y = position.m_y - m_y;
}

void Point::serialize(const char * name, JsonIterator & it) const
{
	it[name]["x_"] = m_x;
	it[name]["y_"] = m_y;
}

void Point::unserialize(const char * name, JsonIterator & it)
{
	JsonIterator field = it[name];
	Dim x = m_x;
	m_x_undefined = berialdraw::unserialize("x_",field,x);
	m_x = x;

	Dim y = m_y;
	m_y_undefined = berialdraw::unserialize("y_",field,y);
	m_y = y;
}


/** Print content */
void Point::print(const char * name) const
{
	bd_printf("%s x=%s y=%s", name, (const char*)format_coord(m_x), (const char*)format_coord(m_y));
}

/** Adapt point to the UIManager scale */
void Point::adapt_scale()
{
	Dim scale = UIManager::renderer()->scale_q6();
	if (scale != 1<<6 && scale != 0)
	{
		m_x = (Coord)((((int64_t)m_x)<<6) / (int64_t)scale);
		m_y = (Coord)((((int64_t)m_y)<<6) / (int64_t)scale);
	}
}


