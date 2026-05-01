#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Sketch::test1()
{
	Window window;
		window.color(Color::BLUE,64);
		window.position(7,5);

	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::RED,64);
		canvas->position(13,29);

	Rect * rect= new Rect(canvas);
		rect->position(57,31);
		rect->size(10,360);
		rect->color(Color::WHITE,64);

	Sketch * maison = new Sketch(canvas);
		maison->position(rect->position());
		maison->size(rect->size());
		maison->color(Color::YELLOW);
		maison->filename("$(ui.icons)/maison.icn");

	Sketch * computer = new Sketch(canvas);
		computer->position(rect->position());
		computer->size(rect->size());
		computer->color(Color::YELLOW);
		computer->filename("$(ui.icons)/computer.icn");

	UIManager::desktop()->dispatch("$(ui.tests)/out/sketch1_1.svg");
	rect->size(360,10);
	computer->size(rect->size());
	maison->size(rect->size());
	UIManager::desktop()->dispatch("$(ui.tests)/out/sketch1_2.svg");
	rect->size(360,360);
	computer->size(rect->size());
	maison->size(rect->size());
	UIManager::desktop()->dispatch("$(ui.tests)/out/sketch1_3.svg");
	rect->size(60,360);
	computer->size(rect->size());
	maison->size(rect->size());
	UIManager::desktop()->dispatch("$(ui.tests)/out/sketch1_4.svg");
	rect->size(360,60);
	computer->size(rect->size());
	maison->size(rect->size());
	UIManager::desktop()->dispatch("$(ui.tests)/out/sketch1_5.svg");
}

void Sketch::test2()
{
	Window window;

	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::GREEN,64);
		canvas->size(480,480);

	Rect * rect= new Rect(canvas);
		rect->position(240,240);
		rect->size(100,100);
		rect->color(Color::RED,64);
		rect->repeat(Shape::REPEAT_ANGLE, 0, 359, 30);

	Sketch * maison = new Sketch(canvas);
		maison->filename("$(ui.icons)/maison.icn");
		maison->size(rect->size());
		maison->color(Color::WHITE,128);
		maison->position(rect->position());
		maison->repeat(Shape::REPEAT_ANGLE, 0, 359, 30);

	Sketch * computer = new Sketch(canvas);
		computer->position(rect->position());
		computer->size(rect->size());
		computer->color(Color::YELLOW);
		computer->filename("$(ui.icons)/computer.icn");
		computer->repeat(Shape::REPEAT_ANGLE, 0, 359, 30);

	Coord pos = 0;
	maison->center(pos,50);
	rect->center(pos,50);
	computer->center(pos,50);
	computer->color(Color::YELLOW,32);
	UIManager::desktop()->dispatch("$(ui.tests)/out/sketch2_1.svg");

	pos -= 100;
	maison->color(Color::BLUE,64);
	maison->center(pos,50);
	rect->center(pos,50);
	computer->center(pos,50);
	computer->color(Color::BLUE,64);
	UIManager::desktop()->dispatch("$(ui.tests)/out/sketch2_2.svg");

	pos -= 50;
	maison->center(pos,100);
	maison->color(Color::RED);
	rect->center(pos,100);
	computer->center(pos,100);
	computer->color(Color::RED);
	UIManager::desktop()->dispatch("$(ui.tests)/out/sketch2_3.svg");
}

void Sketch::test3()
{
	Window window;

	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::RED,64);
		canvas->extend(Extend::EXTEND_ALL);

	Rect * rect= new Rect(canvas);
		rect->position(57,31);
		rect->size(32,360);
		rect->color(Color::WHITE,64);

	Sketch * maison = new Sketch(canvas);
		maison->position(rect->position());
		maison->size(rect->size());
		maison->filename("$(ui.icons)/maison.icn");
	UIManager::desktop()->dispatch("$(ui.tests)/out/sketch3_0.svg");

	maison->align(Align::ALIGN_BOTTOM);
	UIManager::desktop()->dispatch("$(ui.tests)/out/sketch3_1.svg");

	maison->align(Align::ALIGN_TOP);
	UIManager::desktop()->dispatch("$(ui.tests)/out/sketch3_2.svg");

	maison->align(Align::ALIGN_VERTICAL);
	UIManager::desktop()->dispatch("$(ui.tests)/out/sketch3_3.svg");

	rect->size(360,16);
	maison->size(rect->size());
	maison->align(Align::ALIGN_LEFT);
	UIManager::desktop()->dispatch("$(ui.tests)/out/sketch3_6.svg");

	maison->align(Align::ALIGN_RIGHT);
	UIManager::desktop()->dispatch("$(ui.tests)/out/sketch3_4.svg");

	maison->align(Align::ALIGN_HORIZONTAL);
	UIManager::desktop()->dispatch("$(ui.tests)/out/sketch3_5.svg");
}

void Sketch::test4()
{
}

void Sketch::test5()
{
}

void Sketch::test6()
{
}

void Sketch::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
