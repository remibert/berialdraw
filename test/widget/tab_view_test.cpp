#include "berialdraw_imp.hpp"
using namespace berialdraw;

#ifdef _DEBUG
void TabView::test2()
{
	Window window;
	TabView * tabs = new TabView(&window);
	tabs->align(Align::ALIGN_RIGHT);
	tabs->text_align(Align::ALIGN_LEFT);
	tabs->tab_horizontal_borders(TOP_BORDER);
	tabs->tab_vertical_borders(LEFT_BORDER);
	tabs->add_tab("First");
	tabs->add_tab("Second");
	tabs->add_tab("Third");
	tabs->tab_button(0)->align(Align::ALIGN_LEFT);
	tabs->tab_button(0)->text_align(Align::ALIGN_RIGHT);
	tabs->tab_button(0)->borders(BOTTOM_BORDER);
	tabs->tab_horizontal_borders(RIGHT_BORDER);
	tabs->add_tab("Fourth");
	assert(tabs->tab_count() == 4);
	assert(tabs->selected_index() == 0);
	assert(tabs->tab_button(0)->align() == Align::ALIGN_LEFT);
	assert(tabs->tab_button(0)->text_align() == Align::ALIGN_RIGHT);
	assert(tabs->tab_button(0)->borders() == BOTTOM_BORDER);
	assert(tabs->tab_button(1)->borders() == TOP_BORDER);
	assert(tabs->tab_button(2)->borders() == TOP_BORDER);
	assert(tabs->tab_button(3)->borders() == RIGHT_BORDER);
	assert(tabs->align() == Align::ALIGN_RIGHT);
	assert(tabs->text_align() == Align::ALIGN_LEFT);

	assert(tabs->select(2));
	assert(tabs->tab_button(0)->align() == Align::ALIGN_LEFT);
	assert(tabs->tab_button(0)->text_align() == Align::ALIGN_RIGHT);
	assert(tabs->tab_button(0)->borders() == BOTTOM_BORDER);
	assert(tabs->remove_tab(1));
	assert(tabs->tab_count() == 3);
	assert(tabs->selected_index() == 1);
	assert(tabs->page(1) != 0);
	assert(tabs->page(1)->hidden() == false);
	assert(tabs->tab_button(1)->text() == "Third");

	assert(tabs->remove_tab(String("Third")));
	assert(tabs->tab_count() == 2);
	assert(tabs->selected_index() == 1);
	assert(tabs->page(1)->hidden() == false);
	assert(tabs->remove_tab(0));
	assert(tabs->tab_count() == 1);
	assert(tabs->selected_index() == 0);
	assert(tabs->remove_tab(String("Fourth")));
	assert(tabs->tab_count() == 0);
	assert(tabs->selected_index() == UNDEFINED_INDEX);
	assert(tabs->remove_tab(0) == false);
	assert(tabs->remove_tab(String("Missing")) == false);

	tabs->tab_direction(TabDirection::TAB_VERTICAL);
	tabs->add_tab("After removal");
	assert(tabs->tab_count() == 1);
	assert(tabs->selected_index() == 0);
	assert(tabs->tab_button(0)->borders() == LEFT_BORDER);
}

void TabView::test1()
{
	Window window;
		// window.position(0, 0);
		// window.size(320, 220);
		window.color(Color::SNOW);

	TabView * tabs = new TabView(&window);
	Edit * second_page_edit = 0;
		// tabs->size_policy(SizePolicy::ENLARGE_ALL);
	// assert(tabs->tab_direction() == TabViewStyle::TabDirection::TAB_HORIZONTAL);
	// tabs->tab_direction(TabViewStyle::TabDirection::TAB_VERTICAL);
	// assert(tabs->tab_direction() == TabViewStyle::TabDirection::TAB_VERTICAL);
	// tabs->tab_direction(TabViewStyle::TabDirection::TAB_HORIZONTAL);
	assert(tabs->tab_margin().top() == 10);
	assert(tabs->tab_margin().left() == 10);
	assert(tabs->tab_inter_margin().top() == 5);
	assert(tabs->tab_inter_margin().left() == 3);
	assert(tabs->tab_inter_margin().bottom() == 0);
	assert(tabs->tab_inter_margin().right() == 3);
	// assert(tabs->thickness() == 1);
	// assert(tabs->radius() == 0);
	// assert(tabs->font_size().width() == 20);
	// assert(tabs->font_size().height() == 22);
	// assert((tabs->borders() & TOP_BORDER) == TOP_BORDER);
	// assert((tabs->borders() & LEFT_BORDER) == 0);
	// assert((tabs->borders() & RIGHT_ANGLE_WITHOUT_BORDER) == RIGHT_ANGLE_WITHOUT_BORDER);
	// assert(tabs->tab_count() == 0);
	// assert(tabs->selected_index() == UNDEFINED_INDEX);
	// assert(tabs->select(0) == false);

	for (Dim index = 0; index < 12; index++)
	{
		String title;
		title.print("Tab %02u", (unsigned)(index + 1));
		Pane * page = tabs->add_tab(title);
			assert(page != 0);
			Column * content = new Column(page);
				Label * label = new Label(content);
					label->text("Content for tab %02u", (unsigned)(index + 1));


				Edit * edit = new Edit(content);
				edit->focusable(true);
				if (index == 1)
				{
					second_page_edit = edit;
				}
				label = new Label(content);
				label->text("next field");
				edit = new Edit(content);
				edit->focusable(true);

	}

	assert(tabs->tab_count() == 12);
	assert(tabs->selected_index() == 0);
	assert(tabs->selected_page() == tabs->page(0));
	assert(tabs->page(0)->hidden() == false);
	assert(tabs->page(1)->hidden());
	// UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view1_1.svg");

	assert(tabs->select(5));
	assert(tabs->selected_index() == 5);
	assert(tabs->selected_page() == tabs->page(5));
	assert(tabs->page(5)->hidden() == false);
	assert(tabs->page(0)->hidden());
	// UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view1_2.svg");

	assert(tabs->select(11));
	assert(tabs->page(11)->hidden() == false);
	assert(tabs->page(5)->hidden());
	assert(tabs->select(12) == false);
	assert(tabs->selected_index() == 11);
	assert(tabs->select(0));

	Widget * first_tab = tabs->search(0x20000000);
	Widget * second_tab = tabs->search(0x20000001);
	assert(first_tab != 0);
	assert(second_tab != 0);
	assert(second_page_edit != 0);
	window.focus_to(first_tab);
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_RIGHT, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 1);
	assert(second_tab->focused());
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_TABULATION, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(second_page_edit->focused());
	assert(tabs->selected_index() == 1);
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_TABULATION, KeyEvent::KEY_DOWN, KeyEvent::MODIFIER_SHIFT);
	UIManager::notifier()->dispatch();
	assert(second_tab->focused());
	UIManager::notifier()->key((wchar_t)ReservedKey::KEY_LEFT, KeyEvent::KEY_DOWN);
	UIManager::notifier()->dispatch();
	assert(tabs->selected_index() == 0);
	assert(first_tab->focused());
	// UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view1_3.svg");
 UIManager::desktop()->mainloop();

	// assert(tabs->select(12) == false);
	// assert(tabs->selected_index() == 11);
	// assert(tabs->page(12) == 0);

	// tabs->clear_tabs();
	// assert(tabs->tab_count() == 0);
	// assert(tabs->selected_index() == UNDEFINED_INDEX);
	// Pane * page = tabs->add_tab(String("After clear"));
	// assert(page != 0);
	// assert(tabs->selected_index() == 0);
	// UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view1_4.svg");

}

void TabView::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		// test2();
		test1();
	}
}
#endif