#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Combo::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test3();
		test2();
		test1();
	}
}

void Combo::test1()
{
	MemoryLeakLog
	Window window;
	window.size(320, 240);
	Combo * combo = new Combo(&window);
	combo->append("First");
	combo->append("Second");
	combo->append("Third");
	combo->append("Fourth");
	combo->append("Fifth");
	combo->append("Sixth");
	combo->append("Seventh");
	combo->append("Eighth");
	combo->append("Ninth");
	combo->append("Tenth");
	combo->append("Eleventh");
	combo->append("Twelfth");
	combo->append("Thirteenth");
	combo->append("Fourteenth");
	// UIManager::desktop()->mainloop();

	UIManager::desktop()->dispatch("$(ui.tests)/out/combo1_1.svg");
	assert(combo->input() == String("First"));
	assert(combo->selected_item() == 0);
	assert(combo->selected_index() == -1);
	combo->editable(true);
	combo->typing_mode(Entry::TypingMode::READ_ONLY);

	assert(combo->selection_start() == combo->input().count());
	assert(combo->selection_end() == combo->input().count());

	assert(combo->count() == 14);
	combo->select(1);
	assert(combo->selected_index() == 1);
	assert(combo->input() == String("Second"));
	assert(combo->selection_start() == combo->input().count());
	assert(combo->selection_end() == combo->input().count());
	combo->select_text(0, 3);
	ClickEvent click_event(Point(), combo->at(0));
	combo->on_item_click(combo->at(0), click_event);
	assert(combo->input() == String("First"));
	assert(combo->selection_start() == combo->input().count());
	assert(combo->selection_end() == combo->input().count());
	window.focus_to(combo);
	KeyEvent key_down((wchar_t)ReservedKey::KEY_DOWN, KeyEvent::KEY_DOWN);
	combo->on_key(combo, key_down);
	assert(combo->selected_index() == 0);
	KeyEvent key_up((wchar_t)ReservedKey::KEY_UP, KeyEvent::KEY_DOWN);
	combo->on_key(combo, key_up);
	assert(combo->selected_index() == 0);

	combo->select(13);
	combo->on_key(combo, key_down);
	assert(combo->selected_index() == 13);
	combo->select(0);
	combo->on_key(combo, key_up);
	assert(combo->selected_index() == 0);

	combo->text("custom value");
	assert(combo->selected_item() == 0);
	combo->select(1);

	combo->editable(false);
	assert(combo->typing_mode() == Entry::TypingMode::READ_ONLY);

	combo->editable(true);
	assert(combo->typing_mode() == Entry::TypingMode::INSERTION);

	combo->text("New option");
	KeyEvent key_enter((wchar_t)ReservedKey::KEY_ENTER, KeyEvent::KEY_DOWN);
	combo->on_key(combo, key_enter);
	assert(combo->count() == 15);
	assert(combo->at(14)->text() == String("New option"));
	assert(combo->selected_index() == 14);
	combo->on_key(combo, key_enter);
	assert(combo->count() == 15);

	ClickEvent button_click(Point(), combo->m_button);
	combo->on_button_click(combo->m_button, button_click);
	assert(combo->is_open());
	combo->select(0);
	assert(combo->m_list->select_previous() == 0);
	assert(combo->selected_index() == 0);
	combo->select((int)combo->count() - 1);
	assert(combo->m_list->select_next() == 0);
	assert(combo->selected_index() == (int)combo->count() - 1);
	combo->close();
	assert(combo->is_open() == false);
}

void Combo::test2()
{
	MemoryLeakLog
	Window window;
	window.size(320, 240);
	Button * other = new Button(&window);
	other->focusable(true);

	Combo * combo = new Combo(&window);
	combo->position(24, 28);
	combo->size(260, 38);
	window.focus_to(other);
	assert(combo->is_empty());
	assert(combo->count() == 0);
	assert(combo->selected_item() == nullptr);
	assert(combo->selected_index() == -1);
	assert(combo->items().size() == 0);
	assert(combo->selected_items().size() == 0);
	assert(combo->select_first() == nullptr);
	assert(combo->select_last() == nullptr);
	assert(combo->select_next() == nullptr);
	assert(combo->select_previous() == nullptr);
	combo->text("Unfocused input");
	KeyEvent enter_down((wchar_t)ReservedKey::KEY_ENTER, KeyEvent::KEY_DOWN);
	combo->on_key(combo, enter_down);
	assert(combo->count() == 0);
	assert(combo->input() == String("Unfocused input"));

	bool append_configured = false;
	bool prepend_configured = false;
	bool insert_configured = false;
	combo->append("Bravo");
	combo->append([&append_configured](ListItem * item)
	{
		append_configured = true;
		item->text("Echo");
	});
	combo->prepend("Alpha");
	combo->prepend([&prepend_configured](ListItem * item)
	{
		prepend_configured = true;
		item->text("Aardvark");
	});
	combo->insert(3, "Charlie");
	combo->insert(4, [&insert_configured](ListItem * item)
	{
		insert_configured = true;
		item->text("Delta");
	});
	ListItem * item = combo->new_item();
	assert(item != nullptr);
	item->text("Foxtrot");
	assert(append_configured);
	assert(prepend_configured);
	assert(insert_configured);

	assert(combo->count() == 7);
	assert(combo->is_empty() == false);
	assert(combo->at(0)->text() == String("Aardvark"));
	assert(combo->at(1)->text() == String("Alpha"));
	assert(combo->at(2)->text() == String("Bravo"));
	assert(combo->at(3)->text() == String("Charlie"));
	assert(combo->at(4)->text() == String("Delta"));
	assert(combo->at(5)->text() == String("Echo"));
	assert(combo->at(6)->text() == String("Foxtrot"));
	assert(combo->at(-1) == combo->at(6));
	assert((*combo)[-2] == combo->at(5));
	assert(combo->items().size() == combo->count());

	UIManager::desktop()->dispatch("$(ui.tests)/out/combo2_1.svg");

	combo->unselect_all();
	assert(combo->selected_item() == nullptr);
	assert(combo->selected_index() == -1);
	combo->select_first();
	assert(combo->selected_item() == combo->at(0));
	assert(combo->input() == String("Aardvark"));
	assert(combo->selected_items().size() == 1);
	assert(combo->select_next() == combo->at(1));
	assert(combo->selected_index() == 1);
	assert(combo->input() == String("Alpha"));
	assert(combo->select_previous() == combo->at(0));
	assert(combo->selected_index() == 0);
	combo->unselect(-1);
	assert(combo->selected_item() == nullptr);
	assert(combo->selected_items().size() == 0);
	assert(combo->select_last() == combo->at(6));
	assert(combo->selected_item() == combo->at(6));
	assert(combo->input() == String("Foxtrot"));

	combo->remove(2);
	assert(combo->count() == 6);
	assert(combo->at(2)->text() == String("Charlie"));
	combo->remove(-1);
	assert(combo->count() == 5);
	assert(combo->at(-1)->text() == String("Echo"));
	assert(combo->selected_item() == nullptr);
	assert(combo->selected_index() == -1);
	assert(combo->input().size() == 0);
	combo->remove(-1);
	assert(combo->count() == 4);
	assert(combo->at(-1)->text() == String("Delta"));

	combo->clear();
	assert(combo->is_empty());
	assert(combo->count() == 0);
	assert(combo->selected_items().size() == 0);
	assert(combo->input().size() == 0);
	bool default_item_configured = false;
	ListItem * default_item = combo->insert(0, [&default_item_configured](ListItem * configured_item)
	{
		default_item_configured = true;
		configured_item->text("Configured default");
	});
	assert(default_item_configured);
	assert(default_item != nullptr);
	assert(combo->input() == String("Configured default"));
	assert(combo->count() == 1);
	combo->clear();

	window.focus_to(combo);
	combo->text("");
	combo->on_key(combo, enter_down);
	assert(combo->count() == 0);
	combo->append("Existing option");
	combo->editable(false);
	combo->text("Must not be added");
	combo->on_key(combo, enter_down);
	assert(combo->count() == 1);
	assert(combo->input() == String("Must not be added"));
	combo->editable(true);
	combo->text("Key release must be ignored");
	KeyEvent enter_up((wchar_t)ReservedKey::KEY_ENTER, KeyEvent::KEY_UP);
	combo->on_key(combo, enter_up);
	assert(combo->count() == 1);
	assert(combo->input() == String("Key release must be ignored"));
	combo->select(99);
	assert(combo->selected_index() == -1);
	assert(combo->input() == String("Key release must be ignored"));

	combo->editable(true);
	combo->button_width(36);
	combo->max_popup_height(84);
	Json json;
	JsonIterator iterator(json);
	combo->serialize(iterator);
	combo->editable(false);
	combo->button_width(12);
	combo->max_popup_height(24);
	combo->unserialize(iterator);
	assert(combo->editable());
	assert(combo->button_width() == 36);
	assert(combo->max_popup_height() == 84);

	Combo detached(nullptr);
	assert(detached.open() == false);
	assert(detached.is_open() == false);
	assert(detached.new_item() == nullptr);
	assert(detached.append("No popup") == nullptr);
	assert(detached.insert(0, "No popup") == nullptr);
	detached.remove(0);
	detached.unselect(0);
	detached.unselect_all();
	detached.close();
	assert(detached.count() == 0);
}

void Combo::test3()
{
	MemoryLeakLog
	Window window;
	window.size(320, 240);
	window.color(Color::WHITE);

	Combo * combo = new Combo(&window);
	combo->position(30, 24);
	combo->size(250, 38);
	combo->button_width(32);
	combo->max_popup_height(96);
	combo->append("First option");
	combo->append("Second option");
	combo->append("Third option");
	combo->append("Fourth option");
	combo->append("Fifth option");
	combo->append("Sixth option");
	combo->append("Seventh option");

	UIManager::desktop()->dispatch("$(ui.tests)/out/combo3_1.svg");
	assert(combo->is_open() == false);
	assert(combo->button_width() == 32);
	assert(combo->max_popup_height() == 96);

	assert(combo->open());
	assert(combo->is_open());
	UIManager::desktop()->dispatch("$(ui.tests)/out/combo3_2.svg");

	combo->select(3);
	assert(combo->selected_index() == 3);
	assert(combo->input() == String("Fourth option"));
	UIManager::desktop()->dispatch("$(ui.tests)/out/combo3_3.svg");

	ListItem * clicked_item = combo->at(4);
	const Area & clicked_area = clicked_item->foreclip();
	UIManager::notifier()->click(
		clicked_area.x() + (Coord)(clicked_area.width() / 2),
		clicked_area.y() + (Coord)(clicked_area.height() / 2),
		clicked_item);
	UIManager::notifier()->dispatch();
	assert(combo->is_open() == false);
	assert(combo->selected_index() == 4);
	assert(combo->input() == String("Fifth option"));
	UIManager::desktop()->dispatch("$(ui.tests)/out/combo3_4.svg");

	ClickEvent invalid_item_click(Point(), combo->m_button);
	combo->on_item_click(combo->m_button, invalid_item_click);
	assert(combo->selected_index() == 4);
	assert(combo->input() == String("Fifth option"));

	combo->select(2);
	combo->text("Custom unmatched value");
	assert(combo->selected_item() == nullptr);
	assert(combo->open());
	assert(combo->m_list->selected_item() == nullptr);
	combo->close();

	ClickEvent button_click(Point(), combo->m_button);
	combo->on_button_click(combo->m_button, button_click);
	assert(combo->is_open());
	combo->on_button_click(combo->m_button, button_click);
	assert(combo->is_open() == false);

	combo->size(300, 200);
	combo->button_width(32);
	combo->max_columns(1);
	combo->max_lines(1);
	combo->text("X");
	combo->content_size();
	Area placement_area(0, 0, 640, 480);
	bool in_layout = false;
	combo->place(placement_area, in_layout);
	assert(combo->m_contentclip.width_q6() == combo->m_text_size.width_q6());
	assert(combo->m_contentclip.height_q6() == combo->m_text_size.height_q6());
	UIManager::desktop()->dispatch("$(ui.tests)/out/combo3_5.svg");

	Region parent_region(window.foreclip());
	const Area & button_area = combo->m_button->foreclip();
	Point button_position(
		button_area.x() + (Coord)(button_area.width() / 2),
		button_area.y() + (Coord)(button_area.height() / 2));
	assert(combo->hovered(parent_region, button_position) == combo->m_button);

	Point text_position(
		combo->foreclip().x() + 2,
		combo->foreclip().y() + (Coord)(combo->foreclip().height() / 2));
	assert(combo->hovered(parent_region, text_position) == combo);
	Point outside_position(combo->foreclip().x() + combo->foreclip().width() + 1, text_position.y());
	assert(combo->hovered(parent_region, outside_position) == nullptr);

	combo->m_button->hidden(true);
	assert(combo->hovered(parent_region, button_position) == combo);
	combo->m_button->hidden(false);

	combo->size(8, 8);
	combo->button_width(40);
	UIManager::desktop()->dispatch("$(ui.tests)/out/combo3_6.svg");
	assert(combo->m_contentclip.width_q6() == 0);
	assert(combo->m_button->foreclip().width_q6() <= combo->m_foreclip.width_q6());
}