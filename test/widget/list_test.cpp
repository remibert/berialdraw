#include "berialdraw_imp.hpp"
using namespace berialdraw;

/** Test 1: Basic list creation with items */
void List::test1()
{
	MemoryLeakLog
	Window window;
	List* list = new List(&window);

	// Test empty list
	assert(list->items().size() == 0);
	assert(list->selected_items().size() == 0);

	// Add items
	list->append("Item A");
	list->append("Item B");
	list->append("Item C");

	// Test items() returns all items
	assert(list->items().size() == 3);
	assert(list->selected_items().size() == 0);

	// Verify items content
	auto all_items = list->items();
	assert(all_items[0]->text() == "Item A");
	assert(all_items[1]->text() == "Item B");
	assert(all_items[2]->text() == "Item C");

	// Test negative indexing on items
	assert(all_items[-1]->text() == "Item C");
	assert(all_items[-2]->text() == "Item B");
	assert(all_items[-3]->text() == "Item A");

	UIManager::desktop()->dispatch("$(ui.tests)/out/list1_1.svg");
}

/** Test 2: List with selection */
void List::test2()
{
	MemoryLeakLog
	Window window;
	window.color(Color::LIGHT_GRAY);
	List* list = new List(&window);
	list->selection_mode(ListSelectionMode::LIST_MULTI_SELECTION);

	// Add items
	list->append("Option 1");
	list->append("Option 2");
	list->append("Option 3");
	list->append("Option 4");
	list->append("Option 5");

	// Test initial state: no selection
	assert(list->selected_items().size() == 0);
	assert(list->items().size() == 5);

	// Select first item
	list->select(0);
	assert(list->selected_items().size() == 1);
	assert(list->selected_items()[0]->text() == "Option 1");

	// Select third item (multi-selection enabled)
	list->select(2);
	assert(list->selected_items().size() == 2);

	// Select with negative index
	list->select(-1);
	assert(list->selected_items().size() == 3);
	auto selected = list->selected_items();
	assert(selected[-1]->text() == "Option 5");

	UIManager::desktop()->dispatch("$(ui.tests)/out/list2_1.svg");

	// Unselect middle item
	list->unselect(0);
	UIManager::desktop()->dispatch();
	assert(list->selected_items().size() == 2);

	UIManager::desktop()->dispatch("$(ui.tests)/out/list2_2.svg");

	// Unselect all
	list->unselect_all();
	assert(list->selected_items().size() == 0);
	assert(list->items().size() == 5);

	UIManager::desktop()->dispatch("$(ui.tests)/out/list2_3.svg");

	// Test single selection mode: selecting new item should unselect previous
	list->selection_mode(ListSelectionMode::LIST_SINGLE_SELECTION);
	list->select(0);
	assert(list->selected_items().size() == 1);
	list->select(2);
	assert(list->selected_items().size() == 1);
	assert(list->selected_items()[0]->text() == "Option 3");

	UIManager::desktop()->dispatch("$(ui.tests)/out/list2_4.svg");

	// Test no selection mode
	list->selection_mode(ListSelectionMode::LIST_NO_SELECTION);
	list->unselect_all();
	list->select(0);
	list->select(1);

	// Nothing should be selected in NO_SELECTION mode
	assert(list->selected_items().size() == 0);

	UIManager::desktop()->dispatch("$(ui.tests)/out/list2_5.svg");
}

static void on_click(Widget* widget, const ClickEvent& evt)
{
	List * list = dynamic_cast<List*>(widget->parent()->search(123));
	Edit * edit = dynamic_cast<Edit*>(widget->parent()->search(456));
	if (list && edit)
	{
		list->append(edit->text());
	}
}

/** Test 3: List with enabled/disabled items */
void List::test3()
{
	MemoryLeakLog
	Window window;

	//ScrollView* scroll = new ScrollView(&window);
	//Column* column = new Column(scroll);
	Column* column = new Column(&window);
	List* list = new List(column);
		
		//list->size(3000, 250);
		list->id(123);
		//scroll->align(Align::ALIGN_TOP);
		//list->extend(Extend::EXTEND_ALL);
		list->size_policy(SizePolicy::ENLARGE_ALL);
		list->selection_mode(ListSelectionMode::LIST_MULTI_SELECTION);

//list->thickness(4);
//list->padding(3);
		list->append("One");
		list->append("Two");
		list->append("Three");
		list->append("Four");
		list->append("Five");
		list->append("Six");
		list->append("Seven");

		Button* button = new Button(column);
			button->text("Add");
			button->bind(on_click);

		Edit* edit = new Edit(column);
			edit->id(456);
			edit->text("Height");

	UIManager::desktop()->mainloop();
}

/** Test 4: List with icons */
void List::test4()
{
	MemoryLeakLog
	Window window;
	window.color(Color::LIGHT_GRAY);
	List* list = new List(&window);

	assert(list->is_empty() == true);
	assert(list->count() == 0);
	assert(list->at(0) == nullptr);

	list->remove(0);


	list->append("0");
	assert(list->is_empty() == false);
	assert(list->count() == 1);

	list->append("1");
	assert(list->is_empty() == false);
	assert(list->count() == 2);

	list->append("2");
	assert(list->is_empty() == false);
	assert(list->count() == 3);

	list->remove(0);
	assert(list->is_empty() == false);
	assert(list->count() == 2);

	list->remove(-1);
	assert(list->is_empty() == false);
	assert(list->count() == 1);

	UIManager::desktop()->dispatch("$(ui.tests)/out/list4_1.svg");

}

/** Test 5: List with arrows */
void List::test5()
{
	MemoryLeakLog
	Window window;
	window.color(Color::LIGHT_GRAY);
	List* list = new List(&window);

	list->append("0");
	list->append("1");
	list->append("2");
	UIManager::desktop()->dispatch("$(ui.tests)/out/list5_1.svg");

	list->at(0)->text("A");
	list->at(1)->text("B");
	list->at(-1)->text("C");
	UIManager::desktop()->dispatch("$(ui.tests)/out/list5_2.svg");

	list->at(-10)->text("0");
	list->at(10)->text("2");
	UIManager::desktop()->dispatch("$(ui.tests)/out/list5_3.svg");

	(*list)[0]->text("a");
	(*list)[1]->text("b");
	(*list)[-1]->text("c");
	UIManager::desktop()->dispatch("$(ui.tests)/out/list5_4.svg");
}

/** Test 6: List with scrollbar (many items) */
void List::test6()
{
	MemoryLeakLog

	Window window;
	window.color(Color::LIGHT_GRAY);
	List* list = new List(&window);
	list->append("0");
	list->append("1");
	list->append("2");
	list->insert(0, "A");
	list->insert(2, "B");
	list->insert(4, "C");
	list->insert(6, "D");
	list->insert(11, "-");

	UIManager::desktop()->dispatch("$(ui.tests)/out/list6_1.svg");

	list->clear();
	list->prepend("2"); UIManager::desktop()->dispatch();
	list->prepend("1"); UIManager::desktop()->dispatch();
	list->prepend("0"); UIManager::desktop()->dispatch();
	list->insert(-1, "A"); UIManager::desktop()->dispatch();
	list->insert(-3, "B"); UIManager::desktop()->dispatch();
	list->insert(-5, "C"); UIManager::desktop()->dispatch();
	list->insert(-7, "D"); UIManager::desktop()->dispatch();
	list->insert(-9, "-"); UIManager::desktop()->dispatch();

	UIManager::desktop()->dispatch("$(ui.tests)/out/list6_2.svg");

	list->clear();

	list->append([](ListItem* item) {
		item->text("Color BLUE");
		item->color(Color::BLUE);
		item->text_color(Color::YELLOW);
		item->font_size(30, 25);
		item->trailing("$(ui.icons)/settings.icn");
		item->leading("$(ui.icons)/computer.icn");
		});

	list->prepend([](ListItem* item) {
		item->text("Color RED");
		item->color(Color::RED);
		item->text_color(Color::CYAN);
		item->font_size(30, 25);
		item->trailing("$(ui.icons)/settings.icn");
		item->leading("$(ui.icons)/computer.icn");
		});

	list->insert(1, [](ListItem* item) {
		item->text("Color GREEN");
		item->color(Color::GREEN);
		item->text_color(Color::MAUVE);
		item->font_size(30, 25);
		item->trailing("$(ui.icons)/settings.icn");
		item->leading("$(ui.icons)/computer.icn");
		});

	UIManager::desktop()->dispatch("$(ui.tests)/out/list6_3.svg");
}

/** Test 7: List with touch selection */
void List::test7()
{
	MemoryLeakLog

	Window window;
	window.color(Color::LIGHT_GRAY);
	Pane* pane = new Pane(&window);
		pane->size(200, 300);
		pane->position(173,113);
		pane->color(Color::ROSE);
		pane->radius(0);
		pane->thickness(0);
		pane->margin(60);

	List* list = new List(&window);

	list->thickness(0);
	list->border_color(Color::GRAY);
	list->radius(0);
	list->size(200, 300);
	list->position(173, 113);
	ListItem* item;

	for (int i = 0; i < 3; i++)
	{
		item = list->new_item();
			item->leading("< ");
			item->trailing(" >");
			item->text("abcdefg");

		item = list->new_item();
			item->font_size(40);
			item->trailing("$(ui.icons)/settings.icn");
			item->text("worldjkhmloj");
			item->leading("$(ui.icons)/settings.icn");
			item->text_color(Color::BLUE);

		item = list->new_item();
			item->leading("< ");
			item->text("bonjour");

		item = list->new_item();
			item->text("monde");

			item = list->new_item();
			item->trailing(" >");
			item->text("worldjkhmlojlllllllllllllllllllllllllllllll");
	}

	struct Test7 {
		Dim radius;
		Dim thickness;
		Dim padding;
	};

	Test7 test7[] = {
		{0,0,0},
		{1,1,1},
		{3,3,3},
		{3,5,7},
		{11,33,23},
		{47,33,0}
	};

	int id = 0;
	for (int i = 0; i < sizeof(test7) / sizeof(Test7); i++)
	{
		list->padding(test7[i].padding);
		list->thickness(test7[i].thickness);
		list->radius(test7[i].radius);
		{
			String name;
			name.print("$(ui.tests)/out/list7_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
	}
}

void List::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;

test6();
//test3();
		test7();
		test6();
		test5();
		test4();
//		test3();
		test2();
		test1();
	}
}
