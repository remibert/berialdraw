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

/** Test 3: List with enabled/disabled items */
void List::test3()
{
	MemoryLeakLog
}

/** Test 4: List with icons */
void List::test4()
{
	MemoryLeakLog
}

/** Test 5: List with arrows */
void List::test5()
{
	MemoryLeakLog
}

/** Test 6: List with scrollbar (many items) */
void List::test6()
{
	MemoryLeakLog
	//Window window;
	//window.color(Color::WHITE_BLUE);
	//
	//List* list = new List(&window);
	//list->size(200, 200);
	//list->position(50, 50);
	//list->scrollbar_visible(true);
	//
	//for (int i = 0; i < 20; i++)
	//{
	//	String text;
	//	text.print("Item %d", i + 1);
	//	list->add(text);
	//}
	//
	//// Scroll down
	//String script(
	//"["
	//	"{'type':'touch','x':150,'y':150,'state':'down'},"
	//	"{'type':'touch','x':150,'y':100,'state':'move'},"
	//	"{'type':'touch','x':150,'y':50,'state':'up'},"
	//	"{'type':'key','key':9208,'state':'press'}"
	//"]");
	//UIManager::notifier()->play_script(script, "$(ui.tests)/out/list6.svg");
}

/** Test 7: List with touch selection */
void List::test7()
{
	MemoryLeakLog

	//	//UIManager::styles()->style("pearl");
	//	//UIManager::colors()->appearance("light");
	//UIManager::styles()->style("pearl");
	//UIManager::colors()->appearance("light");


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
	//UIManager::desktop()->mainloop();
}

void List::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test7();
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
