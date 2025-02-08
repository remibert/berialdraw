#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Create area with position and size */
Area::Area(const Point & position, const Size & size):
	m_position(position),
	m_size(size)
{
}

/** Create area with coordinates */
Area::Area(Coord x, Coord y, Dim w, Dim h, bool pixel) :
	m_position(x,y,pixel),
	m_size(w,h,pixel)
{
}

/** Destructor */
Area::~Area()
{
}

/** Create a copy of area */
Area::Area(const Area & other)
{
	m_size     = other.m_size;
	m_position = other.m_position;
}

/** Decrease size and position of area with margin */
void Area::decrease(const Margin & margin)
{
	m_position.move_(margin.left_(), margin.top_());
	m_size.decrease_(margin.left_() + margin.right_(), margin.top_() + margin.bottom_());
}

/** Increase size and position of area with margin */
void Area::increase(const Margin & margin)
{
	m_position.move_(0-margin.left_(), 0-margin.top_());
	m_size.increase_(margin.left_() + margin.right_(), margin.top_() + margin.bottom_());
}

/** Set the content of area with other area */
Area& Area::operator=(const Area& other)
{
	if (this != &other)
	{
		this->m_position = other.m_position;
		this->m_size     = other.m_size;
	}
	return *this;
}


/** Return position of area according to the shift */
const Point & Area::position() const
{
	return m_position;
}

/** Return size of area */
const Size  & Area::size() const
{
	return m_size;
}

void Area::set(const Point & position, const Size & size)
{
	m_position = position;
	m_size = size;
}

void Area::width_(Dim width)
{
	m_size.width_(width);
}

void Area::height_(Dim height)
{
	m_size.height_(height);
}

void Area::width(Dim w)
{
	m_size.width(w);
}

void Area::height(Dim h)
{
	m_size.height(h);
}

void Area::x_(Coord x)
{
	m_position.x_(x);
}

void Area::y_(Coord y)
{
	m_position.y_(y);
}

Dim   Area::width_() const
{
	return m_size.width_();
}

Dim   Area::height_() const
{
	return m_size.height_();
}

Coord Area::x_() const
{
	return m_position.x_();
}

Coord Area::y_() const
{
	return m_position.y_();
}


/** Move the point by specified coordinates with a precision of 64th of a pixel 
@param shift shift modification */
void Area::move(const Point & shift)
{
	m_position.move(shift);
}

/** Set the x coordinate of the area in pixels
@param x New x coordinate of the area */
void Area::x(Coord x)
{
	m_position.x(x);
}

/** Set the y coordinate of the area in pixels
@param y New y coordinate of the area */
void Area::y(Coord y)
{
	m_position.y(y);
}

/** Get the width of the area in pixels
@return Width of the area */
Dim Area::width() const
{
	return m_size.width();
}

/** Get the height of the area in pixels
@return Height of the area */
Dim Area::height() const
{
	return m_size.height();
}

/** Get the x coordinate of the area in pixels
@return X coordinate of the area */
Coord Area::x() const
{
	return m_position.x();
}

/** Get the y coordinate of the area in pixels
@return Y coordinate of the area */
Coord Area::y() const
{
	return m_position.y();
}


/** Resizes and move itself on the nearest pixel */
void Area::nearest_pixel()
{
	m_position.nearest_pixel();
	m_size.nearest_pixel();
}

/** Return position of area according to the shift
@return Position of the area */
Point& Area::position()
{
	return m_position;
}

/** Return size of area
@return Size of the area */
Size& Area::size()
{
	return m_size;
}

/**	Set the size of the area
@param size New size of the area */
void Area::size(const Size& size)
{
	m_size = size;
}

/**	Set the position of the area
@param position New position of the area */
void Area::position(const Point& position)
{
	m_position = position;
}

/** Print content */
void Area::print(const char * name) const
{
	bd_printf("%s",name);
	m_position.print("");
	m_size.print("");
	bd_printf("\n");
}

