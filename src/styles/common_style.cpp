#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
CommonStyle::CommonStyle()
{
	m_align = Align::ALIGN_DEFAULT;
	m_angle = 0;
	m_angle_modified = 1;
	m_geometry_modified = 1;
	m_style_modified = 1;
	m_borders = ALL_BORDERS;
	m_light = 0;
	m_hidden = false;
	m_saturation = 0;
}

/** Serialize the content of widget into json */
void CommonStyle::serialize(JsonIterator & it)
{
	m_position.serialize(StyleNames::COMMON_POSITION,it);
	m_size.serialize (StyleNames::COMMON_SIZE,it);
	m_margin.serialize (StyleNames::COMMON_MARGIN,it);
	berialdraw::serialize(StyleNames::COMMON_ALIGN,it, m_align);
	it[StyleNames::COMMON_COLOR]        = m_color;
	it[StyleNames::COMMON_LIGHT]        = m_light;
	it[StyleNames::COMMON_SATURATION]   = m_saturation;
	it[StyleNames::COMMON_HIDDEN] = m_hidden;

	it[StyleNames::COMMON_ANGLE]       = m_angle;
	m_center.serialize(StyleNames::COMMON_CENTER,it);
	berialdraw::serialize(it, m_borders);
}

/** Unserialize the content of widget from json */
void CommonStyle::unserialize(JsonIterator & it)
{
	m_position.unserialize(StyleNames::COMMON_POSITION,it);
	m_size.unserialize(StyleNames::COMMON_SIZE,it);
	m_margin.unserialize(StyleNames::COMMON_MARGIN,it);

	m_angle       = (int)(it[StyleNames::COMMON_ANGLE]       | (int)m_angle);
	m_color       = (int)(it[StyleNames::COMMON_COLOR]        | (int)m_color);

	m_light       = (int)(it[StyleNames::COMMON_LIGHT]        | (int)m_light);
	m_saturation  = (int)(it[StyleNames::COMMON_SATURATION]   | (int)m_saturation);

	m_hidden      = (int)(it[StyleNames::COMMON_HIDDEN] | (int)m_hidden);

	Align align = m_align;
	berialdraw::unserialize(StyleNames::COMMON_ALIGN,it, align);
	m_align = align;
	m_center.unserialize(StyleNames::COMMON_CENTER,it);
	berialdraw::unserialize(it, m_borders);
	m_angle_modified = 1;
	m_geometry_modified = 1;
	m_style_modified = 1;
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

	if (align == Align::ALIGN_LEFT)
	{
		result = position + marge_begin; 
	}
	else if (align == Align::ALIGN_RIGHT)
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

	if (align == Align::ALIGN_DEFAULT)
	{
		align = Align::CENTER;
	}

	Align al;

	if(extend & Extend::EXTEND_WIDTH || (content.width_q6() + margin.left_q6() + margin.right_q6()) >= area.size().width_q6())
	{
		al = Align::CENTER;
		foreclip.width_q6(reduce(area.size().width_q6(), margin.left_q6() + margin.right_q6()));
	}
	else if ((align & Align::ALIGN_HORIZONTAL) == Align::ALIGN_LEFT)
	{
		al = Align::ALIGN_LEFT;
	}
	else if ((align & Align::ALIGN_HORIZONTAL) == Align::ALIGN_RIGHT)
	{
		al = Align::ALIGN_RIGHT;
	}
	else
	{
		al = Align::CENTER;
	}

	foreclip.x_q6(get_position(foreclip.x_q6(), foreclip.width_q6(), margin.left_q6(), margin.right_q6(), (Coord)area.width_q6(), al));

	if(extend & Extend::EXTEND_HEIGHT || (content.height_q6() + margin.top_q6() + margin.bottom_q6()) >= area.size().height_q6())
	{
		al = Align::CENTER;
		foreclip.height_q6(reduce(area.height_q6(), margin.top_q6() + margin.bottom_q6()));
	}
	if ((align & Align::ALIGN_VERTICAL) == Align::ALIGN_TOP)
	{
		al = Align::ALIGN_LEFT;
	}
	else if ((align & Align::ALIGN_VERTICAL) == Align::ALIGN_BOTTOM)
	{
		al = Align::ALIGN_RIGHT;
	}
	else
	{
		al = Align::CENTER;
	}

	foreclip.y_q6(get_position(foreclip.y_q6(), foreclip.height_q6(), margin.top_q6(), margin.bottom_q6(), area.height_q6(), al));

	foreclip.nearest_pixel();
}

void CommonStyle::place_absolutly(const Point & root, const Size & content, Area & foreclip, const Size & size, const Size & min_size, const Size & max_size)
{
	foreclip.x_q6(root.x_q6() + m_position.x_q6() + m_margin.left_q6());
	foreclip.y_q6(root.y_q6() + m_position.y_q6() + m_margin.top_q6());
	foreclip.width_q6(adapt_size(content.width_q6() , size.is_width_undefined()  ? content.width_q6()  : size.width_q6(),  min_size.width_q6(),  max_size.width_q6()));
	foreclip.height_q6(adapt_size(content.height_q6(), size.is_height_undefined() ? content.height_q6() : size.height_q6(), min_size.height_q6(), max_size.height_q6()));
}


Style * CommonStyle::create()
{
	return new CommonStyle;
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
void CommonStyle::margin_q6(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_margin.set_q6(top,left,bottom,right);
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

/** Set the angle in pixels */
void CommonStyle::angle(Coord v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_angle_modified = 1;
	m_geometry_modified = 1;
	m_angle = v << 6;
}

/** Set the angle with a precision of 64th of a pixel */
void CommonStyle::angle_q6(Coord v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_angle_modified = 1;
	m_geometry_modified = 1;
	m_angle = v;
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
void CommonStyle::center_q6(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_geometry_modified = 1;
	m_center.set_q6(x,y);
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
void CommonStyle::position_q6(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_geometry_modified = 1;
	m_position.set_q6(x,y);
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

/** Set the color */
void CommonStyle::color(Color col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_color = (uint32_t)col;
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
void CommonStyle::size_q6(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_geometry_modified = 1;
	m_size.set_q6(w,h);
}

/** Set the align */
void CommonStyle::align(Align v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_geometry_modified = 1;
	m_align = v;
}

/** Select the displayed border of the rectangle */
void CommonStyle::borders(uint16_t side)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_borders = (Borders)side;
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


