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

/** Resizes and move itself on the nearest pixel */
void Area::nearest_pixel()
{
	m_position.nearest_pixel();
	m_size.nearest_pixel();
}

/** Decrease size and position of area with margin */
void Area::decrease(const Margin& margin)
{
	m_position.move_(margin.left_(), margin.top_());
	m_size.decrease_(margin.left_() + margin.right_(), margin.top_() + margin.bottom_());
}

/** Increase size and position of area with margin */
void Area::increase(const Margin& margin)
{
	m_position.move_(0-margin.left_(), 0-margin.top_());
	m_size.increase_(margin.left_() + margin.right_(), margin.top_() + margin.bottom_());
}

/** Print content */
void Area::print(const char * name) const
{
	bd_printf("%s",name);
	m_position.print("");
	m_size.print("");
	bd_printf("\n");
}

