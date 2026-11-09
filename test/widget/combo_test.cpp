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
	//UIManager::desktop()->mainloop();

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

	assert(combo->open());
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