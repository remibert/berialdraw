#include "berialdraw_imp.hpp"
using namespace berialdraw;

#ifdef _DEBUG
void TabView::test1()
{
	Window window;
	window.size(420, 300);
	window.color(Color::SNOW);
	TabView * tabs = new TabView(&window);
	Pane * first_page = tabs->add_tab("Overview");
	Pane * selected_page = tabs->add_tab("Details");
	tabs->add_tab("Settings");
	Label * first_label = new Label(first_page);
	first_label->text("Overview page");
	Label * selected_label = new Label(selected_page);
	selected_label->text("Selected page remains associated with Details");
	Edit * page_edit = new Edit(selected_page);
	page_edit->text("Focus moved into page");
	page_edit->focusable(true);

	assert(tabs->tab_count() == 3);
	assert(tabs->selected_index() == 0);
	assert(tabs->select(1));
	assert(tabs->selected_page() == selected_page);
	assert(tabs->tab_button(1)->selected());

	window.focus_to(tabs->tab_button(1));
	assert(tabs->tab_button(1)->focused());
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view1_1.svg");

	window.focus_to(page_edit);
	assert(page_edit->focused());
	assert(tabs->selected_index() == 1);
	assert(tabs->tab_button(1)->selected());
	assert(tabs->tab_button(1)->focused() == false);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view1_2.svg");
}

void TabView::test2()
{
	// Unspecified properties retain their defaults and invalid positions are ignored.
	TabViewStyle defaults;
	Json empty_json;
	JsonIterator empty_iterator(empty_json);
	defaults.unserialize(empty_iterator);
	assert(defaults.tab_position() == TabPosition::TOP);
	assert(defaults.tab_margin() == Margin());

	Json partial_json;
	partial_json[StyleNames::TABVIEW_TAB_POSITION] = "diagonal";
	JsonIterator partial_margin = partial_json[StyleNames::TABVIEW_TAB_MARGIN];
	partial_margin[StyleNames::MARGIN_TOP] = 1;
	partial_margin[StyleNames::MARGIN_LEFT] = 2;
	partial_margin[StyleNames::MARGIN_BOTTOM] = 3;
	partial_margin[StyleNames::MARGIN_RIGHT] = 4;
	JsonIterator partial_iterator(partial_json);
	defaults.unserialize(partial_iterator);
	assert(defaults.tab_position() == TabPosition::TOP);
	assert(defaults.tab_margin() == Margin(1, 2, 3, 4));

	Json position_json;
	position_json[StyleNames::TABVIEW_TAB_POSITION] = "left";
	JsonIterator position_iterator(position_json);
	TabViewStyle position_style;
	position_style.unserialize(position_iterator);
	assert(position_style.tab_position() == TabPosition::LEFT);

	Style * created_style = TabViewStyle::create();
	assert(dynamic_cast<TabViewStyle *>(created_style) != 0);
	delete created_style;

	// Verify cardinal border and margin rotation, preserving Q6 values and decorations.
	Borders base_borders = (Borders)(TOP_BORDER | LEFT_BORDER | RIGHT_BORDER | RIGHT_ANGLE_END);
	assert(Tab::rotate_borders(base_borders, TabPosition::TOP) == base_borders);
	assert(Tab::rotate_borders(base_borders, TabPosition::RIGHT) ==
		(Borders)(TOP_BORDER | RIGHT_BORDER | BOTTOM_BORDER | RIGHT_ANGLE_END));
	assert(Tab::rotate_borders(base_borders, TabPosition::BOTTOM) ==
		(Borders)(LEFT_BORDER | RIGHT_BORDER | BOTTOM_BORDER | RIGHT_ANGLE_END));
	assert(Tab::rotate_borders(base_borders, TabPosition::LEFT) ==
		(Borders)(TOP_BORDER | BOTTOM_BORDER | LEFT_BORDER | RIGHT_ANGLE_END));
	assert(Tab::rotate_align(Align::ALIGN_BOTTOM, TabPosition::TOP) == Align::ALIGN_BOTTOM);
	assert(Tab::rotate_align(Align::ALIGN_BOTTOM, TabPosition::RIGHT) == Align::ALIGN_LEFT);
	assert(Tab::rotate_align(Align::ALIGN_BOTTOM, TabPosition::BOTTOM) == Align::ALIGN_TOP);
	assert(Tab::rotate_align(Align::ALIGN_BOTTOM, TabPosition::LEFT) == Align::ALIGN_RIGHT);
	assert(Tab::rotate_align(Align::CENTER, TabPosition::LEFT) == Align::CENTER);
	assert(Tab::rotate_align(Align::ALIGN_DEFAULT, TabPosition::RIGHT) == Align::ALIGN_DEFAULT);
	Margin canonical_margin(1, 2, 3, 4);
	assert(Tab::rotate_margin(canonical_margin, TabPosition::RIGHT) == Margin(2, 3, 4, 1));
	assert(Tab::rotate_margin(canonical_margin, TabPosition::BOTTOM) == Margin(3, 4, 1, 2));
	assert(Tab::rotate_margin(canonical_margin, TabPosition::LEFT) == Margin(4, 1, 2, 3));

	// Round-trip the position and canonical tab-view margin.
	TabViewStyle style;
	style.tab_position(TabPosition::LEFT);
	style.tab_margin(Margin(3, 5, 7, 9));
	Json json;
	JsonIterator iterator(json);
	style.serialize(iterator);
	TabViewStyle restored;
	restored.unserialize(iterator);
	assert(restored.tab_position() == TabPosition::LEFT);
	assert(restored.tab_margin() == Margin(3, 5, 7, 9));
	TabViewStyle copied;
	copied = restored;
	assert(copied.tab_position() == restored.tab_position());
	assert(copied.tab_margin() == restored.tab_margin());
	copied.set(copied);
	assert(copied.tab_position() == restored.tab_position());

	// Verify TabView serializes and restores its own style properties.
	TabView serialized_tabs(0);
	serialized_tabs.tab_position(TabPosition::RIGHT);
	serialized_tabs.tab_margin(Margin(3, 5, 7, 9));
	Json tab_view_json;
	JsonIterator tab_view_iterator(tab_view_json);
	serialized_tabs.serialize(tab_view_iterator);
	serialized_tabs.tab_position(TabPosition::BOTTOM);
	serialized_tabs.tab_margin(Margin(11, 13, 15, 17));
	serialized_tabs.unserialize(tab_view_iterator);
	assert(serialized_tabs.tab_position() == TabPosition::RIGHT);
	assert(serialized_tabs.tab_margin() == Margin(3, 5, 7, 9));

	// Verify all four positions rotate the one tab.sty base style.
	Window window;
	window.size(420, 300);
	window.color(Color::SNOW);
	TabView * tabs = new TabView(&window);
	tabs->tab_margin(Margin(2, 3, 4, 5));
	Pane * first_page = tabs->add_tab("First");
	Pane * second_page = tabs->add_tab("Second");
	Label * first_label = new Label(first_page);
	first_label->text("Top tab layout");
	Label * second_label = new Label(second_page);
	second_label->text("Selected page follows its tab");
	assert(String(tabs->tab_button(0)->classname()) == "tab");
	assert(tabs->tab_button(0)->borders() == base_borders);
	assert(tabs->tab_button(0)->margin() == Margin(5, 3, 0, 3));
	assert(tabs->tab_button(0)->align() == Align::ALIGN_BOTTOM);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view2_1.svg");

	// RIGHT: rotate clockwise and reset a per-tab visual customization.
	tabs->tab_button(0)->borders(NO_BORDER);
	tabs->tab_position(TabPosition::RIGHT);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view2_2.svg");
	assert(tabs->tab_button(0)->row() == 0);
	assert(tabs->tab_button(0)->column() == 0);
	assert(tabs->tab_button(1)->row() == 1);
	assert(tabs->tab_button(1)->column() == 0);
	assert(tabs->tab_button(0)->text() == "First");
	assert(tabs->tab_button(1)->text() == "Second");
	assert(tabs->page(0)->row() == 0);
	assert(tabs->page(0)->column() == 0);
	assert(tabs->tab_button(0)->margin() == Margin(3, 0, 3, 5));
	assert(tabs->tab_button(0)->borders() == Tab::rotate_borders(base_borders, TabPosition::RIGHT));
	assert(tabs->tab_button(0)->align() == Align::ALIGN_LEFT);

	tabs->tab_position(TabPosition::BOTTOM);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view2_3.svg");
	assert(tabs->tab_button(0)->row() == 0);
	assert(tabs->tab_button(0)->column() == 0);
	assert(tabs->tab_button(1)->row() == 0);
	assert(tabs->tab_button(1)->column() == 1);
	assert(tabs->page(0)->row() == 0);
	assert(tabs->tab_button(0)->margin() == Margin(0, 3, 5, 3));
	assert(tabs->tab_button(0)->borders() == Tab::rotate_borders(base_borders, TabPosition::BOTTOM));
	assert(tabs->tab_button(0)->align() == Align::ALIGN_TOP);

	tabs->tab_position(TabPosition::LEFT);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view2_4.svg");
	assert(tabs->tab_button(0)->row() == 0);
	assert(tabs->tab_button(0)->column() == 0);
	assert(tabs->tab_button(1)->row() == 1);
	assert(tabs->tab_button(1)->column() == 0);
	assert(tabs->tab_button(0)->text() == "First");
	assert(tabs->tab_button(1)->text() == "Second");
	assert(tabs->page(0)->row() == 0);
	assert(tabs->page(0)->column() == 1);
	assert(tabs->tab_margin() == Margin(2, 3, 4, 5));
	assert(tabs->tab_button(0)->margin() == Margin(3, 5, 3, 0));
	assert(tabs->tab_button(0)->borders() == Tab::rotate_borders(base_borders, TabPosition::LEFT));
	assert(tabs->tab_button(0)->align() == Align::ALIGN_RIGHT);

	tabs->add_tab("New left tab");
	assert(tabs->tab_button(2)->row() == 2);
	assert(tabs->tab_button(2)->column() == 0);
	assert(tabs->tab_button(2)->borders() == Tab::rotate_borders(base_borders, TabPosition::LEFT));
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view2_5.svg");
}

void TabView::test3()
{
	Window window;
	window.size(480, 320);
	window.color(Color::SNOW);
	TabView * tabs = new TabView(&window);
	const char * titles[] = {"Alpha", "Bravo", "Charlie", "Delta"};
	Edit * page_edits[4] = {0, 0, 0, 0};
	for (Dim index = 0; index < 4; index++)
	{
		Pane * tab_page = tabs->add_tab(titles[index]);
		Column * content = new Column(tab_page);
		Label * label = new Label(content);
		label->text("Page %u", (unsigned)(index + 1));
		page_edits[index] = new Edit(content);
		page_edits[index]->text("Focusable page field");
		page_edits[index]->focusable(true);
	}
	assert(tabs->selected_index() == 0);
	assert(tabs->selected_page() == tabs->page(0));
	assert(tabs->page(4) == 0);
	assert(tabs->tab_button(4) == 0);
	assert(tabs->select(4) == false);

	Button * bravo = tabs->tab_button(1);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view3_1.svg");
	UIManager::notifier()->click(bravo->foreclip().x() + (Coord)(bravo->foreclip().width() / 2),
		bravo->foreclip().y() + (Coord)(bravo->foreclip().height() / 2), bravo);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 1);
	assert(bravo->focused());
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view3_2.svg");

	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_RIGHT, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 2);
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_LEFT, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 1);
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_LEFT, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 0);
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_LEFT, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 3);

	tabs->tab_position(TabPosition::LEFT);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view3_3.svg");
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_UP, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 2);
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_DOWN, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 3);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view3_4.svg");

	// Removing selected tabs transfers focus and selects the next or previous tab.
	assert(tabs->select(2));
	window.focus_to(page_edits[2]);
	assert(page_edits[2]->focused());
	assert(tabs->remove_tab(2));
	assert(tabs->selected_index() == 2);
	assert(tabs->tab_button(2)->text() == "Delta");
	assert(tabs->tab_button(2)->focused());
	assert(tabs->selected_page() == tabs->page(2));
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view3_5.svg");
	assert(tabs->remove_tab(2));
	assert(tabs->selected_index() == 1);
	assert(tabs->tab_button(1)->text() == "Bravo");
	assert(tabs->tab_button(1)->focused());

	// Removing a tab before the selection renumbers the remaining pages and buttons.
	window.focus_to(page_edits[0]);
	assert(page_edits[0]->focused());
	assert(tabs->remove_tab(0));
	assert(tabs->selected_index() == 0);
	assert(tabs->tab_button(0)->text() == "Bravo");
	assert(tabs->tab_button(0)->focused());
	assert(tabs->tab_count() == 1);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view3_6.svg");

	Edit * outside_focus = new Edit(&window);
	outside_focus->hidden(true);
	outside_focus->focusable(true);
	window.focus_to(outside_focus);
	assert(outside_focus->focused());
	assert(tabs->remove_tab(String("Bravo")));
	assert(tabs->tab_count() == 0);
	assert(tabs->selected_index() == UNDEFINED_INDEX);
	assert(tabs->remove_tab(0) == false);
	assert(tabs->remove_tab(String("Missing")) == false);
	assert(tabs->select(0) == false);
	tabs->add_tab("After removal");
	assert(tabs->selected_index() == 0);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view3_7.svg");
}

void TabView::test4()
{
	Window window;
	window.color(Color::SNOW);
	TabView * tabs = new TabView(&window);
	const char * titles[] = {"Albertina", "Bruno", "Charles", "Didier"};
	for (Dim index = 0; index < 4; index++)
	{
		Pane * tab_page = tabs->add_tab(titles[index]);
		Column * content = new Column(tab_page);
		Label * label = new Label(content);
		label->text("Page %s", titles[index]);
		Edit * edit = new Edit(content);
		edit->text(titles[index]);
	}

	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view4_1.svg");
	tabs->tab_position(TabPosition::BOTTOM);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view4_2.svg");
	tabs->tab_position(TabPosition::RIGHT);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view4_3.svg");
	Dim tab_width = tabs->tab_button(0)->foreclip().width_q6();
	for (Dim index = 1; index < tabs->tab_count(); index++)
	{
		assert(tabs->tab_button(index)->foreclip().width_q6() == tab_width);
	}

	tabs->tab_position(TabPosition::LEFT);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view4_4.svg");
	tab_width = tabs->tab_button(0)->foreclip().width_q6();
	for (Dim index = 1; index < tabs->tab_count(); index++)
	{
		assert(tabs->tab_button(index)->foreclip().width_q6() == tab_width);
	}

	tabs->tab_position(TabPosition::TOP);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view4_5.svg");
}

void TabView::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test4();
		test3();
		test2();
		test1();
	}
}
#endif