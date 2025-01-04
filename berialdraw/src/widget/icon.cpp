#include "berialdraw_imp.hpp"

using namespace berialdraw;

Icon::Icon(Widget * parent):
	Widget("icon", parent)
{
	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);
	UIManager::styles()->apply(m_classname, (BorderStyle*)this);
	UIManager::styles()->apply(m_classname, (IconStyle*)this);
	UIManager::styles()->apply(m_classname, (TextStyle*)this);
}

Icon::~Icon()
{
}

/** Return the icon size */
Size Icon::icon_size()
{
	Size result;
	result.set_(compute_zoom(m_resolution.width_(), m_zoom), compute_zoom(m_resolution.height_(), m_zoom));
	result.height_(result.height_() + icon_padding().bottom_() + icon_padding().top_());
	result.width_ (result.width_()  + icon_padding().left_()   + icon_padding().right_());
	return result;
}

/** Return the size of content without marges */
Size Icon::content_size()
{
	if (load())
	{
		if (m_size.is_height_undefined())
		{
			m_size.height_(m_size.height_());
		}
		if (m_size.is_width_undefined())
		{
			m_size.width_(m_size.width_());
		}
	}
	Size result = icon_size();

	if(m_text_modified || m_font_modified)
	{
		Area area;
		select_font();
		m_text_box.parse(area, *m_font, m_text, UINT32_MAX, UINT32_MAX, UINT32_MAX, (Align)m_text_align);
		m_text_modified = m_font_modified = 0;
		m_text_size = m_text_box.content_size();
	}
	
	// Add text height in the icon content size
	if (m_text_size.width_() > 0 && m_text_size.height_() > 0)
	{
		result.height_(result.height_()+ padding().top_() + m_text_size.height_() + padding().bottom_());
	}
	return result;
}

void Icon::place(const Area & area, bool in_layout)
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
		place_in_layout(backclip, content_size(), marg, EXTEND_NONE, m_foreclip, (Align)m_align);
	}

	Size ico_size = icon_size();
		m_icon_foreclip = m_foreclip;
		m_icon_foreclip.height_(ico_size.height_());

	m_text_backclip.position().set_(m_foreclip.x_(), m_foreclip.y_()+ico_size.height_());
	m_text_backclip.size().set_(m_foreclip.width_(), m_foreclip.height_() - ico_size.height_());
	m_text_foreclip = m_text_backclip;

	Area backclip(m_backclip);
	Margin marg;
	place_in_layout(m_text_backclip, m_text_size, marg, EXTEND_NONE, m_text_foreclip, (Align)m_text_align);

	// Restore backclip
	m_backclip = backclip;
}

void Icon::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);
	UIManager::renderer()->region(region);
	Point shift;

	// Paint background
	Rect::build_polygon(m_icon_foreclip, shift, m_radius, m_thickness, 0, m_sides, pressed_color(color(),pressed()), border_color());

	// If button with focus
	if (m_focused)
	{
		// Display focus
		Rect::build_polygon(m_icon_foreclip, shift, m_radius+ (m_thickness>>1), m_focus_thickness<<6, m_focus_gap, m_sides, Color::TRANSPARENT, focus_color());
	}

	// If icon existing
	if (m_paths.size() > 0)
	{
		Canvas canvas(this);
		canvas.position(m_foreclip.position());
		canvas.color(0);
		Point shift(m_foreclip.position());
		shift.move_(icon_padding().right_(), icon_padding().top_());

		uint32_t custom_color = Color::TRANSPARENT;
		bool custom_color_set = false;
		bool one_color = true;

		// Check if icon has only one color
		for (uint32_t i = 1; i < m_paths.size(); i++)
		{
			Path * path = m_paths[i];
			Path * path_prev = m_paths[i-1];
			
			if (path && path_prev)
			{
				if (path->color() != path_prev->color())
				{
					one_color = false;
					break;
				}
			}
		}

		// Display icon
		for (uint32_t i = 0; i < m_paths.size(); i++)
		{
			Path * path = m_paths[i];

			if (path)
			{
				Polygon polygon(&canvas);

				// If icon color can be changed
				if (icon_color() != Color::TRANSPARENT && one_color == true)
				{
					// Change icon color
					polygon.color(icon_color());
				}
				else
				{
					// Used icon file color
					polygon.color(path->color());
				}
				
				// Apply icon zoom
				polygon.zoom_(m_zoom);
				
				// Parse icon path
				VectorsScript vectors_script(polygon);
				vectors_script.select(path->path());
				vectors_script.parse();

				// Paint icon path
				polygon.paint(shift);
			}
		}
	}

	// Paint text
	region.intersect(m_text_backclip);
	select_font();
	UIManager::renderer()->region(region);
	m_text_box.paint(shift, *m_font.get(), m_text, m_text_foreclip.position(), m_text_backclip, text_color(), 0, 0, true);

	Widget::paint(region);
}

/** Get the widget hovered */
Widget * Icon::hovered(const Region & parent_region, const Point & position)
{
	Region region(parent_region);
	region.intersect(m_foreclip);

	// If the widget hovered
	if(region.is_inside(position))
	{
		return this;
	}
	return 0;
}

/** Serialize the content of widget into json */
void Icon::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	BorderStyle::serialize(it);
	IconStyle::serialize(it);
	TextStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Icon::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	BorderStyle::unserialize(it);
	IconStyle::unserialize(it);
	TextStyle::unserialize(it);
}

/** Indicates if the window must be refreshed */
bool Icon::dirty()
{
	return 
		UIManager::invalidator()->is_dirty(this) || 
		WidgetStyle::is_dirty() || 
		BorderStyle::is_dirty() || 
		CommonStyle::is_dirty() || 
		IconStyle::is_dirty()   || 
		TextStyle::is_dirty();
}

#ifdef _DEBUG

void Icon::test1()
{
	//UIManager::notifier()->log();
	Window window;
		window.size(480,480);

	Directory directory;
		directory.open("resources/icons");

	if (directory.first())
	{
		ScrollView * scroll = new ScrollView(&window);
			scroll->align(Align::ALIGN_TOP);
			scroll->extend(Extend::EXTEND_HEIGHT);
			scroll->margin(10);

		Row * main = new Row(scroll);
			main->flow(true);

		int angle = 0;
		do
		{
			if (directory.match("*.icn"))
			{
				Icon * icon = new Icon(main);
					size_t s = sizeof(*icon);
					icon->filename(directory.full_path());
					icon->zoom(4);
					icon->radius(16);
					icon->icon_padding(10);
					icon->margin(5);
					icon->color(Hsl::to_rgb(angle, 50, 90));
					icon->icon_color(Hsl::to_rgb(angle, 100, 25));
					icon->text(directory.filename());
				angle += 30;
			}
		}
		while(directory.next());
	}

	String script(
	"["
		"{'type':'touch','x':293,'y':316,'state':'down'},"
		"{'type':'touch','x':293,'y':316,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':166,'y':183,'state':'down'},"
		"{'type':'touch','x':166,'y':183,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':410,'y':436,'state':'down'},"
		"{'type':'touch','x':406,'y':409,'state':'move'},"
		"{'type':'touch','x':386,'y':256,'state':'move'},"
		"{'type':'touch','x':384,'y':140,'state':'move'},"
		"{'type':'touch','x':393,'y': 61,'state':'move'},"
		"{'type':'touch','x':379,'y': 38,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':302,'y':458,'state':'down'},"
		"{'type':'touch','x':303,'y':339,'state':'move'},"
		"{'type':'touch','x':311,'y':275,'state':'move'},"
		"{'type':'touch','x':311,'y':261,'state':'move'},"
		"{'type':'touch','x':309,'y':249,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
	"]");

	//UIManager::desktop()->mainloop();
	UIManager::notifier()->play_script(script, "test/out/icon1_%d.svg");
}

void Icon::test2()
{
	//UIManager::notifier()->log();
	Window window;
		window.size(480,480);

	ScrollView * scroll = new ScrollView(&window);
		scroll->align(Align::ALIGN_TOP);
		scroll->extend(Extend::EXTEND_HEIGHT);
		scroll->margin(10);

	Row * main = new Row(scroll);
		main->flow(true);

	int angle = 0;
	for (Directory directory("resources/icons"); directory.exist(); directory.next())
	{
		if (directory.match("*.icn"))
		{
			Icon * icon = new Icon(main);
				size_t s = sizeof(*icon);
				icon->filename(directory.full_path());
				icon->zoom(5);
				icon->radius(0);
				icon->icon_padding(0);
				icon->margin(2);
				icon->padding(0);
				icon->color(Hsl::to_rgb(angle, 50, 90));
				icon->icon_color(Hsl::to_rgb(angle, 100, 25));
				icon->text("");
			angle += 30;
		}
	}
	UIManager::desktop()->dispatch("test/out/icon2_0.svg");
}

void Icon::test3()
{
	Window window;
		window.size(480,480);

	ScrollView * scroll = new ScrollView(&window);
		scroll->align(Align::ALIGN_TOP);
		scroll->extend(Extend::EXTEND_HEIGHT);
		scroll->margin(10);

	Row * main = new Row(scroll);
		main->flow(true);

	int angle = 0;
	for (Directory directory("resources/icons"); directory.exist(); directory.next())
	{
		if (directory.match("*.icn"))
		{
			Icon * icon = new Icon(main);
				icon->filename(directory.full_path());
				icon->zoom(5);
				icon->radius(100);
				icon->icon_padding(10);
				icon->margin(2);
				icon->padding(0);
				icon->color(Hsl::to_rgb(angle, 50, 90));
				icon->icon_color(Hsl::to_rgb(angle, 100, 25));
				icon->text("");
			angle += 30;
		}
	}

	//UIManager::desktop()->mainloop();
	UIManager::desktop()->dispatch("test/out/icon3_0.svg");
}

void Icon::test4()
{
}

void Icon::test()
{
	test4();
	test3();
	test2();
	test1();
}
#endif