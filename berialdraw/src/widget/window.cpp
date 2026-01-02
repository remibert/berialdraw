#include "berialdraw_imp.hpp"

using namespace berialdraw;

Window::Window() : Widget("window",0, sizeof(Window))
{
	m_allocated = false;
	UIManager::desktop()->add(this);

	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);

	bind(this, &Window::on_click_widget);
	bind(this, &Window::on_key_focus);
	bind(this, &Window::on_select_widget);
	m_refocus = true;
}

void* Window::operator new(size_t size)
{
	void* obj = ::operator new(size);
	static_cast<Window*>(obj)->m_allocated = true;
	return obj;
}

Window::~Window()
{
	if (UIManager::is_initialized())
	{
		UIManager::desktop()->remove(this);
	}
}

/** Copy all styles of the window */
void Window::copy(const Window & window)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&window);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&window);
}

/** Copy all styles of the window */
void Window::copy(const Window * window)
{
	if (window)
	{
		copy(*window);
	}
}

bool Window::is_allocated()
{
	return m_allocated;
}

/** Return the size of content without marges */
Size Window::content_size()
{
	Size result;

	// If the width defined
	if (m_size.is_width_undefined() == false)
	{
		// Set the width specified
		result.width_(m_size.width_());
	}
	else
	{
		// Set the width of screen
		result.width(UIManager::renderer()->size().width());
	}

	// If the height defined
	if (m_size.is_height_undefined() == false)
	{
		// Set the height specified
		result.height_(m_size.height_());
	}
	else
	{
		// Set the height of screen
		result.height(UIManager::renderer()->size().height());
	}

	return result;
}


/** Remove the focus of the widget */
void Window::remove_focus(Widget * widget)
{
	if (m_with_focus == widget)
	{
		focus_next(m_with_focus);
		if (m_with_focus == widget)
		{
			m_with_focus = 0;
			m_refocus = 1;
		}
	}
}


void Window::place()
{
	if (m_refocus)
	{
		m_refocus = 0;
		focus_next(m_with_focus);
	}
	
	Area oldForeclip(m_foreclip);

	// If the position or size not specified
	if (m_position.is_x_undefined() && m_position.is_y_undefined() || 
		m_size.is_width_undefined() && m_size.is_height_undefined())
	{
		// Place into screen area
		Area screen_area;
		screen_area.position().set(0,0);
		screen_area.size().set(UIManager::renderer()->size().width(),UIManager::renderer()->size().height());
		place_in_layout(screen_area, content_size(), margin(), extend(), m_foreclip, align());
	}
	else
	{
		// Set fixed position and size
		m_foreclip.set(m_position,m_size);
	}
	
	Widget::place(m_foreclip, false);

	// If size of window changed
	if (m_foreclip.width() != oldForeclip.width() || m_foreclip.height() != oldForeclip.height())
	{
		if (m_flow_in_children)
		{
			// Force the flow placement
			m_flow_place = 1;
		}
	}
}

bool Window::flow_replacement()
{
	bool result = m_flow_place;
	m_flow_place = 0;
	return result;
}

void Window::paint(const Region & parent_region)
{
	// Create region of window
	Region region(m_backclip);

	// Remove other windows region
	region.subtract(parent_region);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		// Register region
		UIManager::renderer()->region(region);

		// Open svg export if it required
		Exporter * exporter = UIManager::exporter();

		// If export svg required
		if(exporter)
		{
			// Open svg group for the current window
			exporter->open_group(m_backclip.position(), m_backclip.size());
		}

		// Show window background
		Point shift;
		Rect rect(0);
			rect.size(m_backclip.size());
			rect.position(m_backclip.position());
			rect.radius_(0);
			rect.color(stated_color(m_color));
			rect.paint(shift);

		// Paint widgets children
		Widget::paint(region);

		// If svg export required
		if(exporter)
		{
			// Close svg group for the current window
			exporter->close_group();
		}
	}
}

/** Get the widget hovered */
Widget * Window::hovered(const Region & parent_region, const Point & position)
{
	Widget * result = 0;

	// Create region with window area
	Region region(m_backclip);

	// Remove the other window at foreground from the region
	region.subtract(parent_region);

	// If the touch is in the current window
	if(region.is_inside(position) != Region::Overlap::OUT)
	{
		Widget* child = m_children;
		Widget * hovered = 0;
		Widget * found = 0;

		result = this;

		// Scan all widget children
		while (child)
		{
			found = child->hovered(region, position);
			if (found)
			{
				hovered = found;
			}
			child = child->next();
		}

		// If children hovered
		if(hovered)
		{
			// Select children
			result = hovered;
		}
	}
	return result;
}

/** Places the specified window at the top of its parents canvas stack, above all of its sibling window. */
void Window::back()
{
	UIManager::desktop()->back(this);
}

/** Places the specified window at the bottom of its parents canvas stack, below all of its sibling window.*/
void Window::front()
{
	UIManager::desktop()->front(this);
}

/** Serialize the content of widget into json */
void Window::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	Widget::serialize(it);
}

/** Unserialize the content of widget from json */
void Window::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
}

/** Force flow replacement */
void Window::force_flow_replacement()
{
	m_flow_in_children = 1;
	m_flow_place = 1;
}

/** Treat the key to change the focused widget */
void Window::on_key_focus(Widget * widget, const KeyEvent & evt)
{
	if (evt.state() == KeyEvent::KEY_DOWN)
	{
		switch(evt.key())
		{
		case (wchar_t)ReservedKey::KEY_TABULATION  :
			if (evt.modifier() & KeyEvent::MODIFIER_SHIFT)
			{
				focus_previous(m_with_focus);
			}
			else
			{
				focus_next(m_with_focus);
			}
			break;
		}
	}
}

/** Treat the click on widget, used to change the focus */
void Window::on_click_widget(Widget * widget, const ClickEvent & evt)
{
	focus_to(m_with_focus, widget);
}

/** Treat the select widget, used to change the focus */
void Window::on_select_widget(Widget * widget, const SelectEvent & evt)
{
	focus_to(m_with_focus, widget);
}

#ifdef _DEBUG
void Window::test1()
{
	Window window1;
		window1.position(11,19);
		window1.size(117,219);
		window1.color(Color::LIGHT_AQUAMARINE);

		Label * label1 = new Label(&window1);
			label1->position(0,window1.size().middle().height());
			label1->text("Window 1");
			label1->font_size(20);
			label1->text_color(Color::BLUE_MOSS_GREEN);

	Window window2;
		window2.position(50,180);
		window2.size(117,219);
		window2.color(Color::LIGHT_JADE);

		Label * label2 = new Label(&window2);
			label2->position(0,window2.size().middle().height());
			label2->text("Window 2");
			label2->font_size(20);

	Window window3;
		window3.position(90,40);
		window3.size(117,219);
		window3.color(Color::LIGHT_BLUE);

		Label * label3 = new Label(&window3);
			label3->position(0,window3.size().middle().height());
			label3->text("Window 3");
			label3->font_size(20);

	UIManager::desktop()->dispatch("${ui.tests}/out/window1_0.svg");
	
	window1.front();
	UIManager::desktop()->dispatch("${ui.tests}/out/window1_1.svg");

	window1.back();
	UIManager::desktop()->dispatch("${ui.tests}/out/window1_2.svg");

	window2.front();
	UIManager::desktop()->dispatch("${ui.tests}/out/window1_3.svg");
}

void Window::test2()
{
	Window window;
		window.position(0,0);
		window.size(380,380);
		window.color(Color::TURQUOISE_GREEN);

		Grid * grid = new Grid(&window);
			Keyboard * keyboard = new Keyboard(grid);
				keyboard->cell(0,0);

			Pane * pane = new Pane(grid);
				pane->thickness(4);
				pane->radius(30);
				pane->color(Color::COTTON_CANDY);
				pane->border_color(Color::CROCUS_PURPLE);
				pane->extend(Extend::EXTEND_ALL);
				pane->cell(2,0);
				pane->margin(5,5,5,5);

			Label * label = new Label(grid);
				label->cell(2,0);
				label->size_policy(SizePolicy::SHRINK_HEIGHT);
				label->text("Bonjour");
				label->margin(10,10,10,10);

			Canvas * canvas = new Canvas(grid);
				canvas->cell(1,0);
				canvas->extend(Extend::EXTEND_ALL);
				canvas->color(Color::LIGHT_SKY_BLUE);

				Polygon * arrow = new Polygon(canvas);
					arrow->color(Color::RED,30);
					arrow->position(30,30);
					arrow->angle(180);
					
					arrow->add_point(0,15);
					arrow->add_point(-15,0);
					arrow->add_point(-7,0);
					arrow->add_point(-7,-15);
					arrow->add_point(7,-15);
					arrow->add_point(7,0);
					arrow->add_point(15,0);
					arrow->next_contour();

				arrow = new Polygon(*arrow);
					arrow->zoom(2);

				Rect * rect = new Rect(canvas);
					rect->size(60,60);
					rect->center(30,30);
					rect->position(30+60,30+3);
					rect->color(ORANGE,64);
					rect->radius(20);
					rect->thickness(0);

				rect = new Rect(*rect);
					rect->thickness(3);
					rect->color(ORANGE,128);
					rect->center(30,30);

				Text * text = new Text(canvas);
					text->text("hello\nworld");
					text->position(190,78);
					text->font_size(30,30);
					text->color(JADE,64);
					text->center(text->content_size().middle());
		
				text = new Text(*text);

				Circle * circle = new Circle(canvas);
					circle->position(90+63,33);
					circle->radius(30);
					circle->color(PURPLE,64);
					circle->thickness(0);

				circle = new Circle(*circle);
					circle->thickness(3);
					circle->color(PURPLE,128);

				Triangle * triangle = new Triangle(canvas);
					triangle->position(90+63+63,40);
					triangle->radius(30);
					triangle->color(RED,64);
					triangle->thickness(0);

				triangle = new Triangle(*triangle);
					triangle->thickness(3);
					triangle->color(RED,128);

				Cross * cross = new Cross(canvas);
					cross->position(90+63+63+63,33);
					cross->radius(30);
					cross->color(GREEN,64);
					cross->thickness(3);

				Square * square = new Square(canvas);
					square->position(90+63+63+63+63,33);
					square->radius(30);
					square->color(BLUE,64);
					square->thickness(0);

				square = new Square(*square);
					square->thickness(3);
					square->color(BLUE,128);

				PolyLines * polylines = new PolyLines(canvas);
					polylines->position(33,126);
					polylines->append(30,15);
					polylines->append(30,-15);
					polylines->append( 15,-30);
					polylines->append(-15,-30);
					polylines->append(-30,-15);
					polylines->append(-30,15);
					polylines->append(-15,30);
					polylines->append(15,30);
					polylines->append(30,15);
					polylines->thickness(3);
					polylines->color(BLUE,64);

				polylines = new PolyLines(*polylines);

				Pie * pie = new Pie(canvas);
					pie->color(BLUE,64);
					pie->position(33+68, 126);
					pie->radius(30);
					pie->thickness(0);
					pie->rope(true);
					pie->start_angle(0);
					pie->end_angle(270);

				pie = new Pie(*pie);
					pie->thickness(3);
					pie->color(BLUE,128);

				Line * line = new Line(canvas);
					line->color(BLUE,64);
					line->position(30+63+63+10, 30+63+30);
					line->thickness(3);
					line->point1(0,0);
					line->point2(60,60);
					line->center(30,30);

				line = new Line(*line);

	int angle = 0;
	UIManager::desktop()->dispatch();
	{
		for(int i = 0; i < 100; i+=10)
		{
			window.position(i,i);
			UIManager::desktop()->dispatch();
			arrow->angle(angle ++);
			rect->angle(angle);
			triangle->angle(angle);
			cross->angle(angle);
			square->angle(angle);
			text->angle(angle);
			polylines->angle(angle);
			pie->angle(angle);
			line->angle(angle);
		}
		
		UIManager::desktop()->dispatch("${ui.tests}/out/window2_1.svg");

		for(int i = 100; i > 0; i-=10)
		{
			window.position(i,i);
			UIManager::desktop()->dispatch();
			arrow->angle(angle ++);
			rect->angle(angle);
			triangle->angle(angle);
			cross->angle(angle);
			square->angle(angle);
			text->angle(angle);
			polylines->angle(angle);
			pie->angle(angle);
			line->angle(angle);
		}
		UIManager::desktop()->dispatch("${ui.tests}/out/window2_2.svg");
	}
}

void Window::test3()
{
	Window window;
		window.position(0,0);
		window.size(200,200);
		window.color(Color::TURQUOISE_GREEN);

		Grid * grid = new Grid(&window);
			Label * label = new Label(grid);
				label->cell(0,0);
				label->size_policy(SizePolicy::SHRINK_HEIGHT);
				label->text("Bonjour");
				label->margin(10,10,10,10);

			Canvas * canvas = new Canvas(grid);
				canvas->cell(1,0);
				canvas->extend(Extend::EXTEND_ALL);
				canvas->color(Color::LIGHT_SKY_BLUE);

				Text * text = new Text(canvas);
					text->text("hello\nworld");
					text->position(100,70);
					text->font_size(30,30);
					text->color(JADE,64);
					text->center(text->content_size().middle());
		
				text = new Text(*text);

	int angle = 0;
	UIManager::desktop()->dispatch();
	//MemoryLeakTracer::stat(__FUNCTION__);
	//while(true)
	{
		for(int i = 0; i < 280; i+=10)
		{
			angle++;
			window.position(i,i);
			UIManager::desktop()->dispatch();
			text->angle(angle);
			//text->text("hello\nworld");
		}
		UIManager::desktop()->dispatch("${ui.tests}/out/window3_1.svg");
		for(int i = 280; i > 0; i-=10)
		{
			angle++;
			window.position(i,i);
			UIManager::desktop()->dispatch();
			text->angle(angle);
		}
		UIManager::desktop()->dispatch("${ui.tests}/out/window3_2.svg");
	}
}

void Window::test4()
{
	Window window;
		window.position(0,0);
		window.size(380,380);

		Grid * grid = new Grid(&window);
			Canvas * canvas = new Canvas(grid);
				canvas->cell(0,0);
				canvas->extend(Extend::EXTEND_ALL);
				canvas->color(Color::LIGHT_SKY_BLUE);

				Polygon * arrow = new Polygon(canvas);
					arrow->color(Color::RED,30);
					arrow->position(190,190);
					arrow->add_point(0,15);
					arrow->add_point(-15,0);
					arrow->add_point(-7,0);
					arrow->add_point(-7,-15);
					arrow->add_point(7,-15);
					arrow->add_point(7,0);
					arrow->add_point(15,0);
					arrow->next_contour();
					arrow->center(-160,0);
					arrow->zoom(2);
					arrow->angle(0);

				for (int i = 1; i < 15; i++)
				{
					arrow = new Polygon(*arrow);
						arrow->angle(i*24);
				}
	UIManager::desktop()->dispatch("${ui.tests}/out/window4_1.svg");
}

static void window_test5(Window & window)
{
	Label * label;
	Edit  * edit;

	Pane * pane = new Pane(&window);
		pane->border_color(Color::ASH_GRAY);
		pane->thickness(2);
		pane->radius(8);
		pane->color(Color::WHITE_GRAY);
		pane->margin(2);
	
	Column * grid = new Column(pane);

	label = new Label(grid);
		label->text("Login");
		label->text_color(Color::GRAY_DOLPHIN);

	edit = new Edit(grid);
		edit->text("");
		edit->id(123);
		edit->margin(2,10,2,10);

	label = new Label(grid);
		label->text("Password");
		label->text_color(Color::GRAY_DOLPHIN);

	edit = new Edit(grid);
		edit->text("");
		edit->password(true);
		edit->margin(2,10,2,10);

	Label * spacer = new Label(grid);
		spacer->text("");
		spacer->font_size(4);
		spacer->size_policy(SizePolicy::ENLARGE_HEIGHT);

	Button * connect = new Button(grid);
		connect->text("Connect");
		connect->color(Color::DARK_GAINSBORO);
		connect->border_color(Color::BLACK_CAT);
		connect->margin(2,30,10,30);
}

void Window::test5()
{
	// No geometry defined
	{
		Window window;
		window_test5(window);
		UIManager::desktop()->dispatch("${ui.tests}/out/window5_1.svg");
	}

	// Only position defined
	{
		Window window;
		window.position(140,140);
		window_test5(window);
		UIManager::desktop()->dispatch("${ui.tests}/out/window5_2.svg");
	}

	// Size defined
	{
		Window window;
		window.size(160,180);
		window_test5(window);
		UIManager::desktop()->dispatch("${ui.tests}/out/window5_3.svg");
	}

	// Size and position defined
	{
		Window window;
		window.position(100,100);
		window.size(160,180);
		window_test5(window);
		UIManager::desktop()->dispatch("${ui.tests}/out/window5_4.svg");
	}

	// No geometry defined align
	{
		Window window;
		window_test5(window);
		window.size(160,180);
		window.align(Align::ALIGN_BOTTOM);
		UIManager::desktop()->dispatch("${ui.tests}/out/window5_6.svg");
		window.align(Align::ALIGN_TOP);
		UIManager::desktop()->dispatch("${ui.tests}/out/window5_7.svg");
		window.align(Align::ALIGN_LEFT);
		UIManager::desktop()->dispatch("${ui.tests}/out/window5_8.svg");
		window.align(Align::ALIGN_RIGHT);
		UIManager::desktop()->dispatch("${ui.tests}/out/window5_9.svg");
	}

	// No geometry defined extend
	{
		Window window;
		window_test5(window);
		window.size(160,180);
		window.extend(Extend::EXTEND_WIDTH);
		UIManager::desktop()->dispatch("${ui.tests}/out/window5_10.svg");
		window.extend(Extend::EXTEND_HEIGHT);
		UIManager::desktop()->dispatch("${ui.tests}/out/window5_11.svg");
	}
}

void Window::test6()
{
	Window window;
		window.margin(15);
		window.color(Color::NIGHT_BLUE);

		Canvas * canvas = new Canvas(&window);
			canvas->extend(Extend::EXTEND_ALL);
			canvas->color(0xFF003399);
			canvas->margin(10);

			Polygon * arrow = new Polygon(canvas);
				arrow->color(0xFFFFCC00);
				arrow->position(240-10-15,240-10-15);
				arrow->add_point(0,15);
				arrow->add_point(-15,0);
				arrow->add_point(-7,0);
				arrow->add_point(-7,-15);
				arrow->add_point(7,-15);
				arrow->add_point(7,0);
				arrow->add_point(15,0);
				arrow->next_contour();
				arrow->center(-180,0);

			for (int i = 30; i < 360; i+= 30)
			{
				arrow = new Polygon(*arrow);
					arrow->angle(i);
			}

		UIManager::desktop()->dispatch("${ui.tests}/out/window6_1.svg");
}

void window_test7(Widget * widget, int &id)
{
	widget->position(13,17);
	//widget->position(50,80);
	widget->margin(23,37,41,13);
	widget->size(211,191);
	widget->color(Color::MAUVE);
	if (dynamic_cast<TextStyle*>(widget))
	{
		(dynamic_cast<TextStyle*>(widget))->text("hello");
	}
	if (dynamic_cast<SliderStyle*>(widget))
	{
		(dynamic_cast<SliderStyle*>(widget))->value(50);
	}
	if (dynamic_cast<ProgressBarStyle*>(widget))
	{
		(dynamic_cast<ProgressBarStyle*>(widget))->value(50);
	}
	if (dynamic_cast<BorderStyle*>(widget))
	{
		(dynamic_cast<BorderStyle*>(widget))->border_color(Color::AZTECH_PURPLE);
		(dynamic_cast<BorderStyle*>(widget))->thickness(7);
	}

	if (dynamic_cast<IconStyle*>(widget))
	{
		(dynamic_cast<IconStyle*>(widget))->filename("${ui.icons}/computer.icn");
	}

	String name;
	name.print("${ui.tests}/out/window7_%d.svg",id++);
	UIManager::desktop()->dispatch(name);

	widget->align(Align::ALIGN_TOP_LEFT);
	name.print("${ui.tests}/out/window7_%d.svg",id++);
	UIManager::desktop()->dispatch(name);

	widget->align(Align::ALIGN_BOTTOM_LEFT);
	name.print("${ui.tests}/out/window7_%d.svg",id++);
	UIManager::desktop()->dispatch(name);

	delete widget;
}

void Window::test7()
{
	int id = 1;

	Window window;
		window.margin(19,23,47,73);
		window.color(Color::NIGHT_BLUE,32);

		Pane * pane = new Pane(&window);
			pane->position(13,17);
			pane->margin(23,37,41,13);
			pane->size(211,191);
			pane->color(Color::BLUE,32);

		window_test7(new Label(&window), id);
		window_test7(new Edit(&window), id);
		window_test7(new Button(&window), id);
		window_test7(new Switch(&window), id);
		window_test7(new Pane(&window), id);
		window_test7(new Icon(&window), id);
		window_test7(new Slider(&window), id);
		window_test7(new Canvas(&window), id);
		window_test7(new Keyboard(&window), id);
		window_test7(new ProgressBar(&window), id);
}

static void window_all_widgets(Window & window)
{
	Column * main_layout = new Column(&window);

		ScrollView * scroll_view = new ScrollView(main_layout);
			scroll_view->scroll_direction(SCROLL_VERTICAL);
			scroll_view->align(Align::ALIGN_TOP);
			scroll_view->margin(5);

			Column * scroll_layout = new Column(scroll_view);

				Label * event_text = new Label(scroll_layout);
					event_text->text("No event");
					event_text->font_size(10);

				Row * switch_layout = new Row(scroll_layout);
					Label * switch_text = new Label(switch_layout);
						switch_text->text("Switch");
						switch_text->align(Align::ALIGN_LEFT);
					Switch * switch_widget = new Switch(switch_layout);
						switch_widget->align(Align::ALIGN_RIGHT);

				Row * slider_layout = new Row(scroll_layout);
					Label * slider_text = new Label(slider_layout);
						slider_text->text("Slider");
						slider_text->size_policy(SizePolicy::SHRINK_WIDTH);
					Slider * slider_widget = new Slider(slider_layout);

				Label * progress_text = new Label(scroll_layout);
					progress_text->text("ProgressBar");

				ProgressBar * progress = new ProgressBar(scroll_layout);
					progress->value(25);

				Button * button = new Button(scroll_layout);
					button->text("Button");
					button->extend(Extend::EXTEND_WIDTH);

				Label * edit_text = new Label(scroll_layout);
					edit_text->text("Edit");
					edit_text->text_align(Align::ALIGN_LEFT);

				Edit * edit = new Edit(scroll_layout);
					edit->place_holder("Enter text");

				Label * keyboard_text = new Label(scroll_layout);
					keyboard_text->text("Keyboard");
					keyboard_text->text_align(Align::ALIGN_LEFT);

				Icon * icon = new Icon(scroll_layout);
					icon->filename("${ui.icons}/person.icn");
					icon->text("Icon");

				Keyboard * keyboard = new Keyboard(scroll_layout);

}

void Window::test8()
{
	Window window;
	window_all_widgets(window);
	UIManager::desktop()->dispatch("${ui.tests}/out/window8_1.svg");
	UIManager::colors()->appearance("test_dark");
	UIManager::desktop()->dispatch("${ui.tests}/out/window8_2.svg");
	UIManager::colors()->appearance("test_light");
	UIManager::desktop()->dispatch("${ui.tests}/out/window8_3.svg");
}

void Window::test9()
{
	Window window;
		window.color(Color::PINK);
		window.id(0);
		window.size(100,100);

		Window window1;
			window1.size(100, 100);
			window1.position(100, 100);
			window1.color(Color::LIGHT_JADE);
			window1.id(1);

		Window window2;
			window2.size(100, 100);
			window2.position(150, 150);
			window2.align(Align::ALIGN_BOTTOM);
			window2.extend(Extend::EXTEND_WIDTH);
			window2.color(Color::LIGHT_SKY_BLUE);
			window2.id(2);

	int id = 0;
	for (Coord i = 100; i < 240; i+= 7)
	{
		window1.position(i, i);
		UIManager::desktop()->dispatch();
		String name;
		name.print("${ui.tests}/out/window9_%d.svg", ++id);
		UIManager::desktop()->dispatch(name);
	}
}

void Window::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		test9();
		test8();
		test7();
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
#endif

