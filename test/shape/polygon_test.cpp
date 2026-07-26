#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Polygon::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		Window window;
			window.position(20,20);
			window.size(440,440);
			window.color(Color::WHITE);

		Canvas * canvas = new Canvas(&window);
			canvas->position(0,0);
			canvas->size(window.size());
			canvas->color(Color::YELLOW,32);
	
		{
			Polygon * square = new Polygon(canvas);
				square->color(Color::RED,128);
				square->add_point(100, 100);
				square->add_point(200, 100);
				square->add_point(200, 200);
				square->add_point(100, 200);
				square->position(-100, -100);
		}

		{
			Point center(240, 120);

			Cross * cross;
				cross = new Cross(canvas);
				cross->radius(5);
				cross->thickness(1);
				cross->color(Color::BLACK);
				cross->position(center);

			Polygon * polygon = new Polygon(canvas);
					polygon->color(Color::GREEN,64);
					polygon->add_point(0, 50);
					polygon->add_cubic( 50, 50);
					polygon->add_cubic(-50, -50);
					polygon->add_point(0, -50);
					polygon->center(0, 0);
					polygon->position(center);

			Polygon * clone;

			for(int i = 15; i < 180; i+=15)
			{
				clone = new Polygon(*polygon);
					polygon->angle(i);
			}

			for(int i = 0; i < 180; i+=30)
			{
				cross = new Cross(canvas);
					cross->radius(5);
					cross->thickness(1);
					cross->color(Color::BLACK);
					cross->position(i + 100, i + 200);
				clone = new Polygon(*polygon);
					clone->position(i + 100, i + 200);
					clone->angle(i);
			}
		}
		UIManager::desktop()->dispatch("$(ui.tests)/out/polygon1.svg");
	}
}
