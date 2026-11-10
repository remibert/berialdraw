#include "berialdraw_imp.hpp"
using namespace berialdraw;

#ifdef _DEBUG
// Test grid shape rendering
void GridShape::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		Window window;
			window.size(480, 480);
			window.color(Color::WHITE);

		Canvas * canvas = new Canvas(&window);
			canvas->position(0, 0);
			canvas->size(window.size());

		GridShape * grid = new GridShape(canvas);
			grid->horizontal_thickness(3);
			grid->vertical_thickness(2);
			grid->position(40, 40);
			grid->size(401, 409);
			grid->rows(7);
			grid->columns(11);
			grid->grid_color(0xFF404040);

		UIManager::desktop()->dispatch("$(ui.tests)/out/grid_shape1.svg");
	}
}
#endif