#include "berialdraw_imp.hpp"
using namespace berialdraw;

#ifdef _DEBUG
void TabView::test1()
{
	Window window;
		window.position(0, 0);
		// window.size(320, 220);
		window.color(Color::SNOW);

	TabView * tabs = new TabView(&window);
		// tabs->size_policy(SizePolicy::ENLARGE_ALL);
	assert(tabs->tab_direction() == TabViewStyle::TabDirection::TAB_HORIZONTAL);
	tabs->tab_direction(TabViewStyle::TabDirection::TAB_VERTICAL);
	assert(tabs->tab_direction() == TabViewStyle::TabDirection::TAB_VERTICAL);
	tabs->tab_direction(TabViewStyle::TabDirection::TAB_HORIZONTAL);
	// assert(tabs->tab_margin().top() == 10);
	// assert(tabs->tab_margin().left() == 10);
	// assert(tabs->tab_inter_margin().top() == 5);
	// assert(tabs->tab_inter_margin().left() == 3);
	// assert(tabs->tab_inter_margin().bottom() == 0);
	// assert(tabs->tab_inter_margin().right() == 3);
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
	}

	// assert(tabs->tab_count() == 12);
	// assert(tabs->selected_index() == 0);
	// assert(tabs->selected_page() == tabs->page(0));
	// assert(tabs->page(0)->hidden() == false);
	// assert(tabs->page(1)->hidden());
	// UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view1_1.svg");

	// assert(tabs->select(5));
	// assert(tabs->selected_index() == 5);
	// assert(tabs->selected_page() == tabs->page(5));
	// assert(tabs->page(5)->hidden() == false);
	// assert(tabs->page(0)->hidden());
	// UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view1_2.svg");

	// assert(tabs->select(11));
	// UIManager::desktop()->dispatch("$(ui.tests)/out/tab_view1_3.svg");
// UIManager::desktop()->mainloop();

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
		test1();
	}
}
#endif