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

Point& Point::operator=(const Point& other)
{
	if (this != &other)
	{
		this->m_x = other.m_x;
		this->m_y = other.m_y;
		this->m_x_undefined = other.m_x_undefined;
		this->m_y_undefined = other.m_y_undefined;
	}
	return *this;
}

void Point::set(Coord x_, Coord y_)
{
	x(x_);
	y(y_);
}

void Point::set_(Coord x, Coord y)
{
	m_x_undefined = 0;
	m_y_undefined = 0;
	m_x = x;
	m_y = y;
}

Coord Point::y() const
{
	return (m_y+32) >> 6;
}

Coord Point::x() const
{
	return (m_x+32) >> 6;
}

Coord Point::y_() const
{
	return m_y;
}

Coord Point::x_() const
{
	return m_x;
}

void Point::y(Coord y_)
{
	m_y_undefined = 0;
	m_y = y_ << 6;
}

void Point::x(Coord x_)
{
	m_x_undefined = 0;
	m_x = x_ << 6;
}

void Point::y_(Coord y)
{
	m_y_undefined = 0;
	m_y = y;
}

void Point::x_(Coord x)
{
	m_x_undefined = 0;
	m_x = x;
}

void Point::move(const Point & p)
{
	move(p.x(),p.y());
}

void Point::move(Coord x_,Coord y_)
{
	m_x_undefined = 0;
	m_y_undefined = 0;
	m_x += x_ << 6;
	m_y += y_ << 6;
}

void Point::move_(Coord x_,Coord y_)
{
	m_x_undefined = 0;
	m_y_undefined = 0;
	m_x += x_;
	m_y += y_;
}

bool Point::operator==(const Point& other)  const
{
	return m_x == other.m_x && m_y == other.m_y;
}

bool Point::operator!=(const Point& other)  const
{
	return m_x != other.m_x || m_y != other.m_y;
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

/** Positions itself on the nearest pixel */
void Point::nearest_pixel()
{
	m_x_undefined = 0;
	m_y_undefined = 0;
	m_x = ((m_x + 32) >> 6) << 6;
	m_y = ((m_y + 32) >> 6) << 6;
}

/** Indicates if x is not defined */
bool Point::is_x_undefined() const
{
	return m_x_undefined;
}

/** Indicates if y is not defined */
bool Point::is_y_undefined() const
{
	return m_y_undefined;
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
