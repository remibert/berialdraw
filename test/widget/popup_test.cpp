#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Popup::test1()
{
	MemoryLeakLog
	Window window;
	window.position(20,20);
	window.size(320,240);
	window.color(Color::WHITE);

	Button * anchor = new Button(&window);
	anchor->position(24,22);
	anchor->size(110,32);
	anchor->text("Anchor");

	Popup * popup = new Popup(&window);
	popup->size(150,55);
	Button * action = new Button(popup);
	action->text("Popup button");
	action->margin(5);

	assert(popup->open(anchor));
	UIManager::desktop()->dispatch("$(ui.tests)/out/popup1.svg");

	assert(popup->is_open());
	assert(popup->foreclip().y_q6() >= anchor->foreclip().y_q6() + anchor->foreclip().height_q6());
}

void Popup::test2()
{
	MemoryLeakLog
	Window window;
	window.position(30,24);
	window.size(340,260);
	window.color(Color::WHITE);

	Button * anchor = new Button(&window);
	anchor->position(34,28);
	anchor->size(120,32);
	anchor->text("Choose an item");

	Popup * popup = new Popup(&window);
	popup->size(190,140);
	List * list = new List(popup);
	list->extend(Extend::EXTEND_ALL);
	list->scroll_direction(SCROLL_VERTICAL);
	list->append("First option");
	list->append("Second option");
	list->append("Third option");
	list->append("Fourth option");

	assert(popup->open(anchor));
	UIManager::desktop()->dispatch("$(ui.tests)/out/popup2.svg");

	assert(popup->is_open());
	assert(list->count() == 4);
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_ESCAPE, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(popup->is_open() == false);
}

void Popup::test3()
{
	MemoryLeakLog
	Window window;
	window.position(18,18);
	window.size(360,360);
	window.color(Color::WHITE);

	ScrollView * scroll_view = new ScrollView(&window);
	scroll_view->position(28,36);
	scroll_view->size(230,150);
	scroll_view->scroll_size(230,400);
	scroll_view->scroll_direction(SCROLL_VERTICAL);

	Label * content = new Label(scroll_view);
	content->position(16,12);
	content->text("Scrollable content");

	Button * anchor = new Button(scroll_view);
	anchor->position(16,108);
	anchor->size(145,30);
	anchor->text("Open from scroll area");

	Popup * popup = new Popup(&window);
	popup->size(190,132);
	List * list = new List(popup);
	list->extend(Extend::EXTEND_ALL);
	list->scroll_direction(SCROLL_VERTICAL);
	list->append("Alpha");
	list->append("Bravo");
	list->append("Charlie");
	list->append("Delta");
	list->append("Echo");
	list->append("Foxtrot");
	list->append("Golf");
	list->append("Hotel");
	list->margin(0);
	assert(popup->open(anchor));
	UIManager::desktop()->dispatch("$(ui.tests)/out/popup3_1.svg");

	assert(popup->foreclip().y_q6() + popup->foreclip().height_q6() >
		scroll_view->foreclip().y_q6() + scroll_view->foreclip().height_q6());
	Coord initial_list_scroll = list->scroll_position().y_q6();
	UIManager::notifier()->scroll(0, 8, list);
	UIManager::desktop()->dispatch();
	UIManager::desktop()->dispatch();
	UIManager::desktop()->dispatch();
	UIManager::desktop()->dispatch("$(ui.tests)/out/popup3_2.svg");
	assert(list->scroll_position().y_q6() != initial_list_scroll);
	assert(popup->is_open());
	UIManager::notifier()->scroll(0, 8, scroll_view);
	UIManager::desktop()->dispatch();
	UIManager::desktop()->dispatch();
	UIManager::desktop()->dispatch();
	UIManager::desktop()->dispatch();
	UIManager::desktop()->dispatch("$(ui.tests)/out/popup3_3.svg");
	assert(popup->is_open() == false);
}

void Popup::test4()
{
	MemoryLeakLog
	Window window;
	window.position(20,20);
	window.size(320,240);
	window.color(Color::WHITE);

	Popup * popup = new Popup(&window);
	popup->size(100,60);
	Button * action = new Button(popup);
	action->text("Popup action");

	Point undefined_position;
	assert(popup->open_at(undefined_position) == false);

	Point click_position(120,100);
	assert(popup->open_at(click_position));
	UIManager::desktop()->dispatch();
	assert(popup->is_open());
	assert(popup->foreclip().x_q6() == click_position.x_q6());
	assert(popup->foreclip().y_q6() == click_position.y_q6());

	popup->close();
	Point edge_position(330,250);
	assert(popup->open_at(edge_position));
	UIManager::desktop()->dispatch("$(ui.tests)/out/popup4.svg");
	assert(popup->is_open());
	assert(popup->foreclip().x_q6() >= window.foreclip().x_q6());
	assert(popup->foreclip().y_q6() >= window.foreclip().y_q6());
	assert(popup->foreclip().x_q6() + popup->foreclip().width_q6() ==
		window.foreclip().x_q6() + window.foreclip().width_q6());
	assert(popup->foreclip().y_q6() + popup->foreclip().height_q6() <=
		window.foreclip().y_q6() + window.foreclip().height_q6());
}

void Popup::test5()
{
	MemoryLeakLog
	Window window;
	window.position(20,20);
	window.size(320,320);

	ScrollView * scroll_view = new ScrollView(&window);
	scroll_view->position(20,20);
	scroll_view->size(220,120);
	scroll_view->scroll_size(220,400);
	scroll_view->scroll_direction(SCROLL_VERTICAL);

	Button * anchor = new Button(scroll_view);
	anchor->position(16,100);
	anchor->size(120,30);

	Popup * popup = new Popup(&window);
	popup->size(180,120);
	List * list = new List(popup);
	list->focusable(true);
	list->selection_mode(ListSelectionMode::LIST_SINGLE_SELECTION);
	list->append("First item");
	list->append("Second item");
	list->append("Third item");

	assert(popup->open(anchor));
	UIManager::desktop()->dispatch();
	assert(popup->is_open());

	window.focus_to(list);
	list->select(1);
	UIManager::notifier()->scroll(0, 8, scroll_view);
	UIManager::notifier()->dispatch();

	assert(popup->is_open() == false);
	assert(scroll_view->scroll_position().y() == 8);
}

// Verify popup hit testing while open and after closing.
void Popup::test6()
{
	MemoryLeakLog
	Window window;
	window.position(20,20);
	window.size(320,240);

	Button * background = new Button(&window);
	background->position(100,80);
	background->size(180,120);

	Popup * popup = new Popup(&window);
	popup->size(120,80);
	Button * action = new Button(popup);
	action->position(10,10);
	action->size(80,30);

	Point anchor_position(120,100);
	assert(popup->open_at(anchor_position));
	UIManager::desktop()->dispatch();

	Point hit_position;
	hit_position.set_q6(
		action->foreclip().x_q6() + (Coord)(action->foreclip().width_q6() / 2),
		action->foreclip().y_q6() + (Coord)(action->foreclip().height_q6() / 2));
	assert(UIManager::desktop()->hovered(hit_position) == action);

	popup->close();
	assert(UIManager::desktop()->hovered(hit_position) == background);
}

// Verify destruction closes an active popup and safely unregisters a closed one.
void Popup::test7()
{
	MemoryLeakLog
	Window window;
	window.position(20,20);
	window.size(320,240);

	Point popup_position(80,60);
	Popup * popup = new Popup(&window);
	popup->size(80,60);
	assert(popup->open_at(popup_position));
	UIManager::desktop()->dispatch();
	assert(popup->is_open());
	delete popup;

	Popup * replacement = new Popup(&window);
	replacement->size(80,60);
	assert(replacement->open_at(popup_position));
	UIManager::desktop()->dispatch();
	assert(replacement->is_open());
	replacement->close();
	delete replacement;
}

// Verify anchor visibility against hidden and clipped ancestors.
void Popup::test8()
{
	MemoryLeakLog
	Window window;
	window.position(20,20);
	window.size(320,240);

	Pane * clipper = new Pane(&window);
	clipper->position(40,40);
	clipper->size(120,120);
	Button * anchor = new Button(clipper);
	anchor->position(20,20);
	anchor->size(40,40);

	Popup * popup = new Popup(&window);
	popup->size(80,60);
	assert(popup->open(anchor));
	UIManager::desktop()->dispatch();

	Area anchor_area = anchor->foreclip();
	Area clip = anchor_area;
	clip.x_q6(anchor_area.x_q6() + 10 * 64);
	clip.width_q6(anchor_area.width_q6() - 10 * 64);
	clipper->m_contentclip = clip;
	assert(popup->anchor_is_visible());

	clip = anchor_area;
	clip.y_q6(anchor_area.y_q6() + 10 * 64);
	clip.height_q6(anchor_area.height_q6() - 10 * 64);
	clipper->m_contentclip = clip;
	assert(popup->anchor_is_visible());

	clip = anchor_area;
	clip.width_q6(anchor_area.width_q6() - 10 * 64);
	clipper->m_contentclip = clip;
	assert(popup->anchor_is_visible());

	clip = anchor_area;
	clip.height_q6(anchor_area.height_q6() - 10 * 64);
	clipper->m_contentclip = clip;
	assert(popup->anchor_is_visible());

	clip = anchor_area;
	clip.x_q6(anchor_area.x_q6() + (Coord)anchor_area.width_q6() + 64);
	clip.width_q6(10 * 64);
	clipper->m_contentclip = clip;
	assert(popup->anchor_is_visible() == false);

	clip = anchor_area;
	clip.y_q6(anchor_area.y_q6() + (Coord)anchor_area.height_q6() + 64);
	clip.height_q6(10 * 64);
	clipper->m_contentclip = clip;
	assert(popup->anchor_is_visible() == false);

	bool in_layout = true;
	popup->place(window.foreclip(), in_layout);
	assert(popup->is_open() == false);

	clipper->m_contentclip = anchor_area;
	assert(popup->open(anchor));
	UIManager::desktop()->dispatch();
	clipper->hidden(true);
	assert(popup->anchor_is_visible() == false);
	clipper->hidden(false);

	anchor->hidden(true);
	assert(popup->anchor_is_visible() == false);
	popup->place(window.foreclip(), in_layout);
	assert(popup->is_open() == false);
}

// Verify popup size constraints and position clamping at every area boundary.
void Popup::test9()
{
	MemoryLeakLog
	Window window;
	window.position(20,20);
	window.size(320,240);

	Popup * popup = new Popup(&window);
	popup->size(60,40);
	UIManager::desktop()->dispatch();
	Area area = window.foreclip();

	popup->m_point_anchor = true;
	popup->m_anchor_position.set_q6(area.x_q6() + 100 * 64, area.y_q6() + 100 * 64);
	popup->position_popup(area, area.width_q6() + 64, area.height_q6() + 64);
	assert(popup->position().x_q6() == 0);
	assert(popup->position().y_q6() == 0);
	popup->m_point_anchor = false;

	Size requested_min(500,400);
	Size requested_max(600,500);
	popup->min_size(requested_min);
	popup->max_size(requested_max);
	Point center_position(area.x() + 100, area.y() + 80);
	assert(popup->open_at(center_position));
	UIManager::desktop()->dispatch();
	assert(popup->foreclip().width_q6() == popup->backclip().width_q6());
	assert(popup->foreclip().height_q6() == popup->backclip().height_q6());
	assert(popup->min_size() == requested_min);
	assert(popup->max_size() == requested_max);

	popup->close();
	popup->min_size(0,0);
	popup->max_size(200,150);
	popup->size(60,40);

	Point left_position(0, area.y() + 60);
	assert(popup->open_at(left_position));
	UIManager::desktop()->dispatch();
	assert(popup->foreclip().x_q6() == area.x_q6());

	popup->close();
	Point top_position(area.x() + 100, 0);
	assert(popup->open_at(top_position));
	UIManager::desktop()->dispatch();
	assert(popup->foreclip().y_q6() == area.y_q6());

	popup->close();
	Point bottom_position(area.x() + 100, area.y() + area.height() + 100);
	assert(popup->open_at(bottom_position));
	UIManager::desktop()->dispatch();
	assert(popup->foreclip().y_q6() + popup->foreclip().height_q6() ==
		area.y_q6() + area.height_q6());
	popup->close();
}

void Popup::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
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