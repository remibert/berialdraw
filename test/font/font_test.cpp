#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Font::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		Window window;
			window.color(Color::WHITE);
			window.size(480,480);

		Canvas * canvas = new Canvas(&window);
			canvas->size(440,440);
			canvas->position(20,20);
			canvas->color(Color::MINT_CREAM);

		for (uint32_t angle = 0; angle < 360; angle += 30)
		{
			Text * label = new Text(canvas);
				label->color(Color::CADET_BLUE, 128);

				// Set position of text
				label->position(220,220);

				// Change the rotation center to the left side
				label->center(-100,30);

				label->angle(angle);
				label->text("|rémi %3d|",angle);
				label->font_size(20,60);
		}

		UIManager::desktop()->dispatch("$(ui.tests)/out/font_1.svg");
	}
}
