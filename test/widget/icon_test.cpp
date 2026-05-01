#include "berialdraw_imp.hpp"
using namespace berialdraw;


void Icon::test1()
{
	//UIManager::notifier()->log();
	Window window;
		window.size(480,480);

	Directory directory;
		directory.open("$(ui.icons)");

	if (directory.first())
	{
		ScrollView * scroll = new ScrollView(&window);
			scroll->align(Align::ALIGN_TOP);
			scroll->scroll_direction(ScrollDirection::SCROLL_VERTICAL);
			scroll->margin(10);

		Row * main = new Row(scroll);
			main->flow(true);

		int angle = 0;
		do
		{
			if (directory.match("*.icn"))
			{
				Icon * icon = new Icon(main);
					size_t s = sizeof(*icon);
					icon->filename(directory.full_path());
					icon->zoom(4);
					icon->radius(16);
					icon->icon_padding(10);
					icon->margin(5);
					icon->color(Hsl::to_rgb(angle, 50, 90));
					icon->icon_color(Hsl::to_rgb(angle, 100, 25));
					icon->text(directory.filename());
				angle += 30;
			}
		}
		while(directory.next());
	}

	String script(
	"["
		"{'type':'touch','x':293,'y':316,'state':'down'},"
		"{'type':'touch','x':293,'y':316,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':166,'y':183,'state':'down'},"
		"{'type':'touch','x':166,'y':183,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':410,'y':436,'state':'down'},"
		"{'type':'touch','x':406,'y':409,'state':'move'},"
		"{'type':'touch','x':386,'y':256,'state':'move'},"
		"{'type':'touch','x':384,'y':140,'state':'move'},"
		"{'type':'touch','x':393,'y': 61,'state':'move'},"
		"{'type':'touch','x':379,'y': 38,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':302,'y':458,'state':'down'},"
		"{'type':'touch','x':303,'y':339,'state':'move'},"
		"{'type':'touch','x':311,'y':275,'state':'move'},"
		"{'type':'touch','x':311,'y':261,'state':'move'},"
		"{'type':'touch','x':309,'y':249,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
	"]");

	//UIManager::desktop()->mainloop();
	UIManager::notifier()->play_script(script, "$(ui.tests)/out/icon1_%d.svg");
}

void Icon::test2()
{
	//UIManager::notifier()->log();
	Window window;
		window.size(480,480);

	ScrollView * scroll = new ScrollView(&window);
		scroll->align(Align::ALIGN_TOP);
		scroll->scroll_direction(ScrollDirection::SCROLL_VERTICAL);
		scroll->margin(10);

	Row * main = new Row(scroll);
		main->flow(true);

	int angle = 0;
	for (Directory directory("$(ui.icons)"); directory.exist(); directory.next())
	{
		if (directory.match("*.icn"))
		{
			Icon * icon = new Icon(main);
				size_t s = sizeof(*icon);
				icon->filename(directory.full_path());
				icon->zoom(5);
				icon->radius(0);
				icon->icon_padding(0);
				icon->margin(2);
				icon->padding(0);
				icon->color(Hsl::to_rgb(angle, 50, 90));
				icon->icon_color(Hsl::to_rgb(angle, 100, 25));
				icon->text("");
			angle += 30;
		}
	}
	UIManager::desktop()->dispatch("$(ui.tests)/out/icon2_0.svg");
}

void Icon::test3()
{
	Window window;
		window.size(480,480);

	ScrollView * scroll = new ScrollView(&window);
		scroll->align(Align::ALIGN_TOP);
		scroll->scroll_direction(ScrollDirection::SCROLL_VERTICAL);
		scroll->margin(10);

	Row * main = new Row(scroll);
		main->flow(true);

	int angle = 0;
	for (Directory directory("$(ui.icons)"); directory.exist(); directory.next())
	{
		if (directory.match("*.icn"))
		{
			Icon * icon = new Icon(main);
				icon->filename(directory.full_path());
				icon->zoom(5);
				icon->radius(100);
				icon->icon_padding(10);
				icon->margin(2);
				icon->padding(0);
				icon->color(Hsl::to_rgb(angle, 50, 90));
				icon->icon_color(Hsl::to_rgb(angle, 100, 25));
				icon->text("");
			angle += 30;
		}
	}

	//UIManager::desktop()->mainloop();
	UIManager::desktop()->dispatch("$(ui.tests)/out/icon3_0.svg");
}

void Icon::test4()
{
	Window window;
		Icon * icon = new Icon(&window);
			icon->filename("$(ui.icons)/maison.icn");
			icon->position(10,10);
			icon->text("Icon");

		Button * button = new Button(&window);
			button->position(200,200);
			button->text("Button");

	UIManager::desktop()->dispatch("$(ui.tests)/out/icon4_0.svg");
		icon->zoom(4);
		button->font_size(30,60);

	UIManager::desktop()->dispatch("$(ui.tests)/out/icon4_1.svg");
		button->size(150,200);
		icon->size(150,200);

	UIManager::desktop()->dispatch("$(ui.tests)/out/icon4_2.svg");
}

void Icon::test5()
{
	Window window;
		window.color(Color::WHITE);

	Icon * icon1 = new Icon(&window);
		icon1->position(100, 20);
		icon1->size(100, 50);
		icon1->text("Absolute position with text");
		icon1->text_color(Color::RED);
		icon1->font_size(16);
		icon1->color(Color::LIGHT_BLUE);
		icon1->border_color(Color::BLUE);
		icon1->thickness(4);
		icon1->filename("$(ui.icons)/maison.icn");

	Icon * icon2 = new Icon(&window);
		icon2->color(Color::LIGHT_GREEN);
		icon2->border_color(Color::GREEN);
		icon2->thickness(2);
		icon2->filename("$(ui.icons)/maison.icn");

	Icon * icon3 = new Icon(&window);
		icon3->position(20, 130);
		icon3->size(350, 50);
		icon3->text("Another absolute icon");
		icon3->text_color(Color::GREEN);
		icon3->font_size(14);
		icon3->color(Color::LIGHT_RED);
		icon3->border_color(Color::RED);
		icon3->thickness(3);
		icon3->filename("$(ui.icons)/maison.icn");

	Icon * icon4 = new Icon(&window);
		icon4->position(200, 190);
		icon4->zoom(4);
		icon4->color(Color::LIGHT_ORANGE);
		icon4->border_color(Color::ORANGE);
		icon4->thickness(0);
		icon4->filename("$(ui.icons)/maison.icn");

	Icon * icon5 = new Icon(&window);
		icon5->position(20, 190);
		icon5->min_size(100,100);
		icon5->color(Color::LIGHT_BROWN);
		icon5->border_color(Color::BROWN);
		icon5->thickness(0);
		icon5->filename("$(ui.icons)/maison.icn");

	//UIManager::desktop()->mainloop();
	UIManager::desktop()->dispatch("$(ui.tests)/out/icon5.svg");
}

void Icon::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
