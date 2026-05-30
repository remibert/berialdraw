#include "berialdraw_imp.hpp"
using namespace berialdraw;

// Display PNG image with default settings (aspect fit)
void Picture::test1()
{
	Window window;
		window.size(480, 480);
		window.color(Color::WHITE);

	Picture * picture = new Picture(&window);
		picture->filename("$(ui.images)/house.png");

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture1_0.svg");
}

// Display JPEG image with default settings
void Picture::test2()
{
	Window window;
		window.size(480, 480);
		window.color(Color::WHITE);

	Picture * picture = new Picture(&window);
		picture->filename("$(ui.images)/filleperle.jpg");

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture2_0.svg");
}

// Test all fit modes with PNG
void Picture::test3()
{
	Window window;
		window.size(480, 480);
		window.color(Color::LIGHT_GRAY);

	Grid * grid = new Grid(&window);

	// STRETCH
	Picture * p1 = new Picture(grid);
		p1->cell(0, 0);
		p1->filename("$(ui.images)/house.png");
		p1->fit_mode(STRETCH);
		p1->size(220, 220);
		p1->border_color(Color::RED);
		p1->thickness(1);

	// FIT
	Picture * p2 = new Picture(grid);
		p2->cell(1, 0);
		p2->filename("$(ui.images)/house.png");
		p2->fit_mode(FIT);
		p2->size(220, 220);
		p2->border_color(Color::GREEN);
		p2->thickness(1);

	// FIT
	Picture * p3 = new Picture(grid);
		p3->cell(0, 1);
		p3->filename("$(ui.images)/house.png");
		p3->fit_mode(FIT);
		p3->size(220, 220);
		p3->border_color(Color::BLUE);
		p3->thickness(1);

	// STRETCH
	Picture * p4 = new Picture(grid);
		p4->cell(1, 1);
		p4->filename("$(ui.images)/house.png");
		p4->fit_mode(STRETCH);
		p4->size(220, 220);
		p4->border_color(Color::ORANGE);
		p4->thickness(1);

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture3_0.svg");
}

// Test all fit modes with JPEG
void Picture::test4()
{
	Window window;
		window.size(480, 480);
		window.color(Color::LIGHT_GRAY);

	Grid * grid = new Grid(&window);

	// FIT
	Picture * p1 = new Picture(grid);
		p1->cell(0, 0);
		p1->filename("$(ui.images)/filleperle.jpg");
		p1->fit_mode(FIT);
		p1->size(220, 220);
		p1->border_color(Color::RED);
		p1->thickness(1);

	// FIT
	Picture * p2 = new Picture(grid);
		p2->cell(1, 0);
		p2->filename("$(ui.images)/filleperle.jpg");
		p2->fit_mode(FIT);
		p2->size(220, 220);
		p2->border_color(Color::GREEN);
		p2->thickness(1);

	// STRETCH
	Picture * p3 = new Picture(grid);
		p3->cell(0, 1);
		p3->filename("$(ui.images)/filleperle.jpg");
		p3->fit_mode(STRETCH);
		p3->size(220, 220);
		p3->border_color(Color::BLUE);
		p3->thickness(1);

	// FIT
	Picture * p4 = new Picture(grid);
		p4->cell(1, 1);
		p4->filename("$(ui.images)/filleperle.jpg");
		p4->fit_mode(FIT);
		p4->size(220, 220);
		p4->border_color(Color::ORANGE);
		p4->thickness(1);

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture4_0.svg");
}

// Alpha transparency
void Picture::test5()
{
	Window window;
		window.size(480, 480);
		window.color(Color::LIGHT_BLUE);

	// Fully opaque
	Picture * p1 = new Picture(&window);
		p1->position(10, 10);
		p1->size(200, 200);
		p1->filename("$(ui.images)/house.png");
		p1->alpha(255);
		p1->border_color(Color::BLACK);
		p1->thickness(1);

	// Semi-transparent
	Picture * p2 = new Picture(&window);
		p2->position(120, 120);
		p2->size(200, 200);
		p2->filename("$(ui.images)/filleperle.jpg");
		p2->alpha(128);
		p2->border_color(Color::BLACK);
		p2->thickness(1);

	// Mostly transparent
	Picture * p3 = new Picture(&window);
		p3->position(240, 240);
		p3->size(200, 200);
		p3->filename("$(ui.images)/house.png");
		p3->alpha(64);
		p3->border_color(Color::BLACK);
		p3->thickness(1);

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture5_0.svg");
}

// Resize and position changes
void Picture::test6()
{
	Window window;
		window.size(480, 480);
		window.color(Color::WHITE);

	Picture * picture = new Picture(&window);
		picture->filename("$(ui.images)/filleperle.jpg");
		picture->border_color(Color::DARK_GRAY);
		picture->thickness(2);

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture6_0.svg");

		picture->position(50, 50);
		picture->size(200, 150);

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture6_1.svg");

		picture->size(400, 300);

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture6_2.svg");
}

// Both PNG and JPEG side by side in a Row
void Picture::test7()
{
	Window window;
		window.size(480, 240);
		window.color(Color::WHITE);

	Row * row = new Row(&window);

	Picture * png = new Picture(row);
		png->filename("$(ui.images)/house.png");
		png->size(230, 220);
		png->margin(5);
		png->border_color(Color::GREEN);
		png->thickness(1);

	Picture * jpg = new Picture(row);
		jpg->filename("$(ui.images)/filleperle.jpg");
		jpg->size(230, 220);
		jpg->margin(5);
		jpg->border_color(Color::BLUE);
		jpg->thickness(1);

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture7_0.svg");
}

// Picture with vertical scrolling - narrow window
void Picture::test8()
{
	Window window;
		window.size(250, 400);
		window.color(Color::WHITE);

	ScrollView * scroll = new ScrollView(&window);
		scroll->scroll_direction(SCROLL_VERTICAL);

	Column * layout = new Column(scroll);

	Picture * png_picture = new Picture(layout);
		png_picture->filename("$(ui.images)/house.png");
		png_picture->fit_mode(FIT);
		png_picture->margin(10);
		png_picture->border_color(Color::BLUE);
		png_picture->thickness(2);

	Label * label = new Label(layout);
		label->text("Image 1: House (PNG)");
		label->margin(10);

	Picture * jpg_picture = new Picture(layout);
		jpg_picture->filename("$(ui.images)/filleperle.jpg");
		jpg_picture->fit_mode(FIT);
		jpg_picture->margin(10);
		jpg_picture->border_color(Color::GREEN);
		jpg_picture->thickness(2);

	Label * label2 = new Label(layout);
		label2->text("Image 2: Filleperle (JPG)");
		label2->margin(10);

	String script(
	"["
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':240,'y': 427,'state':'down'},"
		"{'type':'touch','x':230,'y':-100,'state':'move'},"
		"{'type':'touch','x':230,'y':-100,'state':'up'},"

		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
	"]");
	UIManager::notifier()->play_script(script, "$(ui.tests)/out/picture8_%d.svg");
}

// Picture with horizontal scrolling - wide window
void Picture::test9()
{
	Window window;
		window.size(400, 250);
		window.color(Color::WHITE);

	ScrollView * scroll = new ScrollView(&window);
		scroll->scroll_direction(SCROLL_HORIZONTAL);

	Row * layout = new Row(scroll);

	Picture * png_picture = new Picture(layout);
		png_picture->filename("$(ui.images)/house.png");
		png_picture->fit_mode(FIT);
		png_picture->margin(10);
		png_picture->border_color(Color::RED);
		png_picture->thickness(2);

	Label * label = new Label(layout);
		label->text("Image 1:\nHouse");
		label->margin(10);

	Picture * jpg_picture = new Picture(layout);
		jpg_picture->filename("$(ui.images)/filleperle.jpg");
		jpg_picture->fit_mode(FIT);
		jpg_picture->margin(10);
		jpg_picture->border_color(Color::ORANGE);
		jpg_picture->thickness(2);

	Label * label2 = new Label(layout);
		label2->text("Image 2:\nFilleperle");
		label2->margin(10);

	String script(
	"["
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':435,'y':250,'state':'down'},"
		"{'type':'touch','x':100,'y':250,'state':'move'},"
		"{'type':'touch','x':100,'y':250,'state':'up'},"

		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
	"]");
	UIManager::notifier()->play_script(script, "$(ui.tests)/out/picture9_%d.svg");
}

// Picture with bidirectional scrolling - square window
void Picture::test10()
{
	Window window;
		window.size(350, 350);
		window.color(Color::WHITE);

	ScrollView * scroll = new ScrollView(&window);
		scroll->scroll_direction(SCROLL_ALL_DIRECTIONS);

	Row * layout = new Row(scroll);

	Picture * png_picture = new Picture(layout);
		png_picture->filename("$(ui.images)/house.png");
		png_picture->fit_mode(FIT);
		png_picture->margin(10);
		png_picture->border_color(Color::PURPLE);
		png_picture->thickness(2);

	Label * label = new Label(layout);
		label->text("Image 1:\nHouse");
		label->margin(10);

	Picture * jpg_picture = new Picture(layout);
		jpg_picture->filename("$(ui.images)/filleperle.jpg");
		jpg_picture->fit_mode(FIT);
		jpg_picture->margin(10);
		jpg_picture->border_color(Color::CYAN);
		jpg_picture->thickness(2);

	Label * label2 = new Label(layout);
		label2->text("Image 2:\nFilleperle");
		label2->margin(10);

	String script(
	"["
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':414,'y':414,'state':'down'},"
		"{'type':'touch','x': -100,'y':0,'state':'move'},"
		"{'type':'touch','x': -100,'y':0,'state':'up'},"

		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
	"]");
	UIManager::notifier()->play_script(script, "$(ui.tests)/out/picture10_%d.svg");
}

void Picture::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test10();
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
