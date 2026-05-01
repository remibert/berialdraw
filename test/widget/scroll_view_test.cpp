#include "berialdraw_imp.hpp"
using namespace berialdraw;


void ScrollView::test1()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(140,140);
		window.size(160,180);
		int row = 0;
		int column = 0;
		Label * label;
		Edit  * edit;

		ScrollView * scroll_view = new ScrollView(&window);
		Grid * grid = new Grid(scroll_view);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("First Name");
			label->margin_top(10);

		edit = new Edit(grid);
			edit->text("");
			edit->cell(row++,column);
			edit->max_columns(20);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Last name");
			label->margin_top(10);

		edit = new Edit(grid);
			edit->text("");
			edit->cell(row++,column);
			edit->max_columns(20);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Address");
			label->margin_top(10);

		edit = new Edit(grid);
			edit->text("");
			edit->cell(row++,column);
			edit->max_columns(20);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Email");
			label->margin_top(10);

		edit = new Edit(grid);
			edit->text("");
			edit->cell(row++,column);
			edit->max_columns(20);

		Button * connect = new Button(grid);
			connect->margin_top(10);
			connect->text("OK");
			connect->cell(row++,column);
			connect->margin(2,30,10,30);

	UIManager::desktop()->dispatch();

	Size size = scroll_view->scroll_size();
	size.decrease(scroll_view->viewport_size());
	Point position = scroll_view->scroll_position();
	int id = 0;
	int i = 0;
	for (Dim y = 0; y <= size.height(); y+=10)
	{
		for (Dim x = 0; x <= size.width(); x+=7)
		{
			position.set(x,y);
			scroll_view->scroll_position(position);

			if (i % 11 == 0)
			{
				String name;
				name.print("$(ui.tests)/out/scroll1_%d.svg", ++id);
				UIManager::desktop()->dispatch(name);
			}
			else
			{
				UIManager::desktop()->dispatch();
			}
			i++;
		}
	}
}

void ScrollView::test_create_window(Window & window)
{
	int row = 0;
	int column = 0;
	Label * label;
	Edit  * edit;

	ScrollView * scroll_view = new ScrollView(&window);

	scroll_view->scroll_direction(SCROLL_VERTICAL);
	scroll_view->id(1);

	Grid * grid = new Grid(scroll_view);

	label = new Label(grid);
		label->cell(row++,column);
		label->text("First Name");
		label->margin_top(20);

	edit = new Edit(grid);
		edit->text("");
		edit->cell(row++,column);

	label = new Label(grid);
		label->cell(row++,column);
		label->text("Last name");
		label->margin_top(20);

	edit = new Edit(grid);
		edit->text("");
		edit->cell(row++,column);

	label = new Label(grid);
		label->cell(row++,column);
		label->text("Address");
		label->margin_top(20);

	edit = new Edit(grid);
		edit->text("");
		edit->cell(row++,column);

	label = new Label(grid);
		label->cell(row++,column);
		label->text("Email");
		label->margin_top(20);

	edit = new Edit(grid);
		edit->text("");
		edit->cell(row++,column);

	Button * button = new Button(grid);
		button->text("OK");
		button->cell(row++,column);
		button->margin(20,30,10,30);
}

void ScrollView::test2()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(140,140);
		window.size(160,180);
		window.color(Color::ALICE_BLUE);

	test_create_window(window);

	String script(
	"["
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':299,'y': 26,'state':'move'},"
		"{'type':'touch','x':263,'y': 61,'state':'move'},"
		"{'type':'touch','x':258,'y':149,'state':'move'},"
		"{'type':'touch','x':231,'y':221,'state':'move'},"
		"{'type':'touch','x':235,'y':259,'state':'move'},"
		"{'type':'touch','x':238,'y':263,'state':'down'},"
		"{'type':'touch','x':237,'y':254,'state':'move'},"
		"{'type':'touch','x':231,'y':187,'state':'move'},"
		"{'type':'touch','x':230,'y':178,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':229,'y':178,'state':'down'},"
		"{'type':'touch','x':229,'y':215,'state':'move'},"
		"{'type':'touch','x':228,'y':274,'state':'move'},"
		"{'type':'touch','x':225,'y':294,'state':'move'},"
		"{'type':'touch','x':224,'y':305,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':224,'y':305,'state':'down'},"
		"{'type':'touch','x':224,'y':304,'state':'move'},"
		"{'type':'touch','x':221,'y':206,'state':'move'},"
		"{'type':'touch','x':221,'y':144,'state':'move'},"
		"{'type':'touch','x':220,'y':102,'state':'move'},"
		"{'type':'touch','x':220,'y': 87,'state':'move'},"
		"{'type':'touch','x':219,'y': 83,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':220,'y': 98,'state':'move'},"
		"{'type':'touch','x':225,'y':203,'state':'move'},"
		"{'type':'touch','x':227,'y':289,'state':'move'},"
		"{'type':'touch','x':226,'y':294,'state':'down'},"
		"{'type':'touch','x':227,'y':299,'state':'move'},"
		"{'type':'touch','x':237,'y':323,'state':'move'},"
		"{'type':'touch','x':241,'y':330,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':225,'y':280,'state':'move'},"
		"{'type':'touch','x':213,'y':218,'state':'move'},"
		"{'type':'touch','x':208,'y':204,'state':'down'},"
		"{'type':'touch','x':208,'y':204,'state':'up'},"
		"{'type':'key','key':114,'state':'down','modifier':''     ,'character':'r'},"
		"{'type':'key','key':101,'state':'down','modifier':''     ,'character':'e'},"
		"{'type':'key','key':114,'state':'up'  ,'modifier':''     ,'character':'r'},"
		"{'type':'key','key':101,'state':'up'  ,'modifier':''     ,'character':'e'},"
		"{'type':'key','key':109,'state':'down','modifier':''     ,'character':'m'},"
		"{'type':'key','key':109,'state':'up'  ,'modifier':''     ,'character':'m'},"
		"{'type':'key','key':105,'state':'down','modifier':''     ,'character':'i'},"
		"{'type':'key','key':105,'state':'up'  ,'modifier':''     ,'character':'i'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':208,'y':204,'state':'down'},"
		"{'type':'touch','x':208,'y':182,'state':'move'},"
		"{'type':'touch','x':208,'y':123,'state':'move'},"
		"{'type':'touch','x':207,'y':120,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':217,'y':139,'state':'move'},"
		"{'type':'touch','x':239,'y':232,'state':'move'},"
		"{'type':'touch','x':238,'y':232,'state':'down'},"
		"{'type':'touch','x':212,'y':233,'state':'move'},"
		"{'type':'touch','x':170,'y':234,'state':'move'},"
		"{'type':'touch','x':172,'y':235,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':172,'y':223,'state':'move'},"
		"{'type':'touch','x':155,'y':164,'state':'move'},"
		"{'type':'touch','x':155,'y':160,'state':'down'},"
		"{'type':'touch','x':165,'y':161,'state':'move'},"
		"{'type':'touch','x':175,'y':163,'state':'move'},"
		"{'type':'touch','x':176,'y':163,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
	"]");
	UIManager::notifier()->play_script(script, "$(ui.tests)/out/scroll2_%d.svg");

}

void ScrollView::test3()
{
	Window window;
		window.position(140,140);
		window.size(160,180);
		window.color(Color::ALICE_BLUE);
	int id = 0;

	ScrollView * scroll_view = new ScrollView(&window);

	Grid * grid = new Grid(scroll_view);

	scroll_view->scroll_size(30,40);

	Label * label = new Label(grid);
		label->text("0");
		label->cell(0,0);

	label = new Label(grid);
		label->text("1");
		label->cell(1,0);

	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll3_1.svg");
	scroll_view->align(Align::ALIGN_BOTTOM);
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll3_2.svg");
	scroll_view->align(Align::ALIGN_TOP);
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll3_3.svg");
	scroll_view->align(Align::ALIGN_LEFT);
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll3_4.svg");
	scroll_view->align(Align::ALIGN_RIGHT);
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll3_5.svg");
}

void ScrollView::test4()
{
	Window window;
		window.position(140,140);
		window.size(160,180);
		window.color(Color::ALICE_BLUE);
	int id = 0;

	ScrollView * scroll_view = new ScrollView(&window);

	Grid * grid = new Grid(scroll_view);

	for (uint16_t row = 0; row < 10; row ++)
	{
		for (uint16_t column = 0; column < 10; column++)
		{
			Label * label = new Label(grid);
				label->text("%d%c",row,0x41 + column);
				label->cell(row,column);
		}
	}

	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll4_1.svg");
	scroll_view->scroll_direction(SCROLL_VERTICAL);
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll4_2.svg");
	scroll_view->scroll_direction(SCROLL_HORIZONTAL);
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll4_3.svg");
	scroll_view->scroll_direction(SCROLL_ALL_DIRECTIONS);
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll4_4.svg");
}

void ScrollView::test5()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(140,140);
		window.size(160,180);
		window.color(Color::ALICE_BLUE);

	ScrollView * scroll_view = new ScrollView(&window);
		scroll_view->scroll_direction(SCROLL_VERTICAL);
		scroll_view->id(123);
	Column * grid = new Column(scroll_view);
	Button * button;
	int id = 1000;
	for (int i = 0; i < 11; i++)
	{
		if (i == 5)
		{
			ScrollView * imbricated_scrollview = new ScrollView(grid);
				imbricated_scrollview->scroll_direction(SCROLL_VERTICAL);
				imbricated_scrollview->id(456);
				imbricated_scrollview->viewport_size(Size::MAX_SIZE, 60);
				imbricated_scrollview->margin(5);

			Column * imbricated_grid = new Column(imbricated_scrollview);
			imbricated_grid->id(445);
			for (int  j = 0; j < 5; j++)
			{
				button = new Button(imbricated_grid);
					button->text("%c",0x41 + j);
					button->id(id++);
			}
		}
		else
		{
			button = new Button(grid);
				button->margin(20,4);
				button->text("%d",i);
				button->color(Color::BLUE_WHALE);
				button->border_color(Color::TYRIAN_PURPLE);
				button->id(id++);
		}
	}

	//while(true) UIManager::desktop()->dispatch();
	String script(
	"["

		"{'type':'touch','x':249,'y':288,'state':'down'},"
		"{'type':'touch','x':249,'y':277,'state':'move'},"
		"{'type':'touch','x':250,'y':192,'state':'move'},"
		"{'type':'touch','x':250,'y':190,'state':'up'},"

		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':249,'y':288,'state':'down'},"
		"{'type':'touch','x':249,'y':277,'state':'move'},"
		"{'type':'touch','x':250,'y':192,'state':'move'},"
		"{'type':'touch','x':250,'y':190,'state':'up'},"

		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':249,'y':288,'state':'down'},"
		"{'type':'touch','x':249,'y':277,'state':'move'},"
		"{'type':'touch','x':250,'y':192,'state':'move'},"
		"{'type':'touch','x':250,'y':190,'state':'up'},"

		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':249,'y':288,'state':'down'},"
		"{'type':'touch','x':249,'y':277,'state':'move'},"
		"{'type':'touch','x':250,'y':192,'state':'move'},"
		"{'type':'touch','x':250,'y':190,'state':'up'},"

		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':249,'y':288,'state':'down'},"
		"{'type':'touch','x':249,'y':277,'state':'move'},"
		"{'type':'touch','x':250,'y':192,'state':'move'},"
		"{'type':'touch','x':250,'y':190,'state':'up'},"

		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

	"]");
	UIManager::notifier()->play_script(script,"$(ui.tests)/out/scroll5_%d.svg");
}

void ScrollView::test6()
{
	Window window;
	Column *	m_content = new Column(&window);

		Label * label = new Label(m_content);
			label->text("Horizontal scroll view");

		ScrollView * horizontal_scroll_view = new ScrollView(m_content);
			horizontal_scroll_view->scroll_direction(SCROLL_HORIZONTAL);
			horizontal_scroll_view->size_policy(SizePolicy::ENLARGE_ALL);
			Row * horizontal_layout = new Row(horizontal_scroll_view);
				for (int i = 0; i < 26; i++)
				{
					Button *button  = new Button(horizontal_layout);
						button->text("%c",i+0x41);
				}

		label = new Label(m_content);
			label->text("Vertical scroll view");

		ScrollView * vertical_scroll_view = new ScrollView(m_content);
			vertical_scroll_view->scroll_direction(SCROLL_VERTICAL);
			vertical_scroll_view->size_policy(SizePolicy::ENLARGE_ALL);
			Column  * vertical_scroll_layout = new Column(vertical_scroll_view);
				for (int i = 0; i < 26; i++)
				{
					Button *button  = new Button(vertical_scroll_layout);
						button->text("%c",i+0x41);
				}
	
	UIManager::desktop()->dispatch("$(ui.tests)/out/scroll6_0.svg");
}

void ScrollView::test7()
{
}

void ScrollView::test8()
{
}

void ScrollView::test()
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
