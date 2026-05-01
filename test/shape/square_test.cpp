#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Square::test()
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

		Point center(360, 360);

		{
			Cross * cross = new Cross(canvas);
			cross->color(Color::BLACK);
			cross->thickness(3);
			cross->position(center);
			cross->radius(10);
		}
		{
			Square * square = new Square(canvas);

			square->radius(100);
			square->position(center);
			square->thickness(40);
			square->color(Color::RED,64);
		}

		{
			Square * fillsquare = new Square(canvas);
			fillsquare->position(center);
			fillsquare->radius(100);
			fillsquare->color(Color::GREEN,64);
			fillsquare->thickness(0);
		}

		UIManager::desktop()->dispatch("$(ui.tests)/out/square.svg");
	}
}
