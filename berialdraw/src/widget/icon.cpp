#include "berialdraw_imp.hpp"

using namespace berialdraw;

Icon::Icon(Widget * parent):
	Widget("icon", parent, sizeof(Icon))
{
	m_sketch = new Sketch(this);
	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);
	UIManager::styles()->apply(m_classname, (BorderStyle*)this);
	UIManager::styles()->apply(m_classname, (IconStyle*)this);
	UIManager::styles()->apply(m_classname, (TextStyle*)this);
}

Icon::~Icon()
{
	delete m_sketch;
}

/** Copy all styles of the icon */
void Icon::copy(const Icon & icon)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&icon);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&icon);
	*((BorderStyle*)this) = *(BorderStyle*)(&icon);
	*((TextStyle*)this)   = *(TextStyle*)(&icon);
	*((IconStyle*)this)   = *(IconStyle*)(&icon);
}

/** Copy all styles of the icon */
void Icon::copy(const Icon * icon)
{
	if(icon)
	{
		copy(*icon);
	}
}

Dim Icon::compute_zoom(Dim value, Dim zoom)
{
	return (Dim)((((uint64_t)value * (uint64_t)zoom)) >> 6);
}


/** Return the icon size */
Size Icon::icon_size()
{
	if (m_icon_modified)
	{
		m_sketch->filename(m_filename);
		m_icon_modified = false;
		m_sketch->zoom_(m_zoom);
		if (m_sketch->load())
		{
			m_icon_size.set_(compute_zoom(m_sketch->resolution().width_(), zoom_()), compute_zoom(m_sketch->resolution().height_(), zoom_()));
			m_icon_size.height_(m_icon_size.height_() + icon_padding().bottom_() + icon_padding().top_());
			m_icon_size.width_ (m_icon_size.width_()  + icon_padding().left_()   + icon_padding().right_());

			// Compute the children with marged size
			Size children_size = Widget::children_size();
			if (children_size.width_() > m_icon_size.width_())
			{
				m_icon_size.width_(children_size.width_());
			}
			if (children_size.height_() > m_icon_size.height_())
			{
				m_icon_size.height_(children_size.height_());
			}
		}
		else
		{
			m_icon_size.set(0,0);
		}
	}
	return m_icon_size;
}

/** Return the size of content without marges */
Size Icon::content_size()
{
	Size result = icon_size();

	if(m_text_modified || m_font_modified)
	{
		Area area;
		select_font();
		if (m_font.get())
		{
			m_text_box.parse(area, *m_font, m_text, UINT32_MAX, UINT32_MAX, UINT32_MAX, (Align)m_text_align);
		}
		m_text_modified = m_font_modified = 0;
		m_text_size = m_text_box.content_size();
	}
	
	// Add text height in the icon content size
	if (m_text_size.width_() > 0 && m_text_size.height_() > 0)
	{
		//result.height_(result.height_()+ padding().top_() + m_text_size.height_() + padding().bottom_());
		result.increase_(0, padding().top_() + m_text_size.height_() + padding().bottom_());
	}
	return result;
}

void Icon::place(const Area & area, bool in_layout)
{
	Margin marg;

	if (m_size.is_width_undefined() && m_size.is_height_undefined() && m_extend != Extend::EXTEND_NONE && 
		m_position.is_x_undefined() && m_position.is_y_undefined())
	{
		in_layout = true;
	}

	// Place background rectangle
	place_in_area(area, in_layout);

	// Place button text
	m_text_backclip = m_foreclip;
	marg.bottom_(padding().bottom_());
	place_in_layout(m_text_backclip, m_text_size, marg, EXTEND_NONE, m_text_foreclip, (Align)(m_text_align | ALIGN_BOTTOM));
	
	// Place the icon
	m_icon_foreclip = m_foreclip;

	// Add text height in the icon content size
	if (m_text_size.width_() > 0 && m_text_size.height_() > 0)
	{
		m_icon_foreclip.size().decrease_(0,padding().top_() + m_text_size.height_() + padding().bottom_());
	}

	// Place all children
	Area backclip(m_backclip);
	Widget::place(m_foreclip,in_layout);

	// Restore backclip
	m_backclip = backclip;
}

void Icon::paint(const Region & parent_region)
{
	Region region(parent_region);

	// Draw rectangle
	region.intersect(m_backclip);

	// If button visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);
		Point shift;

		Rect::build_focused_polygon(m_icon_foreclip, 
			*(CommonStyle*)this,
			*(BorderStyle*)this,
			stated_color(m_color), 
			stated_color(m_border_color),
			Color::TRANSPARENT, 
			stated_color(m_focus_color),
			m_focused);

		// Paint children
		Widget::paint(region);

		// Paint icon
		m_sketch->size(m_icon_foreclip.size());
		m_sketch->paint(m_icon_foreclip, icon_padding(), stated_color(m_icon_color));

		// Paint text
		region.intersect(m_text_backclip);
		select_font();
		UIManager::renderer()->region(region);
		m_text_box.paint(shift, *m_font.get(), m_text, m_text_foreclip.position(), m_text_backclip, stated_color(m_text_color), 0, 0, true);
	}
}

/** Get the widget hovered */
Widget * Icon::hovered(const Region & parent_region, const Point & position)
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


#ifdef _DEBUG

void Icon::test1()
{
	//UIManager::notifier()->log();
	Window window;
		window.size(480,480);

	Directory directory;
		directory.open("$(ui.icons)");

	if (directory.first())
	{
		ScrollView * scroll = new ScrollView(&window);
			scroll->align(Align::ALIGN_TOP);
			scroll->scroll_direction(ScrollDirection::SCROLL_VERTICAL);
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
	UIManager::notifier()->play_script(script, "$(ui.tests)/out/icon1_%d.svg");
}

void Icon::test2()
{
	//UIManager::notifier()->log();
	Window window;
		window.size(480,480);

	ScrollView * scroll = new ScrollView(&window);
		scroll->align(Align::ALIGN_TOP);
		scroll->scroll_direction(ScrollDirection::SCROLL_VERTICAL);
		scroll->margin(10);

	Row * main = new Row(scroll);
		main->flow(true);

	int angle = 0;
	for (Directory directory("$(ui.icons)"); directory.exist(); directory.next())
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
	UIManager::desktop()->dispatch("$(ui.tests)/out/icon2_0.svg");
}

void Icon::test3()
{
	Window window;
		window.size(480,480);

	ScrollView * scroll = new ScrollView(&window);
		scroll->align(Align::ALIGN_TOP);
		scroll->scroll_direction(ScrollDirection::SCROLL_VERTICAL);
		scroll->margin(10);

	Row * main = new Row(scroll);
		main->flow(true);

	int angle = 0;
	for (Directory directory("$(ui.icons)"); directory.exist(); directory.next())
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
	UIManager::desktop()->dispatch("$(ui.tests)/out/icon3_0.svg");
}

void Icon::test4()
{
	Window window;
		Icon * icon = new Icon(&window);
			icon->filename("$(ui.icons)/maison.icn");
			icon->position(10,10);
			icon->text("Icon");

		Button * button = new Button(&window);
			button->position(200,200);
			button->text("Button");

	UIManager::desktop()->dispatch("$(ui.tests)/out/icon4_0.svg");
		icon->zoom(4);
		button->font_size(30,60);

	UIManager::desktop()->dispatch("$(ui.tests)/out/icon4_1.svg");
		button->size(150,200);
		icon->size(150,200);

	UIManager::desktop()->dispatch("$(ui.tests)/out/icon4_2.svg");
}

void Icon::test5()
{
	Window window;
		window.color(Color::WHITE);

	Icon * icon1 = new Icon(&window);
		icon1->position(100, 20);
		icon1->size(100, 50);
		icon1->text("Absolute position with text");
		icon1->text_color(Color::RED);
		icon1->font_size(16);
		icon1->color(Color::LIGHT_BLUE);
		icon1->border_color(Color::BLUE);
		icon1->thickness(4);
		icon1->filename("$(ui.icons)/maison.icn");

	Icon * icon2 = new Icon(&window);
		icon2->color(Color::LIGHT_GREEN);
		icon2->border_color(Color::GREEN);
		icon2->thickness(2);
		icon2->filename("$(ui.icons)/maison.icn");

	Icon * icon3 = new Icon(&window);
		icon3->position(20, 130);
		icon3->size(350, 50);
		icon3->text("Another absolute icon");
		icon3->text_color(Color::GREEN);
		icon3->font_size(14);
		icon3->color(Color::LIGHT_RED);
		icon3->border_color(Color::RED);
		icon3->thickness(3);
		icon3->filename("$(ui.icons)/maison.icn");

	Icon * icon4 = new Icon(&window);
		icon4->position(200, 190);
		icon4->zoom(4);
		icon4->color(Color::LIGHT_ORANGE);
		icon4->border_color(Color::ORANGE);
		icon4->thickness(0);
		icon4->filename("$(ui.icons)/maison.icn");

	Icon * icon5 = new Icon(&window);
		icon5->position(20, 190);
		icon5->min_size(100,100);
		icon5->color(Color::LIGHT_BROWN);
		icon5->border_color(Color::BROWN);
		icon5->thickness(0);
		icon5->filename("$(ui.icons)/maison.icn");

	//UIManager::desktop()->mainloop();
	UIManager::desktop()->dispatch("$(ui.tests)/out/icon5.svg");
}

void Icon::test()
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
