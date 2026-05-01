#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Line::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		Window window;
			window.position(0,0);
			window.size(480,480);
			window.color(Color::WHITE);

		Canvas * canvas = new Canvas(&window);
			canvas->position(0,0);
			canvas->size(window.size());
			canvas->color(Color::YELLOW,32);

		Compass compass;
			compass.center(240,240);
			compass.radius(220);

		Line * line;

		for (int32_t angle = 0; angle < 180; angle += 6)
		{
			line = new Line(canvas);
				line->color(Color::RED,32);
				line->thickness(20);

			compass.angle(angle);
			line->point1(compass.position());
			compass.angle(angle+180);
			line->point2(compass.position());
		}

		UIManager::desktop()->dispatch("$(ui.tests)/out/line1.svg");
	}
}
