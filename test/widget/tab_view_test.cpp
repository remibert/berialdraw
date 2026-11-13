#include "berialdraw_imp.hpp"
using namespace berialdraw;

#ifdef _DEBUG
void TabView::test2()
{
	// Missing optional fields retain their defaults; unknown directions are ignored.
	TabViewStyle defaults;
	Json empty_json;
	JsonIterator empty_iterator(empty_json);
	defaults.unserialize(empty_iterator);
	assert(defaults.tab_direction() == TabDirection::TAB_HORIZONTAL);
	assert(defaults.tab_margin() == Margin());
	assert(defaults.tab_inter_margin() == Margin());
	assert(defaults.horizontal_tab_style().borders() == ALL_BORDERS);
	assert(defaults.vertical_tab_style().borders() == ALL_BORDERS);
	Json partial_json;
	partial_json[StyleNames::TABVIEW_TAB_DIRECTION] = "diagonal";
	JsonIterator partial_horizontal = partial_json[StyleNames::TABVIEW_HORIZONTAL_STYLE];
	JsonIterator partial_margin = partial_horizontal[StyleNames::TABSTYLE_MARGIN];
	partial_margin[StyleNames::MARGIN_TOP] = 1;
	partial_margin[StyleNames::MARGIN_LEFT] = 2;
	partial_margin[StyleNames::MARGIN_BOTTOM] = 3;
	partial_margin[StyleNames::MARGIN_RIGHT] = 4;
	partial_horizontal[StyleNames::WIDGET_BORDERS] = "none";
	JsonIterator partial_iterator(partial_json);
	defaults.unserialize(partial_iterator);
	assert(defaults.tab_direction() == TabDirection::TAB_HORIZONTAL);
	assert(defaults.horizontal_tab_style().margin() == Margin(1, 2, 3, 4));
	assert(defaults.horizontal_tab_style().borders() == NO_BORDER);
	assert(defaults.vertical_tab_style().borders() == ALL_BORDERS);
	Json horizontal_json;
	horizontal_json[StyleNames::TABVIEW_TAB_DIRECTION] = "horizontal";
	JsonIterator horizontal_iterator(horizontal_json);
	TabViewStyle horizontal_style;
	horizontal_style.unserialize(horizontal_iterator);
	assert(horizontal_style.tab_direction() == TabDirection::TAB_HORIZONTAL);

	Style * created_style = TabViewStyle::create();
	assert(dynamic_cast<TabViewStyle *>(created_style) != 0);
	delete created_style;

	// Round-trip all TabViewStyle properties, including named border keys.
	TabViewStyle style;
	style.tab_direction(TabDirection::TAB_VERTICAL);
	style.horizontal_tab_style().margin(Margin(3, 5, 7, 9));
	style.horizontal_tab_style().inter_margin(Margin(2, 4, 6, 8));
	style.horizontal_tab_style().borders(TOP_BORDER | RIGHT_ANGLE_END);
	style.vertical_tab_style().margin(Margin(11, 13, 15, 17));
	style.vertical_tab_style().inter_margin(Margin(10, 12, 14, 16));
	style.vertical_tab_style().borders(LEFT_BORDER | ROUNDED_END);
	style.tab_direction(TabDirection::TAB_VERTICAL);
	style.tab_margin(style.tab_margin());
	style.tab_inter_margin(style.tab_inter_margin());

	Json json;
	JsonIterator iterator(json);
	style.serialize(iterator);
	TabViewStyle restored;
	restored.unserialize(iterator);
	assert(restored.tab_direction() == TabDirection::TAB_VERTICAL);
	assert(restored.horizontal_tab_style().margin() == Margin(3, 5, 7, 9));
	assert(restored.horizontal_tab_style().inter_margin() == Margin(2, 4, 6, 8));
	assert(restored.horizontal_tab_style().borders() == (TOP_BORDER | RIGHT_ANGLE_END));
	assert(restored.vertical_tab_style().margin() == Margin(11, 13, 15, 17));
	assert(restored.vertical_tab_style().inter_margin() == Margin(10, 12, 14, 16));
	assert(restored.vertical_tab_style().borders() == (LEFT_BORDER | ROUNDED_END));

	TabViewStyle copied;
	copied = restored;
	assert(copied.tab_direction() == restored.tab_direction());
	assert(copied.horizontal_tab_style() == restored.horizontal_tab_style());
	assert(copied.vertical_tab_style() == restored.vertical_tab_style());
	copied.set(copied);
	assert(copied.tab_direction() == restored.tab_direction());

	// Verify the visual layout when existing tabs change orientation.
	Window window;
	window.size(420, 300);
	window.color(Color::SNOW);
	TabView * tabs = new TabView(&window);
	tabs->horizontal_tab_style().margin(Margin(2, 3, 4, 5));
	tabs->horizontal_tab_style().inter_margin(Margin(2, 3, 4, 5));
	tabs->horizontal_tab_style().borders(TOP_BORDER);
	tabs->vertical_tab_style().margin(Margin(11, 7, 9, 5));
	tabs->vertical_tab_style().inter_margin(Margin(3, 5, 7, 9));
	tabs->vertical_tab_style().borders(LEFT_BORDER);
	Pane * first_page = tabs->add_tab("First");
	Pane * second_page = tabs->add_tab("Second");
	Label * first_label = new Label(first_page);
	first_label->text("Horizontal tab layout");
	Label * second_label = new Label(second_page);
	second_label->text("The selected page follows the tab");
	assert(tabs->tab_button(0)->borders() == TOP_BORDER);
	assert(tabs->tab_button(1)->borders() == TOP_BORDER);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view2_1.svg");

	tabs->tab_direction(TabDirection::TAB_VERTICAL);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view2_2.svg");
	assert(tabs->tab_button(0)->row() == 0);
	assert(tabs->tab_button(0)->column() == 0);
	assert(tabs->tab_button(1)->row() == 1);
	assert(tabs->tab_button(1)->column() == 0);
	assert(tabs->page(0)->row() == 0);
	assert(tabs->page(0)->column() == 1);
	assert(tabs->tab_margin() == Margin(11, 7, 9, 5));
	assert(tabs->tab_inter_margin() == Margin(3, 5, 7, 9));
	assert(tabs->tab_button(0)->margin() == Margin(3, 5, 7, 9));
	assert(tabs->tab_button(1)->margin() == Margin(3, 5, 7, 9));
	assert(tabs->tab_button(0)->borders() == LEFT_BORDER);

	tabs->vertical_tab_style().margin(Margin(13, 9, 7, 5));
	tabs->vertical_tab_style().inter_margin(Margin(4, 6, 8, 10));
	tabs->vertical_tab_style().borders(RIGHT_BORDER);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view2_3.svg");
	assert(tabs->tab_margin() == Margin(13, 9, 7, 5));
	assert(tabs->tab_inter_margin() == Margin(4, 6, 8, 10));
	assert(tabs->tab_button(0)->margin() == Margin(4, 6, 8, 10));
	assert(tabs->tab_button(1)->margin() == Margin(4, 6, 8, 10));
	assert(tabs->tab_button(0)->borders() == RIGHT_BORDER);
	assert(tabs->tab_button(1)->borders() == RIGHT_BORDER);

	tabs->add_tab("New vertical tab");
	assert(tabs->tab_button(2)->row() == 2);
	assert(tabs->tab_button(2)->column() == 0);
	assert(tabs->tab_button(2)->borders() == RIGHT_BORDER);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view2_4.svg");

	tabs->tab_direction(TabDirection::TAB_HORIZONTAL);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view2_5.svg");
	assert(tabs->tab_margin() == Margin(2, 3, 4, 5));
	assert(tabs->tab_inter_margin() == Margin(2, 3, 4, 5));
	assert(tabs->tab_button(0)->margin() == Margin(2, 3, 4, 5));
	assert(tabs->tab_button(0)->borders() == TOP_BORDER);
	assert(tabs->tab_button(0)->row() == 0);
	assert(tabs->tab_button(0)->column() == 0);
	assert(tabs->tab_button(1)->row() == 0);
	assert(tabs->tab_button(1)->column() == 1);
	assert(tabs->page(0)->row() == 1);
	assert(tabs->page(0)->column() == 0);
}

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
	assert(tabs->style_cascade_mode() == StyleCascadeMode::NONE);
	assert(tabs->select(4) == false);
	Json widget_json;
	JsonIterator widget_iterator(widget_json);
	tabs->serialize(widget_iterator);
	tabs->unserialize(widget_iterator);
	assert(tabs->focusable() == false);
	KeyEvent key_up((wchar_t)ReservedKey::KEY_RIGHT, KeyEvent::KEY_UP);
	tabs->on_key_tab(0, key_up);
	KeyEvent right_key((wchar_t)ReservedKey::KEY_RIGHT, KeyEvent::KEY_DOWN);
	tabs->on_key_tab(0, right_key);
	assert(tabs->selected_index() == 0);
	assert(tabs->select_tab(0, true));
	assert(tabs->tab_button(0)->focused());
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view3_1.svg");

	// Ignore null and non-tab click targets.
	ClickEvent click_event(Point(), tabs->tab_button(1));
	tabs->on_click_tab(0, click_event);
	Label * non_tab = new Label(tabs->page(0));
	non_tab->id(1);
	tabs->on_click_tab(non_tab, click_event);
	assert(tabs->selected_index() == 0);

	// Click a tab, then move focus through horizontal and vertical arrow navigation.
	Button * bravo = tabs->tab_button(1);
	UIManager::notifier()->click(bravo->foreclip().x() + (Coord)(bravo->foreclip().width() / 2),
		bravo->foreclip().y() + (Coord)(bravo->foreclip().height() / 2), bravo);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 1);
	assert(bravo->focused());
	tabs->on_click_tab(bravo, click_event);
	assert(tabs->selected_index() == 1);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view3_2.svg");

	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_RIGHT, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 2);
	assert(tabs->tab_button(2)->focused());
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_LEFT, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 1);
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_LEFT, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 0);
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_LEFT, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 3);
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_RIGHT, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 0);
	KeyEvent unrelated_key(L'x', KeyEvent::KEY_DOWN);
	tabs->on_key_tab(0, unrelated_key);

	tabs->tab_direction(TabDirection::TAB_VERTICAL);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view3_3.svg");
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_UP, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 3);
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_DOWN, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 0);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view3_4.svg");

	// Removing a selected tab with focus in its page selects and focuses the next tab.
	assert(tabs->select(2));
	window.focus_to(page_edits[2]);
	assert(page_edits[2]->focused());
	assert(tabs->remove_tab(2));
	assert(tabs->selected_index() == 2);
	assert(tabs->tab_button(2)->text() == "Delta");
	assert(tabs->tab_button(2)->focused());
	assert(tabs->selected_page() == tabs->page(2));
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view3_5.svg");

	// Removing the selected last tab falls back to the previous tab.
	assert(tabs->remove_tab(2));
	assert(tabs->selected_index() == 1);
	assert(tabs->tab_button(1)->text() == "Bravo");
	assert(tabs->tab_button(1)->focused());

	// A focused descendant on a removed non-selected page returns focus to the selected tab.
	window.focus_to(page_edits[0]);
	assert(page_edits[0]->focused());
	assert(tabs->remove_tab(0));
	assert(tabs->selected_index() == 0);
	assert(tabs->selected_page() == tabs->page(0));
	assert(tabs->tab_button(0)->text() == "Bravo");
	assert(tabs->tab_button(0)->focused());
	assert(tabs->tab_count() == 1);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view3_6.svg");

	// Remove the sole selected tab by title, then verify empty and reinitialized states.
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
	assert(tabs->tab_count() == 1);
	assert(tabs->selected_index() == 0);
	assert(tabs->selected_page() == tabs->page(0));
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view3_7.svg");

	// Removing the selected tab without focus does not assign focus to its replacement.
	tabs->add_tab("No-focus replacement");
	assert(outside_focus->focused());
	assert(tabs->select(1));
	assert(outside_focus->focused());
	assert(tabs->tab_button(1)->focused() == false);
	assert(tabs->remove_tab(1));
	assert(tabs->selected_index() == 0);
	assert(tabs->tab_button(0)->focused() == false);

	// Removing a later non-selected tab leaves the earlier selection unchanged.
	tabs->add_tab("Trailing tab");
	assert(tabs->selected_index() == 0);
	assert(tabs->remove_tab(1));
	assert(tabs->tab_count() == 1);
	assert(tabs->selected_index() == 0);
	KeyEvent single_tab_key((wchar_t)ReservedKey::KEY_DOWN, KeyEvent::KEY_DOWN);
	tabs->on_key_tab(0, single_tab_key);
	assert(tabs->selected_index() == 0);
	UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view3_8.svg");
}

void TabView::test()
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
#endif