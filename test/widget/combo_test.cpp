#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Combo::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
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

	assert(combo->input() == String("First"));
	assert(combo->selected_item() == 0);
	assert(combo->selected_index() == -1);
	combo->editable(true);
	combo->typing_mode(Entry::TypingMode::READ_ONLY);

	assert(combo->selection_start() == combo->input().count());
	assert(combo->selection_end() == combo->input().count());

	assert(combo->count() == 2);
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
	assert(combo->selected_index() == 1);

	combo->text("custom value");
	assert(combo->selected_item() == 0);
	combo->select(1);

	combo->editable(false);
	assert(combo->typing_mode() == Entry::TypingMode::READ_ONLY);

	combo->editable(true);
	assert(combo->typing_mode() == Entry::TypingMode::INSERTION);

	assert(combo->open());
	assert(combo->is_open());
	combo->close();
	assert(combo->is_open() == false);
}