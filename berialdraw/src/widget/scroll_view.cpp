#include "berialdraw_imp.hpp"

using namespace berialdraw;

ScrollView::ScrollView(Widget * parent):
	Widget("scroll_view", parent, sizeof(ScrollView))
{
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);
	UIManager::styles()->apply(m_classname, (ScrollViewStyle*)this);
	m_color = Color::TRANSPARENT;
	m_size.clean();
	bind(this, &ScrollView::on_scroll);
}

ScrollView::~ScrollView()
{
}

/** Return the size of content without marges */
Size ScrollView::content_size()
{
	Size result = Widget::content_size();
	if (!m_viewport_size.is_width_undefined())
	{
		result.width_(m_viewport_size.width_());
	}
	if (!m_viewport_size.is_height_undefined())
	{
		result.height_(m_viewport_size.height_());
	}
	return result;
}

/** Return the size of content with its marges */
Size ScrollView::marged_size()
{
	Size result;

	// If size not defined, set null size
	if (m_viewport_size.is_width_undefined() == false)
	{
		result.width_(m_viewport_size.width_());
	}
	if (m_viewport_size.is_height_undefined() == false)
	{
		result.height_(m_viewport_size.height_());
	}

	result.increase_(m_margin.left_() + m_margin.right_(), m_margin.bottom_() + m_margin.top_());
	return result;
}

Coord ScrollView::calc_shift_focus(Coord widget_position, Dim widget_size, Coord viewport_position, Dim viewport_size)
{
	Coord result = 0;

	// shift to center widget in view
	Coord shift = ((Coord)viewport_size - (Coord)widget_size) / 2;

	// If shift is greater than the widget
	if (shift > (Coord)widget_size)
	{
		shift = widget_size;
	}
	
	// If the focused widget after view port
	if ((widget_position + (Coord)widget_size) > (viewport_position + (Coord)viewport_size))
	{
		result -= (viewport_position + (Coord)viewport_size) - (widget_position + (Coord)widget_size) -  shift;
	}
	// If the focused widget is before view port
	else if (widget_position < viewport_position)
	{
		result -= viewport_position - widget_position + shift;
	}
	return result;
}

/** Compute the scroll area */
void ScrollView::space_occupied(Point & min_position, Point & max_position)
{
	Widget::space_occupied(min_position,max_position);
}


/** Compute the scroll area */
void ScrollView::scroll_area(Area & area)
{
	Point min_position;
	Point max_position;

	space_occupied(min_position,max_position);

	area.position().set_(min_position.x_(), min_position.y_());
	area.size().set_(max_position.x_()-min_position.x_(), max_position.y_()-min_position.y_());
}

/** Move the scroll view to see the widget with the focus */
void ScrollView::scroll_focus(Widget * widget)
{
	if (widget)
	{
		Coord x = calc_shift_focus(widget->backclip().x(), widget->backclip().width(),  m_backclip.x(), m_backclip.width());
		Coord y = calc_shift_focus(widget->backclip().y(), widget->backclip().height(), m_backclip.y(), m_backclip.height());
		if (x | y)
		{
			UIManager::notifier()->scroll(x, y, this);
		}
	}
}

Coord ScrollView::compute_scroll(Coord & scroll_position, Coord & m_scroll_position, Dim scroll_size, Dim area, Align align)
{
	Coord result = 0;

	// If the view is smaller than the content size
	if (area <= scroll_size)
	{
		Coord delta = (Coord)(scroll_size - area);

		// If the scroll is before the content
		if (scroll_position < 0)
		{
			result = scroll_position;
			scroll_position = 0;
			m_scroll_position = 0;
		}
		// If the scroll is after the content
		else if (scroll_position > delta)
		{
			result = scroll_position-delta;
			scroll_position = delta;
			m_scroll_position = delta;
		}
		scroll_position = -scroll_position;
	}
	// If the view is greater than the content size
	else
	{
		Coord delta = (Coord)(area - scroll_size);
		
		// If the scrolled content must be aligned to the left
		if ((align & Align::ALIGN_HORIZONTAL) == Align::ALIGN_LEFT)
		{
			result = scroll_position;
			scroll_position = 0;
			m_scroll_position = 0;
		}
		// If the scrolled content must be aligned to the right
		else if ((align & Align::ALIGN_HORIZONTAL) == Align::ALIGN_RIGHT)
		{
			result = scroll_position-delta;
			scroll_position = delta;
			m_scroll_position = delta;
		}
		// the scrolled content is centered
		else
		{
			result = scroll_position-(delta/2);
			scroll_position = delta/2;
			m_scroll_position = delta/2;
		}
	}
	return result;
}

Point ScrollView::compute_scroll_view(const Area & area, Point & scroll_position, Size & scroll_size)
{
	Point result;
	Area fixed_area;

//	if (m_size.is_width_undefined() || m_size.is_height_undefined())
	{
		// If scroll size not specified
		if (m_scroll_size.is_width_undefined() || m_scroll_size.is_height_undefined())
		{
			m_size = Widget::content_size();

			// Compute the absolute positionned widget area
			scroll_area(fixed_area);

			if (m_size.width_() < fixed_area.width_())
			{
				m_size.width_(fixed_area.width_());
			}

			if (m_size.height_() < fixed_area.height_())
			{
				m_size.height_(fixed_area.height_());
			}

			// If the scrolled content can extend only to the width (vertical scroll)
			if (m_scroll_direction == SCROLL_VERTICAL)
			{
				// Reduce the scrolled content width to the viewport width
				m_size.width(area.width());
			}
			// If the scrolled content can extend only to the height (horizontal scroll)
			else if (m_scroll_direction == SCROLL_HORIZONTAL)
			{
				// Reduce the scrolled content height to the viewport height
				m_size.height(area.height());
			}
			scroll_size = m_size;
		}
	}

	//		scroll_size = m_size;

	Coord scroll_position_x   = scroll_position.x();
	Coord scroll_position_y   = scroll_position.y();
	Coord m_scroll_position_x = m_scroll_position.x();
	Coord m_scroll_position_y = m_scroll_position.y();
	Coord result_x;
	Coord result_y;
	result_x = compute_scroll(scroll_position_x, m_scroll_position_x, scroll_size.width(),  area.width(),  (Align)m_align);
	result_y = compute_scroll(scroll_position_y, m_scroll_position_y, scroll_size.height(), area.height(), (Align)(m_align>>2));
	result.set(result_x, result_y);

	scroll_position_x -= fixed_area.position().x();
	scroll_position_y -= fixed_area.position().y();

	scroll_position.set(scroll_position_x, scroll_position_y);
	m_scroll_position.set(m_scroll_position_x, m_scroll_position_y);

	return result;
}

void ScrollView::place(const Area & area, bool in_layout)
{
	Area scroll_area_;
	Size scroll_size(m_scroll_size);
	Point scroll_position(m_scroll_position);
	Area marged_area(area);

	marged_area.decrease(margin());

	// Compute the scroll position and size
	Point scroll_out = compute_scroll_view(marged_area, scroll_position, scroll_size);

	if (scroll_out.x() | scroll_out.y())
	{
		if (m_parent)
		{
			UIManager::notifier()->scroll(scroll_out.x(), scroll_out.y(), m_parent->scroll_view());
		}
	}

	// Move to the screen position
	scroll_position.move(marged_area.position());

	// Set the scroll area to calculate the position of all widgets in the scrolled content
	scroll_area_.set(scroll_position, scroll_size);

	// Place all children in scrolled area
	Widget::place(scroll_area_, false);
	m_backclip = area;
}

/** Paint on screen memory the content of this widget */
void ScrollView::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		Exporter * exporter = UIManager::exporter();

		if (exporter)
		{
			exporter->open_group(m_backclip.position(), m_backclip.size());
		}

		Widget::paint(region);

		if (exporter)
		{
			exporter->close_group();
		}
	}
}

/** Call back on scroll */
void ScrollView::on_scroll(Widget * widget, const ScrollEvent & evt)
{
	m_scroll_position.move(evt.shift());
	UIManager::invalidator()->dirty(this,Invalidator::GEOMETRY);
}

/** Serialize the content of widget into json */
void ScrollView::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	ScrollViewStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void ScrollView::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	ScrollViewStyle::unserialize(it);
}

/** Set scroll size */
void ScrollView::scroll_size(const Size & size)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_scroll_size = size;
}

/** Get scroll size */
const Size & ScrollView::scroll_size() const
{
	if (m_scroll_size.is_width_undefined() && m_scroll_size.is_height_undefined())
	{
		return m_size;
	}
	else
	{
		return m_scroll_size;
	}
}

/** Set the scroll size with width and height in pixels */
void ScrollView::scroll_size(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_scroll_size.set(w,h);
}

/** Set scroll position */
void ScrollView::scroll_position(const Point & position)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_scroll_position = position;
}

/** Set the scroll position with x and y in pixels */
void ScrollView::scroll_position(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_scroll_position.set(x,y);
}

/** Get scroll position */
const Point & ScrollView::scroll_position() const
{
	return m_scroll_position;
}

/** Get viewport size */
const Size & ScrollView::viewport_size() const
{
	if (m_viewport_size.is_width_undefined() && m_viewport_size.is_height_undefined())
	{
		return m_backclip.size();
	}
	else
	{
		return m_viewport_size;
	}
}

/** Set viewport size */
void ScrollView::viewport_size(const Size & size)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_viewport_size = size;
}

/** Set the viewport size with width and height in pixels */
void ScrollView::viewport_size(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_viewport_size.set(w,h);
}

#ifdef _DEBUG

void ScrollView::test1()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(140,140);
		window.size(160,180);
		int row = 0;
		int column = 0;
		Label * label;
		Edit  * edit;

		ScrollView * scroll_view = new ScrollView(&window);
		Grid * grid = new Grid(scroll_view);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("First Name");
			label->margin_top(10);

		edit = new Edit(grid);
			edit->text("");
			edit->cell(row++,column);
			edit->max_columns(20);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Last name");
			label->margin_top(10);

		edit = new Edit(grid);
			edit->text("");
			edit->cell(row++,column);
			edit->max_columns(20);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Address");
			label->margin_top(10);

		edit = new Edit(grid);
			edit->text("");
			edit->cell(row++,column);
			edit->max_columns(20);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Email");
			label->margin_top(10);

		edit = new Edit(grid);
			edit->text("");
			edit->cell(row++,column);
			edit->max_columns(20);

		Button * connect = new Button(grid);
			connect->margin_top(10);
			connect->text("OK");
			connect->cell(row++,column);
			connect->margin(2,30,10,30);

	UIManager::desktop()->dispatch();

	Size size = scroll_view->scroll_size();
	size.decrease(scroll_view->viewport_size());
	Point position = scroll_view->scroll_position();
	int id = 0;
	int i = 0;
	for (Dim y = 0; y <= size.height(); y+=10)
	{
		for (Dim x = 0; x <= size.width(); x+=7)
		{
			position.set(x,y);
			scroll_view->scroll_position(position);

			if (i % 11 == 0)
			{
				String name;
				name.print("$(ui.tests)/out/scroll1_%d.svg", ++id);
				UIManager::desktop()->dispatch(name);
			}
			else
			{
				UIManager::desktop()->dispatch();
			}
			i++;
		}
	}
}

void ScrollView::test_create_window(Window & window)
{
	int row = 0;
	int column = 0;
	Label * label;
	Edit  * edit;

	ScrollView * scroll_view = new ScrollView(&window);

	scroll_view->scroll_direction(SCROLL_VERTICAL);
	scroll_view->id(1);

	Grid * grid = new Grid(scroll_view);

	label = new Label(grid);
		label->cell(row++,column);
		label->text("First Name");
		label->margin_top(20);

	edit = new Edit(grid);
		edit->text("");
		edit->cell(row++,column);

	label = new Label(grid);
		label->cell(row++,column);
		label->text("Last name");
		label->margin_top(20);

	edit = new Edit(grid);
		edit->text("");
		edit->cell(row++,column);

	label = new Label(grid);
		label->cell(row++,column);
		label->text("Address");
		label->margin_top(20);

	edit = new Edit(grid);
		edit->text("");
		edit->cell(row++,column);

	label = new Label(grid);
		label->cell(row++,column);
		label->text("Email");
		label->margin_top(20);

	edit = new Edit(grid);
		edit->text("");
		edit->cell(row++,column);

	Button * button = new Button(grid);
		button->text("OK");
		button->cell(row++,column);
		button->margin(20,30,10,30);
}

void ScrollView::test2()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(140,140);
		window.size(160,180);
		window.color(Color::ALICE_BLUE);

	test_create_window(window);

	String script(
	"["
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':299,'y': 26,'state':'move'},"
		"{'type':'touch','x':263,'y': 61,'state':'move'},"
		"{'type':'touch','x':258,'y':149,'state':'move'},"
		"{'type':'touch','x':231,'y':221,'state':'move'},"
		"{'type':'touch','x':235,'y':259,'state':'move'},"
		"{'type':'touch','x':238,'y':263,'state':'down'},"
		"{'type':'touch','x':237,'y':254,'state':'move'},"
		"{'type':'touch','x':231,'y':187,'state':'move'},"
		"{'type':'touch','x':230,'y':178,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':229,'y':178,'state':'down'},"
		"{'type':'touch','x':229,'y':215,'state':'move'},"
		"{'type':'touch','x':228,'y':274,'state':'move'},"
		"{'type':'touch','x':225,'y':294,'state':'move'},"
		"{'type':'touch','x':224,'y':305,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':224,'y':305,'state':'down'},"
		"{'type':'touch','x':224,'y':304,'state':'move'},"
		"{'type':'touch','x':221,'y':206,'state':'move'},"
		"{'type':'touch','x':221,'y':144,'state':'move'},"
		"{'type':'touch','x':220,'y':102,'state':'move'},"
		"{'type':'touch','x':220,'y': 87,'state':'move'},"
		"{'type':'touch','x':219,'y': 83,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':220,'y': 98,'state':'move'},"
		"{'type':'touch','x':225,'y':203,'state':'move'},"
		"{'type':'touch','x':227,'y':289,'state':'move'},"
		"{'type':'touch','x':226,'y':294,'state':'down'},"
		"{'type':'touch','x':227,'y':299,'state':'move'},"
		"{'type':'touch','x':237,'y':323,'state':'move'},"
		"{'type':'touch','x':241,'y':330,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':225,'y':280,'state':'move'},"
		"{'type':'touch','x':213,'y':218,'state':'move'},"
		"{'type':'touch','x':208,'y':204,'state':'down'},"
		"{'type':'touch','x':208,'y':204,'state':'up'},"
		"{'type':'key','key':114,'state':'down','modifier':''     ,'character':'r'},"
		"{'type':'key','key':101,'state':'down','modifier':''     ,'character':'e'},"
		"{'type':'key','key':114,'state':'up'  ,'modifier':''     ,'character':'r'},"
		"{'type':'key','key':101,'state':'up'  ,'modifier':''     ,'character':'e'},"
		"{'type':'key','key':109,'state':'down','modifier':''     ,'character':'m'},"
		"{'type':'key','key':109,'state':'up'  ,'modifier':''     ,'character':'m'},"
		"{'type':'key','key':105,'state':'down','modifier':''     ,'character':'i'},"
		"{'type':'key','key':105,'state':'up'  ,'modifier':''     ,'character':'i'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':208,'y':204,'state':'down'},"
		"{'type':'touch','x':208,'y':182,'state':'move'},"
		"{'type':'touch','x':208,'y':123,'state':'move'},"
		"{'type':'touch','x':207,'y':120,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':217,'y':139,'state':'move'},"
		"{'type':'touch','x':239,'y':232,'state':'move'},"
		"{'type':'touch','x':238,'y':232,'state':'down'},"
		"{'type':'touch','x':212,'y':233,'state':'move'},"
		"{'type':'touch','x':170,'y':234,'state':'move'},"
		"{'type':'touch','x':172,'y':235,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':172,'y':223,'state':'move'},"
		"{'type':'touch','x':155,'y':164,'state':'move'},"
		"{'type':'touch','x':155,'y':160,'state':'down'},"
		"{'type':'touch','x':165,'y':161,'state':'move'},"
		"{'type':'touch','x':175,'y':163,'state':'move'},"
		"{'type':'touch','x':176,'y':163,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
	"]");
	UIManager::notifier()->play_script(script, "$(ui.tests)/out/scroll2_%d.svg");

}

void ScrollView::test3()
{
	Window window;
		window.position(140,140);
		window.size(160,180);
		window.color(Color::ALICE_BLUE);
	int id = 0;

	ScrollView * scroll_view = new ScrollView(&window);

	Grid * grid = new Grid(scroll_view);

	scroll_view->scroll_size(30,40);

	Label * label = new Label(grid);
		label->text("0");
		label->cell(0,0);

	label = new Label(grid);
		label->text("1");
		label->cell(1,0);

	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll3_1.svg");
	scroll_view->align(Align::ALIGN_BOTTOM);
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll3_2.svg");
	scroll_view->align(Align::ALIGN_TOP);
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll3_3.svg");
	scroll_view->align(Align::ALIGN_LEFT);
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll3_4.svg");
	scroll_view->align(Align::ALIGN_RIGHT);
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll3_5.svg");
}

void ScrollView::test4()
{
	Window window;
		window.position(140,140);
		window.size(160,180);
		window.color(Color::ALICE_BLUE);
	int id = 0;

	ScrollView * scroll_view = new ScrollView(&window);

	Grid * grid = new Grid(scroll_view);

	for (uint16_t row = 0; row < 10; row ++)
	{
		for (uint16_t column = 0; column < 10; column++)
		{
			Label * label = new Label(grid);
				label->text("%d%c",row,0x41 + column);
				label->cell(row,column);
		}
	}

	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll4_1.svg");
	scroll_view->scroll_direction(SCROLL_VERTICAL);
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll4_2.svg");
	scroll_view->scroll_direction(SCROLL_HORIZONTAL);
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll4_3.svg");
	scroll_view->scroll_direction(SCROLL_ALL_DIRECTIONS);
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll4_4.svg");
}

void ScrollView::test5()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(140,140);
		window.size(160,180);
		window.color(Color::ALICE_BLUE);

	ScrollView * scroll_view = new ScrollView(&window);
		scroll_view->scroll_direction(SCROLL_VERTICAL);
		scroll_view->id(123);
	Column * grid = new Column(scroll_view);
	Button * button;
	int id = 1000;
	for (int i = 0; i < 11; i++)
	{
		if (i == 5)
		{
			ScrollView * imbricated_scrollview = new ScrollView(grid);
				imbricated_scrollview->scroll_direction(SCROLL_VERTICAL);
				imbricated_scrollview->id(456);
				imbricated_scrollview->viewport_size(Size::MAX_SIZE, 60);
				imbricated_scrollview->margin(5);

			Column * imbricated_grid = new Column(imbricated_scrollview);
			for (int  j = 0; j < 5; j++)
			{
				button = new Button(imbricated_grid);
					button->text("%c",0x41 + j);
					button->id(id++);
			}
		}
		else
		{
			button = new Button(grid);
				button->margin(20,4);
				button->text("%d",i);
				button->color(Color::BLUE_WHALE);
				button->border_color(Color::TYRIAN_PURPLE);
				button->id(id++);
		}
	}

	//while(true) UIManager::desktop()->dispatch();
	String script(
	"["

		"{'type':'touch','x':249,'y':288,'state':'down'},"
		"{'type':'touch','x':249,'y':277,'state':'move'},"
		"{'type':'touch','x':250,'y':192,'state':'move'},"
		"{'type':'touch','x':250,'y':190,'state':'up'},"

		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':249,'y':288,'state':'down'},"
		"{'type':'touch','x':249,'y':277,'state':'move'},"
		"{'type':'touch','x':250,'y':192,'state':'move'},"
		"{'type':'touch','x':250,'y':190,'state':'up'},"

		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':249,'y':288,'state':'down'},"
		"{'type':'touch','x':249,'y':277,'state':'move'},"
		"{'type':'touch','x':250,'y':192,'state':'move'},"
		"{'type':'touch','x':250,'y':190,'state':'up'},"

		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':249,'y':288,'state':'down'},"
		"{'type':'touch','x':249,'y':277,'state':'move'},"
		"{'type':'touch','x':250,'y':192,'state':'move'},"
		"{'type':'touch','x':250,'y':190,'state':'up'},"

		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':249,'y':288,'state':'down'},"
		"{'type':'touch','x':249,'y':277,'state':'move'},"
		"{'type':'touch','x':250,'y':192,'state':'move'},"
		"{'type':'touch','x':250,'y':190,'state':'up'},"

		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

	"]");
	UIManager::notifier()->play_script(script,"$(ui.tests)/out/scroll5_%d.svg");
}

void ScrollView::test6()
{
	Window window;
	Column *	m_content = new Column(&window);

		Label * label = new Label(m_content);
			label->text("Horizontal scroll view");

		ScrollView * horizontal_scroll_view = new ScrollView(m_content);
			horizontal_scroll_view->scroll_direction(SCROLL_HORIZONTAL);
			horizontal_scroll_view->size_policy(SizePolicy::ENLARGE_ALL);
			Row * horizontal_layout = new Row(horizontal_scroll_view);
				for (int i = 0; i < 26; i++)
				{
					Button *button  = new Button(horizontal_layout);
						button->text("%c",i+0x41);
				}

		label = new Label(m_content);
			label->text("Vertical scroll view");

		ScrollView * vertical_scroll_view = new ScrollView(m_content);
			vertical_scroll_view->scroll_direction(SCROLL_VERTICAL);
			vertical_scroll_view->size_policy(SizePolicy::ENLARGE_ALL);
			Column  * vertical_scroll_layout = new Column(vertical_scroll_view);
				for (int i = 0; i < 26; i++)
				{
					Button *button  = new Button(vertical_scroll_layout);
						button->text("%c",i+0x41);
				}
	
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll6_0.svg");
}

void ScrollView::test7()
{
}

void ScrollView::test8()
{
}

void ScrollView::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
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
