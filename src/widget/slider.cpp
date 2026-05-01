#include "berialdraw_imp.hpp"

using namespace berialdraw;

Slider::Slider(Widget * parent):
	Widget("slider", parent, sizeof(Slider))
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
	UIManager::styles()->apply(this, (SliderStyle*)this);
	bind(this, &Slider::on_key);
	bind(this, &Slider::on_click);
	bind(this, &Slider::on_select);
}

Slider::~Slider()
{
}

/** Copy all styles of the sliderar */
void Slider::copy(const Slider & slider)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&slider);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&slider);
	*((BorderStyle*)this) = *(BorderStyle*)(&slider);
	*((SliderStyle*)this) = *(SliderStyle*)(&slider);
}

/** Copy all styles of the sliderar */
void Slider::copy(const Slider * slider)
{
	if(slider)
	{
		copy(*slider);
	}
}

/** Return the size of content without marges */
Size Slider::content_size()
{
	Size size;
	if (m_handle_size > m_track_size)
	{
		size.set_(m_handle_size,m_handle_size);
	}
	else
	{
		size.set_(m_track_size,m_track_size);
	}

	if (m_extend == Extend::EXTEND_HEIGHT)
	{
		if (size.width() %2)
		{
			size.width(size.width()+1);
		}
	}
	else 
	{
		if (size.height() %2)
		{
			size.height(size.height()+1);
		}
	}

	return size;
}

void Slider::place(const Area & area, bool in_layout)
{
	if (!is_absolute())
	{
		in_layout = true;
	}

	// If margin not support the focus thickness enlarge it
	if (m_margin.left() < (Dim)(m_focus_thickness>>1))
	{
		m_margin.left(m_focus_thickness>>1);
	}
	if (m_margin.right() < (Dim)(m_focus_thickness>>1))
	{
		m_margin.right(m_focus_thickness>>1);
	}
	if (m_margin.top() < (Dim)(m_focus_thickness>>1))
	{
		m_margin.top(m_focus_thickness>>1);
	}
	if (m_margin.bottom() < (Dim)(m_focus_thickness>>1))
	{
		m_margin.bottom(m_focus_thickness>>1);
	}
	place_in_area(area, in_layout);

	// If absolute place
	if (in_layout == false)
	{
		Area backclip = m_foreclip;
		Margin marg;
		Size size(content_size());

		if (m_extend == Extend::EXTEND_HEIGHT)
		{
			size.height_(m_foreclip.height_());
		}
		else
		{
			size.width_(m_foreclip.width_());
		}
		place_in_layout(backclip, size, marg, EXTEND_NONE, m_foreclip, (Align)m_align);
	}
}

Dim Slider::get_location(Dim length)
{
	Dim result = 0;

	if ((m_max_value - m_min_value) > 0)
	{
		if (m_step_value > 0)
		{
			Dim value = m_value;

			Dim delta = m_value % m_step_value;

			if (delta > 0)
			{
				if (delta > m_step_value>>1)
				{
					m_value += (m_step_value - delta);
				}
				else
				{
					m_value -= delta;
				}
			}
		}
		check_slider();
		result = (m_value * (length-m_handle_size)) / (m_max_value - m_min_value);
	}
	return result;
}

void Slider::draw_track()
{
	Coord x;
	Coord y;

	Dim      track_size = (m_track_size>>6) % 2 ? m_track_size + 64 : m_track_size;
	Dim      track_radius = m_radius;
	uint32_t track_border_color;
	Dim      track_thickness;
	Area     track_area(m_foreclip);

	Dim      handle_size  = (m_handle_size >>6) % 2 ? m_handle_size  + 64 : m_handle_size;
	Dim      handle_radius = m_radius;
	Dim      handle_thickness;
	uint32_t handle_border_color;
	Area     handle_area(m_foreclip);

	Dim half_thickness = ((m_thickness>>7)<<6);

	// If the track is thicker than fill
	if (track_size >= handle_size)
	{
		Dim dif_radius = (track_size-handle_size) >> 1;
		
		track_thickness = m_thickness;
		handle_thickness = 0;
		handle_radius = substract(handle_radius, dif_radius);
		handle_border_color = Color::TRANSPARENT;
		track_border_color = stated_color(m_border_color);
		
		// Vertical slider
		if (m_extend == Extend::EXTEND_HEIGHT)
		{
			y = m_foreclip.y_() + get_location(substract(m_foreclip.height_(),dif_radius+dif_radius)) + dif_radius;
			x = m_foreclip.x_() + dif_radius;
		}
		// Horizontal slider
		else
		{
			x = m_foreclip.x_() + get_location(substract(m_foreclip.width_(),dif_radius+dif_radius)) + dif_radius;
			y = m_foreclip.y_() + dif_radius;
		}
	}
	// If the track is thinner than fill
	else if (track_size < handle_size)
	{
		Dim dif_radius = (handle_size-track_size)>>1;

		handle_thickness = m_thickness;
		track_thickness = 0;

		track_radius = substract(track_radius, dif_radius);
		track_border_color = Color::TRANSPARENT;
		handle_border_color = stated_color(m_border_color);

		// Vertical slider
		if (m_extend == Extend::EXTEND_HEIGHT)
		{
			track_area.x_(m_foreclip.x_()+dif_radius);
			track_area.y_(m_foreclip.y_()+dif_radius);

			track_area.width_(track_size);
			track_area.height_(substract(track_area.height_(),dif_radius+dif_radius));

			y = m_foreclip.y_() + get_location(m_foreclip.height_());
			x = m_foreclip.x_();
		}
		// Horizontal slider
		else
		{
			track_area.x_(m_foreclip.x_()+dif_radius);
			track_area.y_(m_foreclip.y_()+dif_radius);

			track_area.width_(substract(track_area.width_(),dif_radius+dif_radius));
			track_area.height_(track_size);
			
			x = m_foreclip.x_() + get_location(m_foreclip.width_());
			y = m_foreclip.y_();
		}
	}

	// Move handle
	handle_area.position().set_(x,y);
	handle_area.size().set_(m_handle_size,m_handle_size);

	// Draw track
	track_area.nearest_pixel();
	Rect::build_polygon(track_area, track_radius, track_thickness, 0, m_borders, stated_color(m_track_color), track_border_color);

	// Draw fill
	handle_area.nearest_pixel();

	Rect::build_focused_polygon(handle_area, 
		*(CommonStyle*)this,
		*(BorderStyle*)this,
		stated_color(m_handle_color), 
		handle_border_color,
		Color::TRANSPARENT, 
		stated_color(m_focus_color),
		m_focused);
}

void Slider::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);
		check_slider();
		draw_track();
	}
}

/** Get the widget hovered */
Widget * Slider::hovered(const Region & parent_region, const Point & position)
{
	Region region(parent_region);
	region.intersect(m_foreclip);

	// If the widget hovered
	if(region.is_inside(position) != Region::Overlap::OUT)
	{
		return this;
	}
	return 0;
}

/** Serialize the content of widget into json */
void Slider::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	BorderStyle::serialize(it);
	SliderStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Slider::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	BorderStyle::unserialize(it);
	SliderStyle::unserialize(it);
}


/** Call back on key */
void Slider::on_key(Widget * widget, const KeyEvent & evt)
{
	if (m_focused)
	{
		if (evt.state() == KeyEvent::KEY_DOWN)
		{
			bool notify = false;
			if ((m_extend == Extend::EXTEND_HEIGHT && evt.key() == (wchar_t)ReservedKey::KEY_UP) ||
				(m_extend != Extend::EXTEND_HEIGHT && evt.key() == (wchar_t)ReservedKey::KEY_LEFT))
				
			{
				m_value -= m_step_value;
				notify = true;
			}
			else if ((m_extend == Extend::EXTEND_HEIGHT && evt.key() == (wchar_t)ReservedKey::KEY_DOWN) ||
					 (m_extend != Extend::EXTEND_HEIGHT && evt.key() == (wchar_t)ReservedKey::KEY_RIGHT))
			{
				m_value += m_step_value;
				notify = true;
			}

			if (notify)
			{
				check_slider();
				UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
				UIManager::notifier()->slide(m_value, this);
			}
		}
	}
}

/** Touch handle */
void Slider::touch_handle(const Point & touch_position)
{
	check_slider();
	int64_t position;
	Dim length;

	if (m_extend == Extend::EXTEND_HEIGHT)
	{
		position = (int64_t)touch_position.y_() - (int64_t)m_foreclip.position().y_()- (int64_t)(m_handle_size>>1);
		length = m_foreclip.height_();
	}
	else
	{
		position = (int64_t)touch_position.x_() - (int64_t)m_foreclip.position().x_() - (int64_t)(m_handle_size>>1);
		length = m_foreclip.width_();
	}

	if (m_track_size >= m_handle_size)
	{
		length = substract(length, m_track_size-m_handle_size);
	}

	if (length != m_handle_size)
	{
		m_value = (int32_t)((((int64_t)m_max_value  - (int64_t)m_min_value)* position)/(length-m_handle_size));
		check_slider();
		UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
		UIManager::notifier()->slide(m_value, this);
	}
}


/** Call back on click */
void Slider::on_click(Widget * widget, const ClickEvent & evt)
{
	touch_handle(evt.position());
}

/** Call back on select */
void Slider::on_select(Widget * widget, const SelectEvent & evt)
{
	touch_handle(evt.position());
}


