#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Circle::test()
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

		Point center(120, 120);

		{
			Cross * cross = new Cross(canvas);
			cross->color(Color::BLACK);
			cross->thickness(3);
			cross->position(center);
			cross->radius(10);
		}
		{
			Circle * circle = new Circle(canvas);

			circle->radius(100);
			circle->position(center);
			circle->thickness(40);
			circle->color(Color::RED,64);
		}

		{
			Circle * fillcircle = new Circle(canvas);
			fillcircle->position(center);
			fillcircle->radius(100);
			fillcircle->color(Color::GREEN,64);
			fillcircle->thickness(0);
		}

		UIManager::desktop()->dispatch("$(ui.tests)/out/circle.svg");
	}
}
