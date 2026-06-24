#include "berialdraw_imp.hpp"
using namespace berialdraw;

/** Test 1: Basic list creation with items */
void List::test1()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	List* list = new List(&window);
	list->size(200, 300);
	list->position(50, 50);
	
	list->add("First item");
	list->add("Second item");
	list->add("Third item");
	list->add("Fourth item");
	list->add("Fifth item");
	
	String script("[{'type':'key','key':9208,'state':'press'}]");
	UIManager::notifier()->play_script(script, "$(ui.tests)/out/list1.svg");
}

/** Test 2: List with selection */
void List::test2()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	List* list = new List(&window);
	list->size(200, 300);
	list->position(50, 50);
	
	list->add("Item A");
	list->add("Item B");
	list->add("Item C");
	list->add("Item D");
	list->add("Item E");
	
	list->select(2);  // Select "Item C"
	
	String script("[{'type':'key','key':9208,'state':'press'}]");
	UIManager::notifier()->play_script(script, "$(ui.tests)/out/list2.svg");
}

/** Test 3: List with enabled/disabled items */
void List::test3()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	List* list = new List(&window);
	list->size(200, 300);
	list->position(50, 50);
	
	list->add("Enabled item 1");
	list->add("Disabled item", nullptr, false);  // Disabled
	list->add("Enabled item 2");
	list->add("Disabled item 2", nullptr, false);  // Disabled
	list->add("Enabled item 3");
	
	String script("[{'type':'key','key':9208,'state':'press'}]");
	UIManager::notifier()->play_script(script, "$(ui.tests)/out/list3.svg");
}

/** Test 4: List with icons */
void List::test4()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	List* list = new List(&window);
	list->size(200, 300);
	list->position(50, 50);
	
	list->add("Home", "$(ui.icons)/home.icn");
	list->add("Settings", "$(ui.icons)/settings.icn");
	list->add("Search", "$(ui.icons)/search.icn");
	list->add("Add", "$(ui.icons)/add.icn");
	list->add("Delete", "$(ui.icons)/delete.icn");
	
	String script("[{'type':'key','key':9208,'state':'press'}]");
	UIManager::notifier()->play_script(script, "$(ui.tests)/out/list4.svg");
}

/** Test 5: List with arrows */
void List::test5()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	List* list = new List(&window);
	list->size(200, 300);
	list->position(50, 50);
	list->show_arrow(true);
	
	list->add("Menu item 1");
	list->add("Menu item 2");
	list->add("Menu item 3");
	list->add("Menu item 4");
	
	String script("[{'type':'key','key':9208,'state':'press'}]");
	UIManager::notifier()->play_script(script, "$(ui.tests)/out/list5.svg");
}

/** Test 6: List with scrollbar (many items) */
void List::test6()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	List* list = new List(&window);
	list->size(200, 200);
	list->position(50, 50);
	list->scrollbar_visible(true);
	
	for (int i = 0; i < 20; i++)
	{
		String text;
		text.print("Item %d", i + 1);
		list->add(text);
	}
	
	// Scroll down
	String script(
	"["
		"{'type':'touch','x':150,'y':150,'state':'down'},"
		"{'type':'touch','x':150,'y':100,'state':'move'},"
		"{'type':'touch','x':150,'y':50,'state':'up'},"
		"{'type':'key','key':9208,'state':'press'}"
	"]");
	UIManager::notifier()->play_script(script, "$(ui.tests)/out/list6.svg");
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
	Window window;
	List* list = new List(&window);
		//list->size(200, 300);
		//list->position(50, 50);

		list->add({"<","Menu",""});
		list->add({"","Menu",">"});
		list->add({"","Menu",""});

		list->add({"$(ui.icons)/settings.icn","Menu",""});
		list->add({"","$<'font-familly':'Cerial','font-size':{'width':30,'height':60},'text-color':0x7F00FF00>Menu","$(ui.icons)/settings.icn"});
		//list->add({"","$<'text-align':'right','font-familly':'Cerial','font-size':{'width':30,'height':60},'text-color':0x7F00FF00>Menu","$(ui.icons)/settings.icn"});
		list->add({"","$(ui.icons)/settings.icn",""});

	//UIManager::desktop()->mainloop();
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
