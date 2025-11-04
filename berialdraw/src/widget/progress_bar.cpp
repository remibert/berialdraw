#include "berialdraw_imp.hpp"

using namespace berialdraw;

ProgressBar::ProgressBar(Widget * parent):
	Widget("progress_bar", parent, sizeof(ProgressBar))
{
	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);
	UIManager::styles()->apply(m_classname, (BorderStyle*)this);
	UIManager::styles()->apply(m_classname, (ProgressBarStyle*)this);
}

ProgressBar::~ProgressBar()
{
}

/** Copy all styles of the progress_bar */
void ProgressBar::copy(const ProgressBar & progress_bar)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&progress_bar);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&progress_bar);
	*((BorderStyle*)this) = *(BorderStyle*)(&progress_bar);
	*((ProgressBarStyle*)this) = *(ProgressBarStyle*)(&progress_bar);
}

/** Copy all styles of the progress_bar */
void ProgressBar::copy(const ProgressBar * progress_bar)
{
	if(progress_bar)
	{
		copy(*progress_bar);
	}
}
/** Return the size of content without marges */
Size ProgressBar::content_size()
{
	Size size;
	if (m_fill_size > m_track_size)
	{
		size.set_(m_fill_size,m_fill_size);
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

void ProgressBar::place(const Area & area, bool in_layout)
{
	if (m_size.is_width_undefined() && m_size.is_height_undefined() && m_extend != Extend::EXTEND_NONE && 
		m_position.is_x_undefined() && m_position.is_y_undefined())
	{
		in_layout = true;
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

Dim ProgressBar::get_bar_length(Dim length)
{
	Dim result = 0;
	check_progress_bar();
	Dim value = m_value;

	if ((m_max_value - m_min_value) > 0)
	{
		if (m_step_value > 0)
		{
			Dim delta = value % m_step_value;

			if (delta > 0)
			{
				if (delta > m_step_value>>1)
				{
					value += (m_step_value - delta);
				}
				else
				{
					value -= delta;
				}
			}
		}
		result = (value * length) / (m_max_value - m_min_value);
	}
	return result;
}

void ProgressBar::draw_track()
{
	Coord x;
	Coord y;
	Dim width;
	Dim height;

	Dim      track_size = (m_track_size>>6) % 2 ? m_track_size + 64 : m_track_size;
	Dim      track_radius = m_radius;
	uint32_t track_border_color;
	Dim      track_thickness;
	Area     track_area(m_foreclip);

	Dim      fill_size  = (m_fill_size >>6) % 2 ? m_fill_size  + 64 : m_fill_size;
	Dim      fill_radius = m_radius;
	Dim      fill_thickness;
	uint32_t fill_border_color;
	Area     fill_area(m_foreclip);

	Dim half_thickness = ((m_thickness>>7)<<6);

	// If the track is thicker than fill
	if (track_size >= fill_size)
	{
		Dim radius_dif = (track_size-fill_size) >> 1;
		
		track_thickness = m_thickness;
		fill_thickness = 0;
		
		if (radius_dif <= m_radius)
		{
			fill_radius -= radius_dif;
		}
		fill_border_color = Color::TRANSPARENT;
		track_border_color = stated_color(m_border_color);
		
		// Vertical progress bar
		if (m_extend == Extend::EXTEND_HEIGHT)
		{
			width  = substract(fill_size,half_thickness+half_thickness);
			height = get_bar_length(substract(m_foreclip.height_(), radius_dif + radius_dif + half_thickness + half_thickness));
			x      = m_foreclip.x_() + radius_dif + half_thickness;

			// Align the fill in track
			if ((m_align & Align::ALIGN_VERTICAL) == Align::ALIGN_TOP)
			{
				y = m_foreclip.y_() + radius_dif + half_thickness;
			}
			else
			{
				y = m_foreclip.y_() + substract(m_foreclip.height_(), (height+radius_dif+half_thickness));
			}
		}
		// Horizontal progress bar
		else
		{
			height  = substract(fill_size,half_thickness+half_thickness);
			width  = get_bar_length(substract(m_foreclip.width_(), radius_dif + radius_dif + half_thickness + half_thickness));
			y      = m_foreclip.y_() + radius_dif + half_thickness;

			// Align the fill in track
			if ((m_align & Align::ALIGN_HORIZONTAL) == Align::ALIGN_LEFT)
			{
				x = m_foreclip.x_() + radius_dif + half_thickness;
			}
			else
			{
				x = m_foreclip.x_() + substract(m_foreclip.width_(), (width+radius_dif+half_thickness));
			}
		}
	}
	// If the track is thinner than fill
	else if (track_size < fill_size)
	{
		Dim radius_dif = (fill_size-track_size)>>1;

		fill_thickness = m_thickness;
		track_thickness = 0;

		if (radius_dif <= m_radius)
		{
			track_radius -= radius_dif;
		}
		track_border_color = Color::TRANSPARENT;
		fill_border_color = stated_color(m_border_color);

		// Vertical progress bar
		if (m_extend == Extend::EXTEND_HEIGHT)
		{
			track_area.width_(track_size);
			track_area.x_(m_foreclip.x_()+radius_dif);

			width = fill_size;
			height = get_bar_length(m_foreclip.height_());
			x      = m_foreclip.x_();

			// Align the fill in track
			if ((m_align & Align::ALIGN_VERTICAL) == Align::ALIGN_TOP)
			{
				y = m_foreclip.y_();
			}
			else
			{
				y = m_foreclip.y_()+ substract(m_foreclip.height_(),height);
			}
		}
		// Horizontal progress bar
		else
		{
			track_area.height_(track_size);
			track_area.y_(m_foreclip.y_()+radius_dif);
			
			width = get_bar_length(m_foreclip.width_());
			height = fill_size;
			y     = m_foreclip.y_();

			// Align the fill in track
			if ((m_align & Align::ALIGN_HORIZONTAL) == Align::ALIGN_LEFT)
			{
				x = m_foreclip.x_();
			}
			else
			{
				x = m_foreclip.x_() + substract(m_foreclip.width_(),width);
			}
		}
	}
	fill_area.position().set_(x,y);
	fill_area.size().set_(width,height);

	// Draw track
	track_area.nearest_pixel();
	Rect::build_polygon(track_area, track_radius, track_thickness, 0, m_borders, stated_color(m_track_color), track_border_color);

	// Draw fill
	fill_area.nearest_pixel();
	Rect::build_polygon(fill_area, fill_radius, fill_thickness, 0, m_borders, stated_color(m_fill_color), fill_border_color);
}

void ProgressBar::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);
		check_progress_bar();
		draw_track();
	}
}

/** Get the widget hovered */
Widget * ProgressBar::hovered(const Region & parent_region, const Point & position)
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
void ProgressBar::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	BorderStyle::serialize(it);
	ProgressBarStyle::serialize(it);

}

/** Unserialize the content of widget from json */
void ProgressBar::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	BorderStyle::unserialize(it);
	ProgressBarStyle::unserialize(it);
}

#ifdef _DEBUG

void ProgressBar::test1()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(13,7);
		window.size(400,200);

	Row * main = new Row(&window);
		Column * column_layout = new Column(main);
			ProgressBar * progress_bar_1 = new ProgressBar(column_layout);
				progress_bar_1->min_value(0);
				progress_bar_1->max_value(100);
				progress_bar_1->step_value(5);	
				progress_bar_1->id(1);
				progress_bar_1->border_color(Color::BLUE,64);
				progress_bar_1->align(Align::ALIGN_LEFT);

			ProgressBar * progress_bar_2 = new ProgressBar(column_layout);
				progress_bar_2->min_value(0);
				progress_bar_2->max_value(100);
				progress_bar_2->step_value(5);	
				progress_bar_2->id(2);
				progress_bar_2->border_color(Color::BLUE,64);
				progress_bar_2->align(Align::ALIGN_RIGHT);

		Row * row_layout = new Row(main);
			ProgressBar * progress_bar_3 = new ProgressBar(row_layout);
				progress_bar_3->min_value(0);
				progress_bar_3->max_value(100);
				progress_bar_3->step_value(5);	
				progress_bar_3->id(3);
				progress_bar_3->border_color(Color::BLUE,64);
				progress_bar_3->extend(Extend::EXTEND_HEIGHT);
				progress_bar_3->align(Align::ALIGN_BOTTOM);

			ProgressBar * progress_bar_4 = new ProgressBar(row_layout);
				progress_bar_4->min_value(0);
				progress_bar_4->max_value(100);
				progress_bar_4->step_value(5);	
				progress_bar_4->id(4);
				progress_bar_4->border_color(Color::BLUE,64);
				progress_bar_4->align(Align::ALIGN_TOP);
				progress_bar_4->extend(Extend::EXTEND_HEIGHT);

	int id = 0;
	for (int32_t value = progress_bar_1->min_value(); value <= progress_bar_1->max_value(); value ++)
	{
		progress_bar_1->value(value);
		progress_bar_1->thickness(value%5);
		progress_bar_2->value(value);
		progress_bar_2->thickness(value%5);
		progress_bar_3->value(value);
		progress_bar_3->thickness(value%5);
		progress_bar_4->value(value);
		progress_bar_4->thickness(value%5);

		if (value % 13 == 0)
		{
			String name;
			name.print("test/out/progress_bar1_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}

	UIManager::desktop()->dispatch();
}

void ProgressBar::test2()
{
	Window window;
		window.position(13,7);
		window.size(400,200);

	Row * main = new Row(&window);
		Column * column_layout = new Column(main);
			ProgressBar * progress_bar_1 = new ProgressBar(column_layout);
				progress_bar_1->min_value(0);
				progress_bar_1->max_value(100);
				progress_bar_1->step_value(5);	
				progress_bar_1->id(1);
				progress_bar_1->border_color(Color::BLUE,64);
				progress_bar_1->align(Align::ALIGN_LEFT);
				progress_bar_1->fill_size(20);
				progress_bar_1->radius(5);
				progress_bar_1->track_size(2);
				progress_bar_1->thickness(0);


			ProgressBar * progress_bar_2 = new ProgressBar(column_layout);
				progress_bar_2->min_value(0);
				progress_bar_2->max_value(100);
				progress_bar_2->step_value(5);	
				progress_bar_2->id(2);
				progress_bar_2->border_color(Color::BLUE,64);
				progress_bar_2->align(Align::ALIGN_RIGHT);
				progress_bar_2->fill_size(10);
				progress_bar_2->radius(5);
				progress_bar_2->track_size(2);
				progress_bar_2->thickness(0);


		Row * row_layout = new Row(main);
			ProgressBar * progress_bar_3 = new ProgressBar(row_layout);
				progress_bar_3->min_value(0);
				progress_bar_3->max_value(100);
				progress_bar_3->step_value(5);	
				progress_bar_3->id(3);
				progress_bar_3->border_color(Color::BLUE,64);
				progress_bar_3->extend(Extend::EXTEND_HEIGHT);
				progress_bar_3->align(Align::ALIGN_BOTTOM);
				progress_bar_3->fill_size(10);
				progress_bar_3->radius(5);
				progress_bar_3->track_size(2);
				progress_bar_3->thickness(0);

			ProgressBar * progress_bar_4 = new ProgressBar(row_layout);
				progress_bar_4->min_value(0);
				progress_bar_4->max_value(100);
				progress_bar_4->step_value(5);	
				progress_bar_4->id(4);
				progress_bar_4->border_color(Color::BLUE,64);
				progress_bar_4->align(Align::ALIGN_TOP);
				progress_bar_4->extend(Extend::EXTEND_HEIGHT);
				progress_bar_4->fill_size(10);
				progress_bar_4->radius(5);
				progress_bar_4->track_size(6);
				progress_bar_4->thickness(1);

	int id = 0;
	for (int32_t value = progress_bar_1->min_value(); value <= progress_bar_1->max_value(); value ++)
	{
		progress_bar_1->value(value);
		progress_bar_1->thickness(value%5);
		progress_bar_2->value(value);
		progress_bar_2->thickness(value%5);
		progress_bar_3->value(value);
		progress_bar_3->thickness(value%5);
		progress_bar_4->value(value);
		progress_bar_4->thickness(value%5);

		if (value % 13 == 0)
		{
			String name;
			name.print("test/out/progress_bar2_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}
}

void ProgressBar::test3()
{
	Window window;
		window.position(13,7);
		window.size(400,200);

	Row * main = new Row(&window);
		Column * column_layout = new Column(main);
			ProgressBar * progress_bar_1 = new ProgressBar(column_layout);
				progress_bar_1->min_value(0);
				progress_bar_1->max_value(100);
				progress_bar_1->step_value(5);	
				progress_bar_1->id(1);
				progress_bar_1->border_color(Color::BLUE,64);
				progress_bar_1->align(Align::ALIGN_LEFT);
				progress_bar_1->fill_size(10);
				progress_bar_1->radius(5);
				progress_bar_1->track_size(20);

			ProgressBar * progress_bar_2 = new ProgressBar(column_layout);
				progress_bar_2->min_value(0);
				progress_bar_2->max_value(100);
				progress_bar_2->step_value(5);	
				progress_bar_2->id(2);
				progress_bar_2->border_color(Color::BLUE,64);
				progress_bar_2->align(Align::ALIGN_RIGHT);
				progress_bar_2->fill_size(10);
				progress_bar_2->radius(5);
				progress_bar_2->track_size(20);

		Row * row_layout = new Row(main);
			ProgressBar * progress_bar_3 = new ProgressBar(row_layout);
				progress_bar_3->min_value(0);
				progress_bar_3->max_value(100);
				progress_bar_3->step_value(5);	
				progress_bar_3->id(3);
				progress_bar_3->border_color(Color::BLUE,64);
				progress_bar_3->extend(Extend::EXTEND_HEIGHT);
				progress_bar_3->align(Align::ALIGN_BOTTOM);
				progress_bar_3->fill_size(10);
				progress_bar_3->radius(5);
				progress_bar_3->track_size(20);

			ProgressBar * progress_bar_4 = new ProgressBar(row_layout);
				progress_bar_4->min_value(0);
				progress_bar_4->max_value(100);
				progress_bar_4->step_value(5);	
				progress_bar_4->id(4);
				progress_bar_4->border_color(Color::BLUE,64);
				progress_bar_4->align(Align::ALIGN_TOP);
				progress_bar_4->extend(Extend::EXTEND_HEIGHT);
				progress_bar_4->fill_size(10);
				progress_bar_4->radius(5);
				progress_bar_4->track_size(20);

	int id = 0;
	for (int32_t value = progress_bar_1->min_value(); value <= progress_bar_1->max_value(); value ++)
	{
		//value = 50;
		progress_bar_1->value(value);
		progress_bar_1->thickness(value%5);
		progress_bar_2->value(value);
		progress_bar_2->thickness(value%5);
		progress_bar_3->value(value);
		progress_bar_3->thickness(value%5);
		progress_bar_4->value(value);
		progress_bar_4->thickness(value%5);

		if (value % 13 == 0)
		{
			String name;
			name.print("test/out/progress_bar3_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}
}

void ProgressBar::test4()
{
}

void ProgressBar::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		test4();
		test3();
		test2();
		test1();
	}
}
#endif
