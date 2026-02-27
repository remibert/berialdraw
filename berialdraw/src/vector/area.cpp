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
/** Clip this area against another area */
void Area::clip(const Area& clip_area)
{
	// Get coordinates in 64ths of a pixel for precision
	Coord this_x = m_position.x_();
	Coord this_y = m_position.y_();
	Dim this_width = m_size.width_();
	Dim this_height = m_size.height_();
	
	Coord clip_x = clip_area.m_position.x_();
	Coord clip_y = clip_area.m_position.y_();
	Dim clip_width = clip_area.m_size.width_();
	Dim clip_height = clip_area.m_size.height_();
	
	// Calculate right and bottom edges
	Coord this_right = this_x + this_width;
	Coord this_bottom = this_y + this_height;
	Coord clip_right = clip_x + clip_width;
	Coord clip_bottom = clip_y + clip_height;
	
	// Adjust left edge: if x < clip_x, move to clip_x
	if (this_x < clip_x)
	{
		Coord diff = clip_x - this_x;
		m_position.x_(clip_x);
		if (this_width > static_cast<Dim>(diff))
			m_size.width_(this_width - static_cast<Dim>(diff));
		else
			m_size.width_(0);
	}
	
	// Adjust top edge: if y < clip_y, move to clip_y
	if (this_y < clip_y)
	{
		Coord diff = clip_y - this_y;
		m_position.y_(clip_y);
		if (this_height > static_cast<Dim>(diff))
			m_size.height_(this_height - static_cast<Dim>(diff));
		else
			m_size.height_(0);
	}
	
	// Recalculate right and bottom with potentially new position
	this_x = m_position.x_();
	this_y = m_position.y_();
	this_width = m_size.width_();
	this_height = m_size.height_();
	this_right = this_x + this_width;
	this_bottom = this_y + this_height;
	
	// Adjust width: if right edge exceeds clip_right
	if (this_right > clip_right)
	{
		Coord new_width = clip_right - this_x;
		if (new_width > 0)
			m_size.width_(new_width);
		else
			m_size.width_(0);
	}
	
	// Adjust height: if bottom edge exceeds clip_bottom
	if (this_bottom > clip_bottom)
	{
		Coord new_height = clip_bottom - this_y;
		if (new_height > 0)
			m_size.height_(new_height);
		else
			m_size.height_(0);
	}
}
/** Print content */
void Area::print(const char * name) const
{
	bd_printf("%s",name);
	m_position.print("");
	m_size.print("");
	bd_printf("\n");
}

