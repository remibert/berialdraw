#include "berialdraw_imp.hpp"
using namespace berialdraw;


void LineChart::test()
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

		set_rand(1);
		DataSet data1;
			data1.text("Data set 1");
			for(int i = 0; i < 100; i++)
			{
				data1.add(get_rand(-100,100));
			}
			data1.border_color(DARK_GREEN);
			data1.thickness(1);

		DataSet data2;
			data2.text("Data set 2");
			for(int i = 0; i < 100; i++)
			{
				data2.add(get_rand(-100,100));
			}
			data2.border_color(ROSY_BROWN);
			data2.thickness(1);

		LineChart * chart = new LineChart(canvas);
			chart->size(460,460);
			chart->position(10,10);
			chart->clear();
			chart->text("Line chart");
			chart->add(data1);
			chart->add(data2);

		UIManager::desktop()->dispatch("$(ui.tests)/out/linechart.svg");
	}
}
