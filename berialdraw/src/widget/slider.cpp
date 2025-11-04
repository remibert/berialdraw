#include "berialdraw_imp.hpp"

using namespace berialdraw;

Slider::Slider(Widget * parent):
	Widget("slider", parent, sizeof(Slider))
{
	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);
	UIManager::styles()->apply(m_classname, (BorderStyle*)this);
	UIManager::styles()->apply(m_classname, (SliderStyle*)this);
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
	if (m_size.is_width_undefined() && m_size.is_height_undefined() && m_extend != Extend::EXTEND_NONE && 
		m_position.is_x_undefined() && m_position.is_y_undefined())
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


#ifdef _DEBUG

void Slider::test1()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(43,37);
		window.size(240,240);

	Grid * main_layout = new Grid(&window);
	Slider * slider;
	int id = 0;

	{
		Grid * row_layout = new Grid(main_layout);
		row_layout->cell(0,0);
		for (int row = 0; row < 3; row ++)
		{
			slider = new Slider(row_layout);
				slider->cell(row,0);
				slider->id(++id);
				slider->extend(Extend::EXTEND_WIDTH);
				slider->value(45+(4*row));
				slider->min_value(0);
				slider->max_value(100);
				slider->step_value(10);
				switch(row)
				{
				case 0 : slider->align(Align::ALIGN_TOP);break;
				case 1 : slider->align(Align::CENTER);break;
				case 2 : slider->align(Align::ALIGN_BOTTOM);break;
				}
		}
	}
	{
		Grid * column_layout = new Grid(main_layout);
		column_layout->cell(0,1);
		for (int column = 0; column < 3; column ++)
		{
			slider = new Slider(column_layout);
				slider->cell(0,column);
				slider->extend(Extend::EXTEND_HEIGHT);
				slider->id(++id);
				slider->value(45+(4*column));
				slider->min_value(0);
				slider->max_value(100);
				slider->step_value(10);
				switch(column)
				{
				case 0 : slider->align(Align::ALIGN_LEFT);break;
				case 1 : slider->align(Align::CENTER);break;
				case 2 : slider->align(Align::ALIGN_RIGHT);break;
				}
		}
	}

//UIManager::desktop()->mainloop();
	
	String script(
	"["
		"{'type':'touch','x':360,'y':  6,'state':'move'},"
		"{'type':'touch','x':325,'y': 15,'state':'move'},"
		"{'type':'touch','x':304,'y': 11,'state':'move'},"
		"{'type':'touch','x':294,'y':  4,'state':'move'},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':175,'y': 17,'state':'move'},"
		"{'type':'touch','x':126,'y': 30,'state':'move'},"
		"{'type':'touch','x': 94,'y': 39,'state':'move'},"
		"{'type':'touch','x': 79,'y': 46,'state':'move'},"
		"{'type':'touch','x': 80,'y': 49,'state':'move'},"
		"{'type':'touch','x': 89,'y': 49,'state':'move'},"
		"{'type':'touch','x': 91,'y': 49,'state':'down'},"
		"{'type':'touch','x': 93,'y': 49,'state':'move'},"
		"{'type':'touch','x':100,'y': 49,'state':'move'},"
		"{'type':'touch','x':111,'y': 50,'state':'move'},"
		"{'type':'touch','x':120,'y': 51,'state':'move'},"
		"{'type':'touch','x':128,'y': 51,'state':'move'},"
		"{'type':'touch','x':137,'y': 51,'state':'move'},"
		"{'type':'touch','x':149,'y': 51,'state':'move'},"
		"{'type':'touch','x':160,'y': 51,'state':'move'},"
		"{'type':'touch','x':165,'y': 52,'state':'move'},"
		"{'type':'touch','x':169,'y': 52,'state':'move'},"
		"{'type':'touch','x':171,'y': 52,'state':'move'},"
		"{'type':'touch','x':164,'y': 51,'state':'move'},"
		"{'type':'touch','x':144,'y': 51,'state':'move'},"
		"{'type':'touch','x':107,'y': 51,'state':'move'},"
		"{'type':'touch','x': 62,'y': 50,'state':'move'},"
		"{'type':'touch','x': 29,'y': 50,'state':'move'},"
		"{'type':'touch','x':  3,'y': 50,'state':'move'},"
		"{'type':'touch','x':  5,'y': 53,'state':'up'},"
		"{'type':'touch','x': 21,'y': 66,'state':'move'},"
		"{'type':'touch','x': 34,'y': 84,'state':'move'},"
		"{'type':'touch','x': 41,'y':102,'state':'move'},"
		"{'type':'touch','x': 47,'y':121,'state':'move'},"
		"{'type':'touch','x': 52,'y':131,'state':'move'},"
		"{'type':'touch','x': 54,'y':133,'state':'move'},"
		"{'type':'touch','x': 57,'y':135,'state':'move'},"
		"{'type':'touch','x': 60,'y':140,'state':'move'},"
		"{'type':'touch','x': 63,'y':147,'state':'move'},"
		"{'type':'touch','x': 66,'y':154,'state':'move'},"
		"{'type':'touch','x': 68,'y':156,'state':'move'},"
		"{'type':'touch','x': 72,'y':160,'state':'move'},"
		"{'type':'touch','x': 73,'y':161,'state':'down'},"
		"{'type':'touch','x': 80,'y':161,'state':'move'},"
		"{'type':'touch','x':111,'y':161,'state':'move'},"
		"{'type':'touch','x':136,'y':161,'state':'move'},"
		"{'type':'touch','x':150,'y':161,'state':'move'},"
		"{'type':'touch','x':159,'y':161,'state':'move'},"
		"{'type':'touch','x':159,'y':161,'state':'move'},"
		"{'type':'touch','x':150,'y':161,'state':'move'},"
		"{'type':'touch','x':120,'y':160,'state':'move'},"
		"{'type':'touch','x': 63,'y':156,'state':'move'},"
		"{'type':'touch','x': 32,'y':154,'state':'move'},"
		"{'type':'touch','x': 14,'y':153,'state':'move'},"
		"{'type':'touch','x':  5,'y':153,'state':'move'},"
		"{'type':'touch','x':  2,'y':154,'state':'up'},"
		"{'type':'touch','x':  3,'y':154,'state':'move'},"
		"{'type':'touch','x': 75,'y':153,'state':'move'},"
		"{'type':'touch','x':110,'y':154,'state':'move'},"
		"{'type':'touch','x':125,'y':154,'state':'move'},"
		"{'type':'touch','x':133,'y':154,'state':'move'},"
		"{'type':'touch','x':139,'y':150,'state':'move'},"
		"{'type':'touch','x':158,'y':140,'state':'move'},"
		"{'type':'touch','x':172,'y':132,'state':'move'},"
		"{'type':'touch','x':173,'y':130,'state':'down'},"
		"{'type':'touch','x':173,'y':129,'state':'move'},"
		"{'type':'touch','x':173,'y':121,'state':'move'},"
		"{'type':'touch','x':173,'y':102,'state':'move'},"
		"{'type':'touch','x':173,'y': 89,'state':'move'},"
		"{'type':'touch','x':173,'y': 77,'state':'move'},"
		"{'type':'touch','x':174,'y': 65,'state':'move'},"
		"{'type':'touch','x':174,'y': 55,'state':'move'},"
		"{'type':'touch','x':174,'y': 47,'state':'move'},"
		"{'type':'touch','x':174,'y': 43,'state':'move'},"
		"{'type':'touch','x':173,'y': 43,'state':'move'},"
		"{'type':'touch','x':173,'y': 47,'state':'move'},"
		"{'type':'touch','x':173,'y': 62,'state':'move'},"
		"{'type':'touch','x':171,'y':110,'state':'move'},"
		"{'type':'touch','x':171,'y':141,'state':'move'},"
		"{'type':'touch','x':171,'y':159,'state':'move'},"
		"{'type':'touch','x':171,'y':188,'state':'move'},"
		"{'type':'touch','x':171,'y':223,'state':'move'},"
		"{'type':'touch','x':172,'y':247,'state':'move'},"
		"{'type':'touch','x':173,'y':253,'state':'move'},"
		"{'type':'touch','x':174,'y':258,'state':'move'},"
		"{'type':'touch','x':174,'y':270,'state':'move'},"
		"{'type':'touch','x':174,'y':277,'state':'move'},"
		"{'type':'touch','x':175,'y':280,'state':'move'},"
		"{'type':'touch','x':176,'y':278,'state':'move'},"
		"{'type':'touch','x':178,'y':265,'state':'move'},"
		"{'type':'touch','x':184,'y':241,'state':'move'},"
		"{'type':'touch','x':188,'y':209,'state':'move'},"
		"{'type':'touch','x':190,'y':186,'state':'move'},"
		"{'type':'touch','x':191,'y':171,'state':'move'},"
		"{'type':'touch','x':190,'y':165,'state':'move'},"
		"{'type':'touch','x':190,'y':163,'state':'move'},"
		"{'type':'touch','x':190,'y':150,'state':'move'},"
		"{'type':'touch','x':190,'y':136,'state':'move'},"
		"{'type':'touch','x':188,'y':128,'state':'move'},"
		"{'type':'touch','x':188,'y':124,'state':'move'},"
		"{'type':'touch','x':188,'y':120,'state':'move'},"
		"{'type':'touch','x':186,'y':120,'state':'move'},"
		"{'type':'touch','x':186,'y':120,'state':'up'},"
		"{'type':'touch','x':186,'y':120,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':207,'y':114,'state':'move'},"
		"{'type':'touch','x':223,'y':111,'state':'move'},"
		"{'type':'touch','x':226,'y':108,'state':'move'},"
		"{'type':'touch','x':223,'y':106,'state':'move'},"
		"{'type':'touch','x':223,'y':105,'state':'down'},"
		"{'type':'touch','x':223,'y':105,'state':'up'},"
		"{'type':'touch','x':229,'y':103,'state':'move'},"
		"{'type':'touch','x':235,'y':102,'state':'move'},"
		"{'type':'touch','x':241,'y':108,'state':'move'},"
		"{'type':'touch','x':243,'y':131,'state':'move'},"
		"{'type':'touch','x':229,'y':181,'state':'move'},"
		"{'type':'touch','x':225,'y':210,'state':'move'},"
		"{'type':'touch','x':225,'y':213,'state':'move'},"
		"{'type':'touch','x':235,'y':217,'state':'move'},"
		"{'type':'touch','x':246,'y':219,'state':'move'},"
		"{'type':'touch','x':255,'y':219,'state':'move'},"
		"{'type':'touch','x':259,'y':219,'state':'move'},"
		"{'type':'touch','x':263,'y':222,'state':'move'},"
		"{'type':'touch','x':266,'y':225,'state':'move'},"
		"{'type':'touch','x':268,'y':228,'state':'move'},"
		"{'type':'touch','x':270,'y':230,'state':'move'},"
		"{'type':'touch','x':270,'y':230,'state':'down'},"
		"{'type':'touch','x':270,'y':230,'state':'up'},"
		"{'type':'touch','x':224,'y':222,'state':'move'},"
		"{'type':'touch','x':182,'y':213,'state':'move'},"
		"{'type':'touch','x':171,'y':211,'state':'move'},"
		"{'type':'touch','x':163,'y':206,'state':'move'},"
		"{'type':'touch','x':150,'y':195,'state':'move'},"
		"{'type':'touch','x':144,'y':187,'state':'move'},"
		"{'type':'touch','x':137,'y':178,'state':'move'},"
		"{'type':'touch','x':135,'y':170,'state':'move'},"
		"{'type':'touch','x':135,'y':166,'state':'move'},"
		"{'type':'touch','x':134,'y':160,'state':'move'},"
		"{'type':'touch','x':134,'y':157,'state':'down'},"
		"{'type':'touch','x':134,'y':157,'state':'up'},"
		"{'type':'touch','x':134,'y':154,'state':'move'},"
		"{'type':'touch','x':134,'y':135,'state':'move'},"
		"{'type':'touch','x':130,'y': 93,'state':'move'},"
		"{'type':'touch','x':128,'y': 83,'state':'move'},"
		"{'type':'touch','x':127,'y': 77,'state':'move'},"
		"{'type':'touch','x':124,'y': 67,'state':'move'},"
		"{'type':'touch','x':122,'y': 58,'state':'move'},"
		"{'type':'touch','x':121,'y': 53,'state':'move'},"
		"{'type':'touch','x':121,'y': 48,'state':'move'},"
		"{'type':'touch','x':121,'y': 47,'state':'down'},"
		"{'type':'touch','x':121,'y': 47,'state':'up'},"
		"{'type':'touch','x':121,'y': 76,'state':'move'},"
		"{'type':'touch','x': 76,'y':222,'state':'move'},"
		"{'type':'touch','x': 22,'y':348,'state':'move'},"
		"{'type':'touch','x': 14,'y':378,'state':'move'},"
		"{'type':'touch','x': 14,'y':377,'state':'move'},"
		"{'type':'touch','x': 18,'y':364,'state':'move'},"
		"{'type':'touch','x': 52,'y':311,'state':'move'},"
		"{'type':'touch','x': 77,'y':277,'state':'move'},"
		"{'type':'touch','x': 81,'y':270,'state':'move'},"
		"{'type':'touch','x': 80,'y':268,'state':'move'},"
		"{'type':'touch','x': 75,'y':266,'state':'move'},"
		"{'type':'touch','x': 69,'y':265,'state':'move'},"
		"{'type':'touch','x': 65,'y':265,'state':'move'},"
		"{'type':'touch','x': 65,'y':265,'state':'down'},"
		"{'type':'touch','x': 65,'y':265,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':111,'y':300,'state':'move'},"
		"{'type':'touch','x':144,'y':331,'state':'move'},"
		"{'type':'touch','x':172,'y':378,'state':'move'},"
		"{'type':'touch','x':175,'y':389,'state':'move'},"
		"{'type':'touch','x':175,'y':397,'state':'move'},"
		"{'type':'touch','x':181,'y':399,'state':'move'},"
		"{'type':'touch','x':211,'y':422,'state':'move'},"
		"{'type':'touch','x':255,'y':465,'state':'move'},"
		"{'type':'touch','x':270,'y':483,'state':'move'},"
	"]");

	UIManager::notifier()->play_script(script, "test/out/slider1_%d.svg");
}

void Slider::test2()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(43,37);
		window.size(240,240);

	Grid * main_layout = new Grid(&window);
	Slider * slider;
	int id = 0;

	{
		Grid * row_layout = new Grid(main_layout);
		row_layout->cell(0,0);
		for (int row = 0; row < 3; row ++)
		{
			slider = new Slider(row_layout);
				slider->cell(row,0);
				slider->id(++id);
				slider->extend(Extend::EXTEND_WIDTH);
				slider->value(45+(4*row));
				slider->radius(0);
				slider->track_size(20);
				slider->handle_size(16);
				slider->track_color(Color::LIGHT_JADE);
				slider->handle_color(Color::GREEN_PEPPER);
				slider->min_value(0);
				slider->max_value(100);
				slider->step_value(10);
				switch(row)
				{
				case 0 : slider->align(Align::ALIGN_TOP);break;
				case 1 : slider->align(Align::CENTER);break;
				case 2 : slider->align(Align::ALIGN_BOTTOM);break;
				}
		}
	}
	{
		Grid * column_layout = new Grid(main_layout);
		column_layout->cell(0,1);
		for (int column = 0; column < 3; column ++)
		{
			slider = new Slider(column_layout);
				slider->cell(0,column);
				slider->extend(Extend::EXTEND_HEIGHT);
				slider->id(++id);
				slider->value(45+(4*column));
				slider->min_value(0);
				slider->max_value(100);
				slider->step_value(10);

				slider->radius(10);
				slider->track_size(20);
				slider->handle_size(16);
				slider->track_color(Color::WATER);
				slider->handle_color(Color::CRYSTAL_BLUE);

				switch(column)
				{
				case 0 : slider->align(Align::ALIGN_LEFT);break;
				case 1 : slider->align(Align::CENTER);break;
				case 2 : slider->align(Align::ALIGN_RIGHT);break;
				}
		}
	}

	//UIManager::desktop()->mainloop();
	
	String script(
	"["
		"{'type':'touch','x':360,'y':  6,'state':'move'},"
		"{'type':'touch','x':325,'y': 15,'state':'move'},"
		"{'type':'touch','x':304,'y': 11,'state':'move'},"
		"{'type':'touch','x':294,'y':  4,'state':'move'},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':175,'y': 17,'state':'move'},"
		"{'type':'touch','x':126,'y': 30,'state':'move'},"
		"{'type':'touch','x': 94,'y': 39,'state':'move'},"
		"{'type':'touch','x': 79,'y': 46,'state':'move'},"
		"{'type':'touch','x': 80,'y': 49,'state':'move'},"
		"{'type':'touch','x': 89,'y': 49,'state':'move'},"
		"{'type':'touch','x': 91,'y': 49,'state':'down'},"
		"{'type':'touch','x': 93,'y': 49,'state':'move'},"
		"{'type':'touch','x':100,'y': 49,'state':'move'},"
		"{'type':'touch','x':111,'y': 50,'state':'move'},"
		"{'type':'touch','x':120,'y': 51,'state':'move'},"
		"{'type':'touch','x':128,'y': 51,'state':'move'},"
		"{'type':'touch','x':137,'y': 51,'state':'move'},"
		"{'type':'touch','x':149,'y': 51,'state':'move'},"
		"{'type':'touch','x':160,'y': 51,'state':'move'},"
		"{'type':'touch','x':165,'y': 52,'state':'move'},"
		"{'type':'touch','x':169,'y': 52,'state':'move'},"
		"{'type':'touch','x':171,'y': 52,'state':'move'},"
		"{'type':'touch','x':164,'y': 51,'state':'move'},"
		"{'type':'touch','x':144,'y': 51,'state':'move'},"
		"{'type':'touch','x':107,'y': 51,'state':'move'},"
		"{'type':'touch','x': 62,'y': 50,'state':'move'},"
		"{'type':'touch','x': 29,'y': 50,'state':'move'},"
		"{'type':'touch','x':  3,'y': 50,'state':'move'},"
		"{'type':'touch','x':  5,'y': 53,'state':'up'},"
		"{'type':'touch','x': 21,'y': 66,'state':'move'},"
		"{'type':'touch','x': 34,'y': 84,'state':'move'},"
		"{'type':'touch','x': 41,'y':102,'state':'move'},"
		"{'type':'touch','x': 47,'y':121,'state':'move'},"
		"{'type':'touch','x': 52,'y':131,'state':'move'},"
		"{'type':'touch','x': 54,'y':133,'state':'move'},"
		"{'type':'touch','x': 57,'y':135,'state':'move'},"
		"{'type':'touch','x': 60,'y':140,'state':'move'},"
		"{'type':'touch','x': 63,'y':147,'state':'move'},"
		"{'type':'touch','x': 66,'y':154,'state':'move'},"
		"{'type':'touch','x': 68,'y':156,'state':'move'},"
		"{'type':'touch','x': 72,'y':160,'state':'move'},"
		"{'type':'touch','x': 73,'y':161,'state':'down'},"
		"{'type':'touch','x': 80,'y':161,'state':'move'},"
		"{'type':'touch','x':111,'y':161,'state':'move'},"
		"{'type':'touch','x':136,'y':161,'state':'move'},"
		"{'type':'touch','x':150,'y':161,'state':'move'},"
		"{'type':'touch','x':159,'y':161,'state':'move'},"
		"{'type':'touch','x':159,'y':161,'state':'move'},"
		"{'type':'touch','x':150,'y':161,'state':'move'},"
		"{'type':'touch','x':120,'y':160,'state':'move'},"
		"{'type':'touch','x': 63,'y':156,'state':'move'},"
		"{'type':'touch','x': 32,'y':154,'state':'move'},"
		"{'type':'touch','x': 14,'y':153,'state':'move'},"
		"{'type':'touch','x':  5,'y':153,'state':'move'},"
		"{'type':'touch','x':  2,'y':154,'state':'up'},"
		"{'type':'touch','x':  3,'y':154,'state':'move'},"
		"{'type':'touch','x': 75,'y':153,'state':'move'},"
		"{'type':'touch','x':110,'y':154,'state':'move'},"
		"{'type':'touch','x':125,'y':154,'state':'move'},"
		"{'type':'touch','x':133,'y':154,'state':'move'},"
		"{'type':'touch','x':139,'y':150,'state':'move'},"
		"{'type':'touch','x':158,'y':140,'state':'move'},"
		"{'type':'touch','x':172,'y':132,'state':'move'},"
		"{'type':'touch','x':173,'y':130,'state':'down'},"
		"{'type':'touch','x':173,'y':129,'state':'move'},"
		"{'type':'touch','x':173,'y':121,'state':'move'},"
		"{'type':'touch','x':173,'y':102,'state':'move'},"
		"{'type':'touch','x':173,'y': 89,'state':'move'},"
		"{'type':'touch','x':173,'y': 77,'state':'move'},"
		"{'type':'touch','x':174,'y': 65,'state':'move'},"
		"{'type':'touch','x':174,'y': 55,'state':'move'},"
		"{'type':'touch','x':174,'y': 47,'state':'move'},"
		"{'type':'touch','x':174,'y': 43,'state':'move'},"
		"{'type':'touch','x':173,'y': 43,'state':'move'},"
		"{'type':'touch','x':173,'y': 47,'state':'move'},"
		"{'type':'touch','x':173,'y': 62,'state':'move'},"
		"{'type':'touch','x':171,'y':110,'state':'move'},"
		"{'type':'touch','x':171,'y':141,'state':'move'},"
		"{'type':'touch','x':171,'y':159,'state':'move'},"
		"{'type':'touch','x':171,'y':188,'state':'move'},"
		"{'type':'touch','x':171,'y':223,'state':'move'},"
		"{'type':'touch','x':172,'y':247,'state':'move'},"
		"{'type':'touch','x':173,'y':253,'state':'move'},"
		"{'type':'touch','x':174,'y':258,'state':'move'},"
		"{'type':'touch','x':174,'y':270,'state':'move'},"
		"{'type':'touch','x':174,'y':277,'state':'move'},"
		"{'type':'touch','x':175,'y':280,'state':'move'},"
		"{'type':'touch','x':176,'y':278,'state':'move'},"
		"{'type':'touch','x':178,'y':265,'state':'move'},"
		"{'type':'touch','x':184,'y':241,'state':'move'},"
		"{'type':'touch','x':188,'y':209,'state':'move'},"
		"{'type':'touch','x':190,'y':186,'state':'move'},"
		"{'type':'touch','x':191,'y':171,'state':'move'},"
		"{'type':'touch','x':190,'y':165,'state':'move'},"
		"{'type':'touch','x':190,'y':163,'state':'move'},"
		"{'type':'touch','x':190,'y':150,'state':'move'},"
		"{'type':'touch','x':190,'y':136,'state':'move'},"
		"{'type':'touch','x':188,'y':128,'state':'move'},"
		"{'type':'touch','x':188,'y':124,'state':'move'},"
		"{'type':'touch','x':188,'y':120,'state':'move'},"
		"{'type':'touch','x':186,'y':120,'state':'move'},"
		"{'type':'touch','x':186,'y':120,'state':'up'},"
		"{'type':'touch','x':186,'y':120,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':207,'y':114,'state':'move'},"
		"{'type':'touch','x':223,'y':111,'state':'move'},"
		"{'type':'touch','x':226,'y':108,'state':'move'},"
		"{'type':'touch','x':223,'y':106,'state':'move'},"
		"{'type':'touch','x':223,'y':105,'state':'down'},"
		"{'type':'touch','x':223,'y':105,'state':'up'},"
		"{'type':'touch','x':229,'y':103,'state':'move'},"
		"{'type':'touch','x':235,'y':102,'state':'move'},"
		"{'type':'touch','x':241,'y':108,'state':'move'},"
		"{'type':'touch','x':243,'y':131,'state':'move'},"
		"{'type':'touch','x':229,'y':181,'state':'move'},"
		"{'type':'touch','x':225,'y':210,'state':'move'},"
		"{'type':'touch','x':225,'y':213,'state':'move'},"
		"{'type':'touch','x':235,'y':217,'state':'move'},"
		"{'type':'touch','x':246,'y':219,'state':'move'},"
		"{'type':'touch','x':255,'y':219,'state':'move'},"
		"{'type':'touch','x':259,'y':219,'state':'move'},"
		"{'type':'touch','x':263,'y':222,'state':'move'},"
		"{'type':'touch','x':266,'y':225,'state':'move'},"
		"{'type':'touch','x':268,'y':228,'state':'move'},"
		"{'type':'touch','x':270,'y':230,'state':'move'},"
		"{'type':'touch','x':270,'y':230,'state':'down'},"
		"{'type':'touch','x':270,'y':230,'state':'up'},"
		"{'type':'touch','x':224,'y':222,'state':'move'},"
		"{'type':'touch','x':182,'y':213,'state':'move'},"
		"{'type':'touch','x':171,'y':211,'state':'move'},"
		"{'type':'touch','x':163,'y':206,'state':'move'},"
		"{'type':'touch','x':150,'y':195,'state':'move'},"
		"{'type':'touch','x':144,'y':187,'state':'move'},"
		"{'type':'touch','x':137,'y':178,'state':'move'},"
		"{'type':'touch','x':135,'y':170,'state':'move'},"
		"{'type':'touch','x':135,'y':166,'state':'move'},"
		"{'type':'touch','x':134,'y':160,'state':'move'},"
		"{'type':'touch','x':134,'y':157,'state':'down'},"
		"{'type':'touch','x':134,'y':157,'state':'up'},"
		"{'type':'touch','x':134,'y':154,'state':'move'},"
		"{'type':'touch','x':134,'y':135,'state':'move'},"
		"{'type':'touch','x':130,'y': 93,'state':'move'},"
		"{'type':'touch','x':128,'y': 83,'state':'move'},"
		"{'type':'touch','x':127,'y': 77,'state':'move'},"
		"{'type':'touch','x':124,'y': 67,'state':'move'},"
		"{'type':'touch','x':122,'y': 58,'state':'move'},"
		"{'type':'touch','x':121,'y': 53,'state':'move'},"
		"{'type':'touch','x':121,'y': 48,'state':'move'},"
		"{'type':'touch','x':121,'y': 47,'state':'down'},"
		"{'type':'touch','x':121,'y': 47,'state':'up'},"
		"{'type':'touch','x':121,'y': 76,'state':'move'},"
		"{'type':'touch','x': 76,'y':222,'state':'move'},"
		"{'type':'touch','x': 22,'y':348,'state':'move'},"
		"{'type':'touch','x': 14,'y':378,'state':'move'},"
		"{'type':'touch','x': 14,'y':377,'state':'move'},"
		"{'type':'touch','x': 18,'y':364,'state':'move'},"
		"{'type':'touch','x': 52,'y':311,'state':'move'},"
		"{'type':'touch','x': 77,'y':277,'state':'move'},"
		"{'type':'touch','x': 81,'y':270,'state':'move'},"
		"{'type':'touch','x': 80,'y':268,'state':'move'},"
		"{'type':'touch','x': 75,'y':266,'state':'move'},"
		"{'type':'touch','x': 69,'y':265,'state':'move'},"
		"{'type':'touch','x': 65,'y':265,'state':'move'},"
		"{'type':'touch','x': 65,'y':265,'state':'down'},"
		"{'type':'touch','x': 65,'y':265,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':111,'y':300,'state':'move'},"
		"{'type':'touch','x':144,'y':331,'state':'move'},"
		"{'type':'touch','x':172,'y':378,'state':'move'},"
		"{'type':'touch','x':175,'y':389,'state':'move'},"
		"{'type':'touch','x':175,'y':397,'state':'move'},"
		"{'type':'touch','x':181,'y':399,'state':'move'},"
		"{'type':'touch','x':211,'y':422,'state':'move'},"
		"{'type':'touch','x':255,'y':465,'state':'move'},"
		"{'type':'touch','x':270,'y':483,'state':'move'},"
	"]");

	UIManager::notifier()->play_script(script, "test/out/slider2_%d.svg");
}

class TestSlider
{
public:

	TestSlider(Label * label)
	{
		m_label = label;
	}
	
	void on_slide(Widget * widget, const SlideEvent & evt)
	{
		m_label->text("%d",evt.value());
	}

	Label * m_label = 0;
};


void Slider::test3()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(43,37);
		window.size(240,100);

	Grid * main_layout = new Grid(&window);
	int row = 0;
	Slider * slider = new Slider(main_layout);
		slider->cell(row++,0);
		slider->align(Align::ALIGN_BOTTOM);

	
	Label * label = new Label(main_layout);
		label->cell(row++,0);
		label->align(Align::ALIGN_TOP);
		label->text("label");

	TestSlider test_slider(label);
		slider->bind(&test_slider, &TestSlider::on_slide);

	//UIManager::desktop()->mainloop();
	
	String script(
	"["
		"{'type':'touch','x':169,'y':  2,'state':'move'},"
		"{'type':'touch','x':201,'y':  7,'state':'move'},"
		"{'type':'touch','x':203,'y':  4,'state':'move'},"
		"{'type':'touch','x':202,'y':  0,'state':'move'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':233,'y': 29,'state':'move'},"
		"{'type':'touch','x':243,'y': 46,'state':'move'},"
		"{'type':'touch','x':247,'y': 53,'state':'move'},"
		"{'type':'touch','x':251,'y': 58,'state':'move'},"
		"{'type':'touch','x':261,'y': 70,'state':'move'},"
		"{'type':'touch','x':268,'y': 78,'state':'move'},"
		"{'type':'touch','x':271,'y': 82,'state':'move'},"
		"{'type':'touch','x':272,'y': 85,'state':'move'},"
		"{'type':'touch','x':271,'y': 82,'state':'move'},"
		"{'type':'touch','x':271,'y': 81,'state':'down'},"
		"{'type':'touch','x':265,'y': 81,'state':'move'},"
		"{'type':'touch','x':244,'y': 81,'state':'move'},"
		"{'type':'touch','x':223,'y': 81,'state':'move'},"
		"{'type':'touch','x':211,'y': 79,'state':'move'},"
		"{'type':'touch','x':203,'y': 79,'state':'move'},"
		"{'type':'touch','x':196,'y': 79,'state':'move'},"
		"{'type':'touch','x':192,'y': 79,'state':'move'},"
		"{'type':'touch','x':193,'y': 79,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':189,'y': 79,'state':'move'},"
		"{'type':'touch','x':176,'y': 76,'state':'move'},"
		"{'type':'touch','x':152,'y': 73,'state':'move'},"
		"{'type':'touch','x':142,'y': 72,'state':'move'},"
		"{'type':'touch','x':148,'y': 73,'state':'move'},"
		"{'type':'touch','x':159,'y': 74,'state':'move'},"
		"{'type':'touch','x':172,'y': 75,'state':'move'},"
		"{'type':'touch','x':178,'y': 75,'state':'move'},"
		"{'type':'touch','x':182,'y': 76,'state':'move'},"
		"{'type':'touch','x':186,'y': 76,'state':'move'},"
		"{'type':'touch','x':187,'y': 76,'state':'down'},"
		"{'type':'touch','x':181,'y': 76,'state':'move'},"
		"{'type':'touch','x':152,'y': 76,'state':'move'},"
		"{'type':'touch','x': 92,'y': 76,'state':'move'},"
		"{'type':'touch','x': 68,'y': 76,'state':'move'},"
		"{'type':'touch','x': 67,'y': 76,'state':'move'},"
		"{'type':'touch','x': 71,'y': 76,'state':'move'},"
		"{'type':'touch','x': 76,'y': 76,'state':'move'},"
		"{'type':'touch','x': 78,'y': 76,'state':'move'},"
		"{'type':'touch','x': 78,'y': 76,'state':'up'},"
		"{'type':'touch','x': 78,'y': 76,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x': 78,'y': 76,'state':'down'},"
		"{'type':'touch','x': 71,'y': 76,'state':'move'},"
		"{'type':'touch','x': 56,'y': 76,'state':'move'},"
		"{'type':'touch','x': 43,'y': 76,'state':'move'},"
		"{'type':'touch','x': 37,'y': 76,'state':'move'},"
		"{'type':'touch','x': 33,'y': 76,'state':'move'},"
		"{'type':'touch','x': 29,'y': 76,'state':'move'},"
		"{'type':'touch','x': 29,'y': 76,'state':'move'},"
		"{'type':'touch','x': 29,'y': 76,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x': 34,'y': 75,'state':'move'},"
		"{'type':'touch','x': 44,'y': 74,'state':'move'},"
		"{'type':'touch','x': 52,'y': 74,'state':'move'},"
		"{'type':'touch','x': 54,'y': 74,'state':'down'},"
		"{'type':'touch','x': 59,'y': 77,'state':'move'},"
		"{'type':'touch','x': 77,'y': 98,'state':'move'},"
		"{'type':'touch','x':112,'y':146,'state':'move'},"
		"{'type':'touch','x':155,'y':188,'state':'move'},"
		"{'type':'touch','x':190,'y':212,'state':'move'},"
		"{'type':'touch','x':212,'y':229,'state':'move'},"
		"{'type':'touch','x':232,'y':242,'state':'move'},"
		"{'type':'touch','x':245,'y':243,'state':'move'},"
		"{'type':'touch','x':236,'y':231,'state':'move'},"
		"{'type':'touch','x':155,'y':151,'state':'move'},"
		"{'type':'touch','x':102,'y': 99,'state':'move'},"
		"{'type':'touch','x': 87,'y': 83,'state':'move'},"
		"{'type':'touch','x': 83,'y': 80,'state':'up'},"
		"{'type':'touch','x': 80,'y': 80,'state':'move'},"
		"{'type':'touch','x': 74,'y': 80,'state':'move'},"
		"{'type':'touch','x': 67,'y': 81,'state':'move'},"
		"{'type':'touch','x': 61,'y': 80,'state':'move'},"
		"{'type':'touch','x': 58,'y': 76,'state':'move'},"
		"{'type':'touch','x': 56,'y': 74,'state':'move'},"
		"{'type':'touch','x': 56,'y': 74,'state':'down'},"
		"{'type':'touch','x': 63,'y': 74,'state':'move'},"
		"{'type':'touch','x': 75,'y': 74,'state':'move'},"
		"{'type':'touch','x': 91,'y': 74,'state':'move'},"
		"{'type':'touch','x':100,'y': 74,'state':'move'},"
		"{'type':'touch','x':104,'y': 78,'state':'move'},"
		"{'type':'touch','x':119,'y': 85,'state':'move'},"
		"{'type':'touch','x':132,'y': 88,'state':'move'},"
		"{'type':'touch','x':133,'y': 94,'state':'move'},"
		"{'type':'touch','x':138,'y':111,'state':'move'},"
		"{'type':'touch','x':160,'y':133,'state':'move'},"
		"{'type':'touch','x':181,'y':153,'state':'move'},"
		"{'type':'touch','x':198,'y':179,'state':'move'},"
		"{'type':'touch','x':217,'y':203,'state':'move'},"
		"{'type':'touch','x':232,'y':211,'state':'move'},"
		"{'type':'touch','x':237,'y':211,'state':'move'},"
		"{'type':'touch','x':240,'y':213,'state':'move'},"
		"{'type':'touch','x':244,'y':213,'state':'move'},"
		"{'type':'touch','x':247,'y':214,'state':'move'},"
		"{'type':'touch','x':254,'y':214,'state':'move'},"
		"{'type':'touch','x':260,'y':215,'state':'move'},"
		"{'type':'touch','x':265,'y':216,'state':'move'},"
		"{'type':'touch','x':271,'y':217,'state':'move'},"
		"{'type':'touch','x':275,'y':217,'state':'move'},"
		"{'type':'touch','x':280,'y':218,'state':'move'},"
		"{'type':'touch','x':283,'y':218,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':288,'y':227,'state':'move'},"
		"{'type':'touch','x':339,'y':345,'state':'move'},"

	"]");

	UIManager::notifier()->play_script(script, "test/out/slider3_%d.svg");
}

void Slider::test4()
{
	Window window;

	Column * column = new Column(&window);

	Slider * horizontal[5];

	Column * layer_top = new Column(column);
	for (int i = 0; i < 5; i++)
	{
		horizontal[i] = new Slider(layer_top);
			horizontal[i]->extend(Extend::EXTEND_WIDTH);
			//horizontal[i]->value(45);
			horizontal[i]->radius(0);
			horizontal[i]->track_size(20);
			horizontal[i]->handle_size(14);
			horizontal[i]->thickness(i);
			horizontal[i]->track_color(Color::LIGHT_ORANGE);
			horizontal[i]->handle_color(Color::ORANGE);
			horizontal[i]->min_value(0);
			horizontal[i]->max_value(100);
			horizontal[i]->margin(i*2);
			horizontal[i]->value(i*25);
	}



	Row * layer_bottom = new Row(column);
	Slider * vertical[5];
	for (int i = 0; i < 5; i++)
	{
		vertical[i] = new Slider(layer_bottom);
			vertical[i]->extend(Extend::EXTEND_HEIGHT);
			//vertical[i]->value(45);
			vertical[i]->radius(0);
			vertical[i]->track_size(20);
			vertical[i]->handle_size(14);
			vertical[i]->thickness(i);
			vertical[i]->track_color(Color::LIGHT_ORANGE);
			vertical[i]->handle_color(Color::ORANGE);
			vertical[i]->min_value(0);
			vertical[i]->max_value(100);
			vertical[i]->margin(i*2);
			vertical[i]->value(i*25);
	}

	UIManager::desktop()->dispatch("test/out/slider4_1.svg");

	for (int i = 0; i < 5; i++)
	{
		horizontal[i]->track_size(4);
		horizontal[i]->handle_size(20);
		vertical[i]->track_size(4);
		vertical[i]->handle_size(20);
	}
	UIManager::desktop()->dispatch("test/out/slider4_2.svg");
}

void Slider::test5()
{
}

void Slider::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
#endif
