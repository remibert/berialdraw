#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
CommonStyle::CommonStyle()
{
	m_align = ALIGN_DEFAULT;
	m_angle = 0;
	m_angle_modified = 1;
	m_geometry_modified = 1;
	m_borders = ALL_BORDERS;
	m_light = 0;
	m_hidden = false;
	m_saturation = 0;
}

/** Serialize the content of widget into json */
void CommonStyle::serialize(JsonIterator & it)
{
	m_position.serialize("position",it);
	m_size.serialize ("size",it);
	m_margin.serialize ("margin",it);
	berialdraw::serialize("align",it, (Align)m_align);
	it["color"]        = m_color;
	it["light"]        = m_light;
	it["saturation"]   = m_saturation;
	it["hidden"] = m_hidden;

	it["angle_"]       = m_angle;
	m_center.serialize("center",it);
	berialdraw::serialize(it, m_borders);
}

/** Unserialize the content of widget from json */
void CommonStyle::unserialize(JsonIterator & it)
{
	m_position.unserialize("position",it);
	m_size.unserialize("size",it);
	m_margin.unserialize("margin",it);

	m_angle       = (int)(it["angle_"]       | (int)m_angle);
	m_color       = (int)(it["color"]        | (int)m_color);

	m_light       = (int)(it["light"]        | (int)m_light);
	m_saturation  = (int)(it["saturation"]   | (int)m_saturation);

	m_hidden      = (int)(it["hidden"] | (int)m_hidden);

	Align align = (Align)m_align;
	berialdraw::unserialize("align",it, align);
	m_align = align;
	m_center.unserialize("center",it);
	berialdraw::unserialize(it, m_borders);
	m_angle_modified = 1;
	m_geometry_modified = 1;
}

/** Set properties with another */
void CommonStyle::set(const CommonStyle & other)
{
	if (this != &other)
	{
		m_color      = other.m_color;
		m_margin     = other.m_margin;
		m_angle      = other.m_angle;
		m_position   = other.m_position;
		m_center     = other.m_center;
		m_size       = other.m_size;
		m_align      = other.m_align;
		m_borders      = other.m_borders;
		m_light      = other.m_light;
		m_saturation = other.m_saturation;
		m_hidden     = other.m_hidden;
		UIManager::invalidator()->dirty(this, Invalidator::ALL);
	}
}


CommonStyle& CommonStyle::operator=(const CommonStyle& other)
{
	set(other);
	return *this;
}


Dim CommonStyle::reduce(Dim  dimension, Dim reduction)
{
	if(reduction <= dimension)
	{
		dimension -= reduction;
	}
	else
	{
		dimension = 0;
	}
	return dimension;
}

Coord CommonStyle::get_position(Coord position, Coord size, Coord marge_begin, Coord marge_end, Coord area, Align align)
{
	Coord result;

	if (align == ALIGN_LEFT)
	{
		result = position + marge_begin; 
	}
	else if (align == ALIGN_RIGHT)
	{
		result = position + area /*- marge_begin*/ - size - marge_end;
	}
	else
	{
		if((size + marge_begin + marge_end) <= area)
		{
			result = position + ((area - marge_begin - size - marge_end)/2) + marge_begin;
		}
		else
		{
			result = position - ((marge_begin + size + marge_end - area)/2) + marge_begin;
		}
	}
	return result;
}

Dim CommonStyle::adapt_size(Dim content, Dim size, Dim mini, Dim maxi)
{
	Dim result = content;
	if (maxi < mini)
	{
		Dim tmp = maxi;
		maxi = mini;
		mini = tmp;
	}
	if (size < Size::MAX_SIZE)
	{
		result = size;
	}
	else
	{
		result = content;
	}
	if (result < mini)
	{
		result = mini;
	}
	if (result > maxi)
	{
		result = maxi;
	}
	return result;
}

void CommonStyle::place_in_layout(const Area & area, const Size & content, const Margin & margin, Extend extend, Area & foreclip, Align align)
{
	foreclip.set(area.position(), content);

	if (align == ALIGN_DEFAULT)
	{
		align = CENTER;
	}

	Align al;

	if(extend & Extend::EXTEND_WIDTH || (content.width_() + margin.left_() + margin.right_()) >= area.size().width_())
	{
		al = CENTER;
		foreclip.width_(reduce(area.size().width_(), margin.left_() + margin.right_()));
	}
	else if ((align & ALIGN_HORIZONTAL) == ALIGN_LEFT)
	{
		al = ALIGN_LEFT;
	}
	else if ((align & ALIGN_HORIZONTAL) == ALIGN_RIGHT)
	{
		al = ALIGN_RIGHT;
	}
	else
	{
		al = CENTER;
	}

	foreclip.x_(get_position(foreclip.x_(), foreclip.width_(), margin.left_(), margin.right_(), (Coord)area.width_(), al));

	if(extend & Extend::EXTEND_HEIGHT || (content.height_() + margin.top_() + margin.bottom_()) >= area.size().height_())
	{
		al = CENTER;
		foreclip.height_(reduce(area.height_(), margin.top_() + margin.bottom_()));
	}
	if ((align & ALIGN_VERTICAL) == ALIGN_TOP)
	{
		al = ALIGN_LEFT;
	}
	else if ((align & ALIGN_VERTICAL) == ALIGN_BOTTOM)
	{
		al = ALIGN_RIGHT;
	}
	else
	{
		al = CENTER;
	}

	foreclip.y_(get_position(foreclip.y_(), foreclip.height_(), margin.top_(), margin.bottom_(), area.height_(), al));

	foreclip.nearest_pixel();
}

void CommonStyle::place_absolutly(const Point & root, const Size & content, Area & foreclip, const Size & min_size, const Size & max_size)
{
	foreclip.x_(root.x_() + m_position.x_() + m_margin.left_());
	foreclip.y_(root.y_() + m_position.y_() + m_margin.top_());
	foreclip.width_ (adapt_size(content.width_() , m_size.is_width_undefined()  ? content.width_()  : m_size.width_(),  min_size.width_(),  max_size.width_()));
	foreclip.height_(adapt_size(content.height_(), m_size.is_height_undefined() ? content.height_() : m_size.height_(), min_size.height_(), max_size.height_()));
}


Style * CommonStyle::create()
{
	return new CommonStyle;
}



/** Get the margin */
const Margin & CommonStyle::margin() const
{
	return m_margin;
}

/** Set the margin */
void CommonStyle::margin(const Margin & m)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_margin = m;
}

/** Set the margin */
void CommonStyle::margin(Dim value)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_margin.set(value,value,value,value);
}

/** Set the margin */
void CommonStyle::margin(Dim horizontal, Dim vertical)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_margin.set(vertical,horizontal,vertical,horizontal);
}

/** Set the margin in pixels */
void CommonStyle::margin(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_margin.set(top,left,bottom,right);
}

/** Set the margin with a precision of 64th of a pixel */
void CommonStyle::margin_(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_margin.set_(top,left,bottom,right);
}

/** Set the margin top in pixels */
void CommonStyle::margin_top(Dim top)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_margin.set(top, m_margin.left(), m_margin.bottom(), m_margin.right());
}

/** Set the margin left in pixels */
void CommonStyle::margin_left(Dim left)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_margin.set(m_margin.top(), left, m_margin.bottom(), m_margin.right());
}

/** Set the margin bottom in pixels */
void CommonStyle::margin_bottom(Dim bottom)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_margin.set(m_margin.top(), m_margin.left(), bottom, m_margin.right());
}

/** Set the margin right in pixels */
void CommonStyle::margin_right(Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_margin.set(m_margin.top(), m_margin.left(), m_margin.bottom(), right);
}


/** Get the angle */
Coord CommonStyle::angle() const
{
	return m_angle >> 6;
}

/** Get the angle with a precision of 64th of a pixel */
Coord CommonStyle::angle_() const
{
	return m_angle;
}

/** Set the angle in pixels */
void CommonStyle::angle(Coord v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_angle_modified = 1;
	m_geometry_modified = 1;
	m_angle = v << 6;
}

/** Set the angle with a precision of 64th of a pixel */
void CommonStyle::angle_(Coord v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_angle_modified = 1;
	m_geometry_modified = 1;
	m_angle = v;
}


/** Get the center */
const Point & CommonStyle::center() const
{
	return m_center;
}

/** Set the center */
void CommonStyle::center(const Point & center_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_geometry_modified = 1;
	m_center = center_;
}

/** Set the center with x and y in pixels */
void CommonStyle::center(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_geometry_modified = 1;
	m_center.set(x,y);
}

/** Set the center with a precision of 64th of a pixel */
void CommonStyle::center_(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_geometry_modified = 1;
	m_center.set_(x,y);
}



/** Get the position */
const Point & CommonStyle::position() const
{
	return m_position;
}

/** Set the position */
void CommonStyle::position(const Point & position_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_geometry_modified = 1;
	m_position = position_;
}

/** Set the position with x and y in pixels */
void CommonStyle::position(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_geometry_modified = 1;
	m_position.set(x,y);
}

/** Set the position with a precision of 64th of a pixel */
void CommonStyle::position_(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_geometry_modified = 1;
	m_position.set_(x,y);
}


/** Get the back color */
uint32_t CommonStyle::color() const
{
	uint32_t result = UIManager::colors()->color(m_color);

	if (m_light != 0 || m_saturation != 0)
	{
		if (m_light == -m_saturation)
		{
			result = Hsl::enlighten(result, m_light);
		}
		else 
		{
			if (m_light != 0)
			{
				result = Hsl::enlighten(result, m_light);
			}
			if (m_saturation != 0)
			{
				result = Hsl::saturate(result, m_saturation);
			}
		}
	}
	return result;
}

/** Set the back color */
void CommonStyle::color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_color = col;
}

/** Set the back color with alpha */
void CommonStyle::color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_color = (UIManager::colors()->color(col) & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Enlighten the back color
@param light value added [-100..100] */
void CommonStyle::enlighten(int8_t light)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_light = light;
}

/** Saturate the back color
@param saturation value added [-100..100] */
void CommonStyle::saturate(int8_t saturation)
{
	m_saturation = saturation;
}

/** Change to pastel the back color
@param level value added [-100..100] */
void CommonStyle::to_pastel(int8_t level)
{
	m_light = level;
	m_saturation = 0-level;
}



/** Get the size */
const Size & CommonStyle::size() const
{
	return m_size;
}

/** Set the size */
void CommonStyle::size(const Size & size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_geometry_modified = 1;
	m_size = size_;
}

/** Set the size with width and height in pixels */
void CommonStyle::size(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_geometry_modified = 1;
	m_size.set(w,h);
}

/** Set the size with a precision of 64th of a pixel */
void CommonStyle::size_(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_geometry_modified = 1;
	m_size.set_(w,h);
}


/** Get the align */
Align CommonStyle::align() const
{
	return (Align)m_align;
}

/** Set the align */
void CommonStyle::align(Align v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_geometry_modified = 1;
	m_align = (Align)v;
}



/** Select the displayed border of the rectangle */
void CommonStyle::borders(uint16_t side)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_borders = (Borders)side;
}

/** Gets the displayed border of the rectangle */
uint16_t CommonStyle::borders() const
{
	return m_borders;
}

/** Get the hidden widget state */
bool CommonStyle::hidden() const
{
	return m_hidden == true;
}

/** Set the hidden widget state */
void CommonStyle::hidden(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	if (v)
	{
		m_hidden = 1;
	}
	else
	{
		m_hidden = 0;
	}
}
