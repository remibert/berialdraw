#include "berialdraw_imp.hpp"
using namespace berialdraw;

/** Test 1: Basic grid with scrolling */
void TableView::test1()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_color(Color::DARK_GRAY);
	table->horizontal_thickness(1);
	table->vertical_thickness(1);
	
	for (uint16_t row = 0; row < 30; row++)
	{
		for (uint16_t column = 0; column < 20; column++)
		{
			Label* label = new Label(table);
			label->text("(%c:%d)", 0x41 + row, column + 1);
			label->cell(row, column);
		}
	}
	
	String script1(
	"["
		"{'type':'touch','x':200,'y':200,'state':'down'},"
		"{'type':'touch','x':190,'y':190,'state':'move'},"
		"{'type':'touch','x':185,'y':185,'state':'up'},"
		"{'type':'key','key':9208,'state':'press'}"
	"]");
	UIManager::notifier()->play_script(script1, "$(ui.tests)/out/tableview1.svg");
	//UIManager::desktop()->mainloop();
}

/** Test 2: Grid with JSON load */
void TableView::test2()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	ScrollView* scrollview = new ScrollView(&window);
	Column* column = new Column(scrollview);
	
	TableView* table = new TableView(column);
	table->horizontal_thickness(1);
	table->vertical_thickness(1);
	table->grid_color(Color::DARK_GRAY);
	
	String json_data(
		"["
		"['ID','Name','Email'],"
		"['001','Alice','alice@example.com'],"
		"['002','Bob','bob@example.com'],"
		"['003','Charlie','charlie@example.com'],"
		"['004','Diana','diana@example.com'],"
		"['005','Eve','eve@example.com'],"
		"]"
	);
	
	json_data.offset(0);
	table->load(json_data);
	
	String script2("[{'type':'key','key':9208,'state':'press'}]");
	UIManager::notifier()->play_script(script2, "$(ui.tests)/out/tableview2.svg");
	// UIManager::desktop()->mainloop();
}

/** Test 3: Grid visibility toggle */
void TableView::test3()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_visible(true);
	table->grid_color(Color::RED);
	table->horizontal_thickness(2);
	table->vertical_thickness(2);
	
	for (uint16_t row = 0; row < 5; row++)
	{
		for (uint16_t col = 0; col < 5; col++)
		{
			Label* label = new Label(table);
			label->text("%d", row * 5 + col + 1);
			label->cell(row, col);
		}
	}
	
	String script3("[{'type':'key','key':9208,'state':'press'}]");
	UIManager::notifier()->play_script(script3, "$(ui.tests)/out/tableview3.svg");
	//UIManager::desktop()->mainloop();
}

/** Test 4: Grid with invisible grid lines */
void TableView::test4()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_visible(false);
	table->alternating_row_color1(Color::ALICE_BLUE);
	table->alternating_row_color2(Color::LIGHT_AQUAMARINE);
	
	for (uint16_t row = 0; row < 8; row++)
	{
		for (uint16_t col = 0; col < 4; col++)
		{
			Label* label = new Label(table);
			label->text("Cell %d", row * 4 + col + 1);
			label->cell(row, col);
		}
	}
	
	String script4("[{'type':'key','key':9208,'state':'press'}]");
	UIManager::notifier()->play_script(script4, "$(ui.tests)/out/tableview4.svg");
	//UIManager::desktop()->mainloop();
}

/** Test 5: Different grid colors and thickness */
void TableView::test5()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_color(Color::BLUE);
	table->horizontal_thickness(3);
	table->vertical_thickness(1);
	table->alternating_row_color1(Color::WHITE);
	table->alternating_row_color2(Color::DARK_GRAY);
	
	for (uint16_t row = 0; row < 6; row++)
	{
		for (uint16_t col = 0; col < 6; col++)
		{
			Label* label = new Label(table);
			label->text("R%dC%d", row, col);
			label->cell(row, col);
		}
	}
	
	String script5("[{'type':'key','key':9208,'state':'press'}]");
	UIManager::notifier()->play_script(script5, "$(ui.tests)/out/tableview5.svg");
	//UIManager::desktop()->mainloop();
}

/** Test 6: Progressive grid filling 1x1 to 10x10 with snapshots */
void TableView::test6()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_color(Color::DARK_GRAY);
	table->horizontal_thickness(1);
	table->vertical_thickness(1);
	table->alternating_row_color1(Color::WHITE);
	table->alternating_row_color2(Color::WHITE_SMOKE);
	
	int id = 0;
	for (uint16_t size = 1; size <= 10; size++)
	{
		table->clear();
		
		for (uint16_t row = 0; row < size; row++)
		{
			for (uint16_t col = 0; col < size; col++)
			{
				Label* label = new Label(table);
				label->text("%d", row * size + col + 1);
				label->cell(row, col);
			}
		}

		String name;
		name.print("$(ui.tests)/out/tableview6_%d.svg", ++id);
		UIManager::desktop()->dispatch(name);
	}
	//UIManager::desktop()->mainloop();
}

/** Test 7: Scrolling behavior test */
void TableView::test7()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_color(Color::DARK_GRAY);
	table->horizontal_thickness(1);
	table->vertical_thickness(1);
	
	for (uint16_t row = 0; row < 20; row++)
	{
		for (uint16_t col = 0; col < 15; col++)
		{
			Label* label = new Label(table);
			label->text("R%dC%d", row, col);
			label->cell(row, col);
		}
	}
	
	table->scroll_position(0, 0);
	
	String script7(
	"["
		"{'type':'key','key':9208,'state':'press'}," // initial
		"{'type':'touch','x':200,'y':200,'state':'down'},"
		"{'type':'touch','x':250,'y':250,'state':'move'},"
		"{'type':'touch','x':250,'y':250,'state':'up'},"
		"{'type':'key','key':9208,'state':'press'}," // after vertical scroll
		"{'type':'touch','x':200,'y':200,'state':'down'},"
		"{'type':'touch','x':150,'y':200,'state':'move'},"
		"{'type':'touch','x':150,'y':200,'state':'up'},"
		"{'type':'key','key':9208,'state':'press'}" // after horizontal scroll
	"]");
	UIManager::notifier()->play_script(script7, "$(ui.tests)/out/tableview7.svg");
	//UIManager::desktop()->mainloop();
}

/** Test 8: Custom colors and row styling */
void TableView::test8()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_color(Color::GREEN);
	table->horizontal_thickness(2);
	table->vertical_thickness(2);
	table->alternating_row_color1(Color::LIGHT_AQUAMARINE);
	table->alternating_row_color2(Color::ALICE_BLUE);
	
	for (uint16_t row = 0; row < 10; row++)
	{
		for (uint16_t col = 0; col < 8; col++)
		{
			Label* label = new Label(table);
			label->text("%d", (row + 1) * (col + 1));
			label->cell(row, col);
		}
	}
	
	String script8("[{'type':'key','key':9208,'state':'press'}]");
	UIManager::notifier()->play_script(script8, "$(ui.tests)/out/tableview8.svg");
	//UIManager::desktop()->mainloop();
}

/** Test 9: Large table with scrolling snapshot */
void TableView::test9()
{
	MemoryLeakLog
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_color(Color::DARK_GRAY);
	table->horizontal_thickness(1);
	table->vertical_thickness(1);
	
	for (uint16_t row = 0; row < 50; row++)
	{
		for (uint16_t col = 0; col < 25; col++)
		{
			Label* label = new Label(table);
			label->text("Val");
			label->cell(row, col);
		}
	}
	
	String script9("[{'type':'key','key':9208,'state':'press'}]"); // Large table top-left
	UIManager::notifier()->play_script(script9, "$(ui.tests)/out/tableview9.svg");
	//UIManager::desktop()->mainloop();
}

/** Main test runner - execute all tests */
void TableView::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test9();
		test8();
		test7();
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
