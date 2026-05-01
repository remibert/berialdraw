#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Text::test1()
{
	Window window;
		window.position(11,19);
		window.size(460,460);
		window.color(Color::BROWN,32);

	Canvas * canvas = new Canvas(&window);
		canvas->position(10,10);
		canvas->size(window.size().width()-20, window.size().height() -20);
		canvas->color(Color::YELLOW,64);

	for (uint32_t i = 0; i < 360; i += 10)
	{
		Text * text = new Text(canvas);
			text->font_size(15);
			text->position(canvas->size().middle());
			text->color(Color::DARK_BLUE);
			text->angle(i);
			text->text("        |%3d°|",i);
	}

	UIManager::desktop()->dispatch("$(ui.tests)/out/text1.svg");
}

void Text::test2()
{
	Window window;
		window.position(10,10);
		window.size(460,460);

	Canvas * canvas = new Canvas(&window);
		canvas->position(10,10);
		canvas->size(window.size().width()-20, window.size().height() -20);
		canvas->color(Color::YELLOW,32);

	Point origin(120, 120);

	//if(0)
	{
		for(uint32_t angle = 0; angle < 360; angle += 30)
		{
			Rect * rect = new Rect(canvas);
			Text * text = new Text(canvas);
				text->text("_   %5d_",angle);
				text->position(origin);
				text->font_size(20);
				text->angle(angle);
	
				rect->color(Color::GREEN,32);
				rect->position(origin);
				rect->size(text->content_size());
				rect->angle(angle);
		}
	}

	//if(0)
	{
		origin.move(200,200);

		for(uint32_t angle = 0; angle < 360; angle += 30)
		{
			Rect * rect = new Rect(canvas);
			Text * text = new Text(canvas);
				text->text("_%3d     _",angle);
				text->position(origin);
				text->font_size(20);
				text->angle(angle);
				text->center(text->content_size());
	
				rect->color(Color::CYAN,32);
				rect->position(origin);
				rect->size(text->content_size());
				rect->angle(angle);
				rect->center(text->content_size());
		}
	}

	//if(0)
	{
		origin.move(-200,0);

		for(uint32_t angle = 0; angle < 360; angle += 30)
		{
			Rect * rect = new Rect(canvas);
			Text * text = new Text(canvas);
				text->text("_     %5d_",angle);
				text->font_size(20);
				text->angle(angle);
				text->position(origin);

			Point center(text->content_size().middle());
				text->center(center);
	
				rect->color(Color::RED, 32);
				rect->position(origin);
				rect->size(text->content_size());
				rect->angle(angle);
				rect->center(center);
		}
	}

	UIManager::desktop()->dispatch("$(ui.tests)/out/text2.svg");
}

void Text::test3()
{
	Window window;
		window.position(11,19);
		window.size(440,440);
		window.color(Color::BROWN,32);

	Canvas * canvas = new Canvas(&window);
		canvas->position(10,10);
		canvas->size(window.size().width()-20, window.size().height() -20);
		canvas->color(Color::AQUA,32);

	Text * text;

	text = new Text(canvas);
		text->text("Remi");
		text->font_size(30);
		text->angle(-30);
		text->color(Color::BLACK, 64);
		text->position(0,0);

	text = new Text(canvas);
		text->text("@");
		text->font_size(440);
		text->color(Color::RED, 32);
		text->position(-10,-10);

	text = new Text(canvas);
		text->text("@");
		text->font_size(440);
		text->position(0,0);
		text->color(Color::GREEN, 32);

	text = new Text(canvas);
		text->text("@");
		text->font_size(440);
		text->position(10,10);
		text->color(Color::BLUE, 32);

	UIManager::desktop()->dispatch("$(ui.tests)/out/text3.svg");
}


void Text::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test1();
		test2();
		test3();
	}
}
