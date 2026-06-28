#include "berialdraw_imp.hpp"
using namespace berialdraw;

/** Test 1: Basic list creation with items */
void List::test1()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	//List* list = new List(&window);
	//list->size(200, 300);
	//list->position(50, 50);
	//
	//list->add("First item");
	//list->add("Second item");
	//list->add("Third item");
	//list->add("Fourth item");
	//list->add("Fifth item");
	//
	//String script("[{'type':'key','key':9208,'state':'press'}]");
	//UIManager::notifier()->play_script(script, "$(ui.tests)/out/list1.svg");
}

/** Test 2: List with selection */
void List::test2()
{
	MemoryLeakLog
	//Window window;
	//window.color(Color::WHITE_BLUE);
	//
	//List* list = new List(&window);
	//list->size(200, 300);
	//list->position(50, 50);
	//
	//list->add("Item A");
	//list->add("Item B");
	//list->add("Item C");
	//list->add("Item D");
	//list->add("Item E");
	//
	////list->select(2);  // Select "Item C"
	//
	//String script("[{'type':'key','key':9208,'state':'press'}]");
	//UIManager::notifier()->play_script(script, "$(ui.tests)/out/list2.svg");
}

/** Test 3: List with enabled/disabled items */
void List::test3()
{
	MemoryLeakLog
	//Window window;
	//window.color(Color::WHITE_BLUE);
	//
	//List* list = new List(&window);
	//list->size(200, 300);
	//list->position(50, 50);
	//
	//list->add("Enabled item 1");
	////list->add("Disabled item", nullptr, false);  // Disabled
	//list->add("Enabled item 2");
	////list->add("Disabled item 2", nullptr, false);  // Disabled
	//list->add("Enabled item 3");
	//
	//String script("[{'type':'key','key':9208,'state':'press'}]");
	//UIManager::notifier()->play_script(script, "$(ui.tests)/out/list3.svg");
}

/** Test 4: List with icons */
void List::test4()
{
	MemoryLeakLog
	//Window window;
	//window.color(Color::WHITE_BLUE);
	//
	//List* list = new List(&window);
	//list->size(200, 300);
	//list->position(50, 50);
	//
	////list->add("Home", "$(ui.icons)/home.icn");
	////list->add("Settings", "$(ui.icons)/settings.icn");
	////list->add("Search", "$(ui.icons)/search.icn");
	////list->add("Add", "$(ui.icons)/add.icn");
	////list->add("Delete", "$(ui.icons)/delete.icn");
	//
	//String script("[{'type':'key','key':9208,'state':'press'}]");
	//UIManager::notifier()->play_script(script, "$(ui.tests)/out/list4.svg");
}

/** Test 5: List with arrows */
void List::test5()
{
	MemoryLeakLog
	//Window window;
	//window.color(Color::WHITE_BLUE);
	//
	//List* list = new List(&window);
	//list->size(200, 300);
	//list->position(50, 50);
	////list->show_arrow(true);
	//
	//list->add("Menu item 1");
	//list->add("Menu item 2");
	//list->add("Menu item 3");
	//list->add("Menu item 4");
	//
	//String script("[{'type':'key','key':9208,'state':'press'}]");
	//UIManager::notifier()->play_script(script, "$(ui.tests)/out/list5.svg");
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

		//UIManager::styles()->style("pearl");
		//UIManager::colors()->appearance("light");
/*	Window window;
	window.color(Color::WHITE_BLUE);
	
	List* list = new List(&window);
		list->size(200, 300);
		list->position(50, 50);
	
	for (int i = 0; i < 15; i++)
	{
		String label;
		label.print("Click me %d",i);
		list->add(label);
	}
	
	// Touch on third item
	String script(
	"["
		"{'type':'touch','x':150,'y':170,'state':'down'},"
		"{'type':'touch','x':150,'y':170,'state':'up'},"
		"{'type':'key','key':9208,'state':'press'}"
	"]");
//UIManager::desktop()->mainloop();
	UIManager::notifier()->play_script(script, "$(ui.tests)/out/list7.svg");*/
/*
	Window window;
	ScrollView * scrollview = new ScrollView(&window);
	Column * column = new Column(scrollview);
	for (int i = 0; i < 1; i+= 2)
	{
		Row * row = new Row(column);
		Label * label;
		label = new Label(row);
			label->text("< ");
			label->font_size(40);

		Picture * maison = new Picture(row);
			//maison->position(rect->position());
			//maison->size(rect->size());
			//maison->color(Color::TRANSPARENT);
			maison->size(100,40);
			maison->icon_color(Color::RED);
//maison->zoom(2);
			maison->filename("$(ui.icons)/settings.icn");

		label = new Label(row);
			label->text("%0*d",i,i);
			label->font_size(40);
			label->size_policy(SizePolicy::ENLARGE_WIDTH);
			label->text_align(Align::ALIGN_LEFT);

		label = new Label(row);
			label->text(" >");
			label->font_size(40);
	}
*/
	UIManager::styles()->style("pearl");
	UIManager::colors()->appearance("light");


	Window window;
#if 0
	ScrollView * scrollview = new ScrollView(&window);
	scrollview->align(Align::ALIGN_TOP);
	Column * column = new Column(scrollview);
	for (int i = 0; i < 20; i+= 1)
	{
		Button * button = new Button(column);
			//button->border_color(Color::LIGHT_BLACK,64);   // Sets border color to light black
			button->borders(Borders::BOTTOM_BORDER);       // Draws border only on the bottom side
			button->margin(0,0,1,0);                       // Adds margin around the button
			button->thickness(1);                          // Sets border thickness to 1
			button->radius(0);                             // Sets corner radius to 0 (square corners)
			//button->font_size(30);

			Row * row = new Row(button);
				Picture * icon = new Picture(row);
					icon->size(button->font_size());
					icon->filename("$(ui.icons)/settings.icn"); // <- leading icon
					icon->align(Align::ALIGN_LEFT);
					icon->inherited_focus_color(true);

				Label * label = new Label(row);
					label->text("%0*d",i,i); // <- middle text
					label->align(Align::ALIGN_LEFT);
					label->size_policy(SizePolicy::ENLARGE_WIDTH);
					label->font_size(button->font_size());
					label->inherited_focus_color(true);
			
				label = new Label(row);
					label->text(">"); // Trailing text
					label->font_size(button->font_size());
					label->inherited_focus_color(true);
	}
#else
#if 0
	List * list = new List(&window);
	list->size(200, 300);
	list->position(50, 50);
	//list->separator_thickness(2);

	for (int i = 1; i < 50; i+= 1)
	{
		String text;
		text.print("%0*d",i,i);
		list->add(text, "$(ui.icons)/settings.icn", ">");
		list->add(text, "  ", ">");
		// list->add(text, "  ","");
	}
#else
#endif
#endif
	Pane* pane = new Pane(&window);
		pane->size(200, 300);
		pane->position(50, 50);
		pane->color(Color::ROSE);
		pane->radius(0);
		pane->thickness(0);
		//pane->border_color(Color::GRAY);

#if 0
	List* list = new List(&window);

	//list->color(Color::PLATINUM);
	//list->thickness(30);
	//list->border_color(Color::GRAY);
	list->radius(40);
	list->padding(0);
	//list->margin(60);
	list->size(200, 300);
	list->position(50, 50);
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
#else
		Column* column = new Column(&window);
		column->size(200, 300);
		column->position(50, 50);

		Label* label;
		
		label = new Label(column);
		label->color(Color::RED);
		label->text("hello");
		label->margin(30);
		label = new Label(column);
		label->color(Color::RED);
		label->text("hello");
		label = new Label(column);
		label->color(Color::RED);
		label->text("hello");
		label = new Label(column);
		label->color(Color::RED);
		label->text("hello");

#endif
	UIManager::desktop()->dispatch("$(ui.tests)/out/list7_1.svg");
	UIManager::desktop()->mainloop();
}

/** Test orchestrator */
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
