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
	set_((Coord)s.width_(), (Coord)s.height_());
}

Point::Point(Coord x, Coord y, bool pixel)
{
	if(pixel)
	{
		set(x,y);
	}
	else
	{
		set_(x,y);
	}
}

Point::Point(const Point& p) : 
	m_x(p.m_x), 
	m_y(p.m_y), 
	m_x_undefined(p.m_x_undefined),
	m_y_undefined(p.m_y_undefined)
{
}

Point::~Point()
{
}

void Point::move(const Point & p)
{
	move(p.x(),p.y());
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
#define FLOAT_PART(value) (abs((((((value)%64)*1000)/64)+5)/10))
	bd_printf("%s x=%d.%02d y=%d.%02d",name, m_x/64, FLOAT_PART(m_x), m_y/64, FLOAT_PART(m_y));
}

/** Adapt point to the UIManager scale */
void Point::adapt_scale()
{
	Dim scale = UIManager::renderer()->scale_();
	if (scale != 1<<6 && scale != 0)
	{
		m_x = (Coord)((((int64_t)m_x)<<6) / (int64_t)scale);
		m_y = (Coord)((((int64_t)m_y)<<6) / (int64_t)scale);
	}
}
