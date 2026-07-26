#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Triangle::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		Window window;
			window.position(0,0);
			window.size(480,480);
			window.color(Color::WHITE);

		Canvas * canvas = new Canvas(&window);
			canvas->position(0,0);
			canvas->size(window.size());
			canvas->color(Color::TRANSPARENT);

		Point center(360, 140);

		{
			Cross * cross = new Cross(canvas);
			cross->color(Color::BLACK);
			cross->thickness(3);
			cross->position(center);
			cross->radius(10);
		}
		{
			Triangle * triangle = new Triangle(canvas);

			triangle->radius(100);
			triangle->position(center);
			triangle->thickness(40);
			triangle->color(Color::RED,64);
		}

		{
			Triangle * filltriangle = new Triangle(canvas);
			filltriangle->position(center);
			filltriangle->radius(100);
			filltriangle->color(Color::GREEN,64);
			filltriangle->thickness(0);
		}

		UIManager::desktop()->dispatch("$(ui.tests)/out/triangle.svg");
	}
}
