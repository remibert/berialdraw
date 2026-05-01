#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Outline::test()
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

		Rect * rect = new Rect(canvas);

			rect->size(200,50);
			rect->radius(10);
			rect->color(PURPLE);
			rect->thickness(5);
			rect->position(0,0);
			rect->center(0,0);
			rect->angle(0);

		UIManager::desktop()->dispatch();
		const Polygon & poly = rect->polygon();
	
		Outline outline(poly.outline());
		outline.next_contour();

		SvgOut svg;
		svg.add_path(outline, rect->color());
		String s = svg.get().c_str();
		for (int i = 0; i < (int)s.size(); i++)
		{
			if (s.get(i) == '\r' || s.get(i) == '\n' || s.get(i) == '\t')
			{
				s.replace(' ',i);
			}
		}
		//assert(s == " <path style=\"fill:#800080;\" d=\"M 187.50,-2.50 Q 193.70,-2.50 198.09,1.89 Q 202.50,6.30 202.50,12.50 L 202.50,37.50 Q 202.50,43.70 198.09,48.09 Q 193.70,52.50 187.50,52.50 L 12.50,52.50 Q 6.30,52.50 1.89,48.09 Q -2.50,43.70 -2.50,37.50 L -2.50,12.50 Q -2.50,6.30 1.89,1.89 Q 6.30,-2.50 12.50,-2.50 L 187.50,-2.50 M 12.50,2.50 Q 8.36,2.50 5.42,5.42 Q 2.50,8.36 2.50,12.50 L 2.50,37.50 Q 2.50,41.64 5.42,44.56 Q 8.36,47.50 12.50,47.50 L 187.50,47.50 Q 191.64,47.50 194.56,44.56 Q 197.50,41.64 197.50,37.50 L 197.50,12.50 Q 197.50,8.36 194.56,5.42 Q 191.64,2.50 187.50,2.50 L 12.50,2.50 Z\" /> </svg> ");
		assert(s == " <path style=\"fill:#800080;\" d=\"M -2.50,10 C -2.50,3.11 3.11,-2.50 10,-2.50 L 190,-2.50 C 196.89,-2.50 202.50,3.11 202.50,10 L 202.50,40 C 202.50,46.89 196.89,52.50 190,52.50 L 10,52.50 C 3.11,52.50 -2.50,46.89 -2.50,40 L -2.50,10 M 10,2.50 C 5.86,2.50 2.50,5.86 2.50,10 L 2.50,40 C 2.50,44.14 5.86,47.50 10,47.50 L 190,47.50 C 194.14,47.50 197.50,44.14 197.50,40 L 197.50,10 C 197.50,5.86 194.14,2.50 190,2.50 L 10,2.50 Z\" /> </svg> ");
	}
}
