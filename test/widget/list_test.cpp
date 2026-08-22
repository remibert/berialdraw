#include "berialdraw_imp.hpp"
using namespace berialdraw;

/** Test 1: Basic list creation with items */
void List::test1()
{
}

/** Test 2: List with selection */
void List::test2()
{
	MemoryLeakLog
}

void on_click(Widget* widget, const ClickEvent& evt)
{
	List* list = dynamic_cast<List *>(widget->parent()->search(123));
	if (list)
	{
		list->append("A");
	}
}


/** Test 3: List with enabled/disabled items */
void List::test3()
{
	MemoryLeakLog
	Window window;
	window.color(Color::LIGHT_GRAY);

	ScrollView* scroll = new ScrollView(&window);
	Column* column = new Column(scroll);
	List* list = new List(column);
		
		list->size(3000, 50);
		list->margin(10);
		list->id(123);
		scroll->align(Align::ALIGN_TOP);

		//list->append("A");
		//list->append("A");
		//list->append("A");
		//list->append("A");
		//list->append("A");
		//list->append("A");
		//list->append("A");
		//list->append("A");

		Button* button = new Button(column);
		button->text("+");
		button->bind(on_click);

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
	list->prepend("2");
	list->prepend("1");
	list->prepend("0");
	list->insert(-1, "A");
	list->insert(-3, "B");
	list->insert(-5, "C");
	list->insert(-7, "D");
	list->insert(-9, "-");

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
