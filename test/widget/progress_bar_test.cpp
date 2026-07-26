#include "berialdraw_imp.hpp"
using namespace berialdraw;


void ProgressBar::test1()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(13,7);
		window.size(400,200);

	Row * main = new Row(&window);
		Column * column_layout = new Column(main);
			ProgressBar * progress_bar_1 = new ProgressBar(column_layout);
				progress_bar_1->min_value(0);
				progress_bar_1->max_value(100);
				progress_bar_1->step_value(5);	
				progress_bar_1->id(1);
				progress_bar_1->border_color(Color::BLUE,64);
				progress_bar_1->align(Align::ALIGN_LEFT);

			ProgressBar * progress_bar_2 = new ProgressBar(column_layout);
				progress_bar_2->min_value(0);
				progress_bar_2->max_value(100);
				progress_bar_2->step_value(5);	
				progress_bar_2->id(2);
				progress_bar_2->border_color(Color::BLUE,64);
				progress_bar_2->align(Align::ALIGN_RIGHT);

		Row * row_layout = new Row(main);
			ProgressBar * progress_bar_3 = new ProgressBar(row_layout);
				progress_bar_3->min_value(0);
				progress_bar_3->max_value(100);
				progress_bar_3->step_value(5);	
				progress_bar_3->id(3);
				progress_bar_3->border_color(Color::BLUE,64);
				progress_bar_3->extend(Extend::EXTEND_HEIGHT);
				progress_bar_3->align(Align::ALIGN_BOTTOM);

			ProgressBar * progress_bar_4 = new ProgressBar(row_layout);
				progress_bar_4->min_value(0);
				progress_bar_4->max_value(100);
				progress_bar_4->step_value(5);	
				progress_bar_4->id(4);
				progress_bar_4->border_color(Color::BLUE,64);
				progress_bar_4->align(Align::ALIGN_TOP);
				progress_bar_4->extend(Extend::EXTEND_HEIGHT);

	int id = 0;
	for (int32_t value = progress_bar_1->min_value(); value <= progress_bar_1->max_value(); value ++)
	{
		progress_bar_1->value(value);
		progress_bar_1->thickness(value%5);
		progress_bar_2->value(value);
		progress_bar_2->thickness(value%5);
		progress_bar_3->value(value);
		progress_bar_3->thickness(value%5);
		progress_bar_4->value(value);
		progress_bar_4->thickness(value%5);

		if (value % 13 == 0)
		{
			String name;
			name.print("$(ui.tests)/out/progress_bar1_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}

	UIManager::desktop()->dispatch();
}

void ProgressBar::test2()
{
	Window window;
		window.position(13,7);
		window.size(400,200);

	Row * main = new Row(&window);
		Column * column_layout = new Column(main);
			ProgressBar * progress_bar_1 = new ProgressBar(column_layout);
				progress_bar_1->min_value(0);
				progress_bar_1->max_value(100);
				progress_bar_1->step_value(5);	
				progress_bar_1->id(1);
				progress_bar_1->border_color(Color::BLUE,64);
				progress_bar_1->align(Align::ALIGN_LEFT);
				progress_bar_1->fill_size(20);
				progress_bar_1->radius(5);
				progress_bar_1->track_size(2);
				progress_bar_1->thickness(0);


			ProgressBar * progress_bar_2 = new ProgressBar(column_layout);
				progress_bar_2->min_value(0);
				progress_bar_2->max_value(100);
				progress_bar_2->step_value(5);	
				progress_bar_2->id(2);
				progress_bar_2->border_color(Color::BLUE,64);
				progress_bar_2->align(Align::ALIGN_RIGHT);
				progress_bar_2->fill_size(10);
				progress_bar_2->radius(5);
				progress_bar_2->track_size(2);
				progress_bar_2->thickness(0);


		Row * row_layout = new Row(main);
			ProgressBar * progress_bar_3 = new ProgressBar(row_layout);
				progress_bar_3->min_value(0);
				progress_bar_3->max_value(100);
				progress_bar_3->step_value(5);	
				progress_bar_3->id(3);
				progress_bar_3->border_color(Color::BLUE,64);
				progress_bar_3->extend(Extend::EXTEND_HEIGHT);
				progress_bar_3->align(Align::ALIGN_BOTTOM);
				progress_bar_3->fill_size(10);
				progress_bar_3->radius(5);
				progress_bar_3->track_size(2);
				progress_bar_3->thickness(0);

			ProgressBar * progress_bar_4 = new ProgressBar(row_layout);
				progress_bar_4->min_value(0);
				progress_bar_4->max_value(100);
				progress_bar_4->step_value(5);	
				progress_bar_4->id(4);
				progress_bar_4->border_color(Color::BLUE,64);
				progress_bar_4->align(Align::ALIGN_TOP);
				progress_bar_4->extend(Extend::EXTEND_HEIGHT);
				progress_bar_4->fill_size(10);
				progress_bar_4->radius(5);
				progress_bar_4->track_size(6);
				progress_bar_4->thickness(1);

	int id = 0;
	for (int32_t value = progress_bar_1->min_value(); value <= progress_bar_1->max_value(); value ++)
	{
		progress_bar_1->value(value);
		progress_bar_1->thickness(value%5);
		progress_bar_2->value(value);
		progress_bar_2->thickness(value%5);
		progress_bar_3->value(value);
		progress_bar_3->thickness(value%5);
		progress_bar_4->value(value);
		progress_bar_4->thickness(value%5);

		if (value % 13 == 0)
		{
			String name;
			name.print("$(ui.tests)/out/progress_bar2_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}
}

void ProgressBar::test3()
{
	Window window;
		window.position(13,7);
		window.size(400,200);

	Row * main = new Row(&window);
		Column * column_layout = new Column(main);
			ProgressBar * progress_bar_1 = new ProgressBar(column_layout);
				progress_bar_1->min_value(0);
				progress_bar_1->max_value(100);
				progress_bar_1->step_value(5);	
				progress_bar_1->id(1);
				progress_bar_1->border_color(Color::BLUE,64);
				progress_bar_1->align(Align::ALIGN_LEFT);
				progress_bar_1->fill_size(10);
				progress_bar_1->radius(5);
				progress_bar_1->track_size(20);

			ProgressBar * progress_bar_2 = new ProgressBar(column_layout);
				progress_bar_2->min_value(0);
				progress_bar_2->max_value(100);
				progress_bar_2->step_value(5);	
				progress_bar_2->id(2);
				progress_bar_2->border_color(Color::BLUE,64);
				progress_bar_2->align(Align::ALIGN_RIGHT);
				progress_bar_2->fill_size(10);
				progress_bar_2->radius(5);
				progress_bar_2->track_size(20);

		Row * row_layout = new Row(main);
			ProgressBar * progress_bar_3 = new ProgressBar(row_layout);
				progress_bar_3->min_value(0);
				progress_bar_3->max_value(100);
				progress_bar_3->step_value(5);	
				progress_bar_3->id(3);
				progress_bar_3->border_color(Color::BLUE,64);
				progress_bar_3->extend(Extend::EXTEND_HEIGHT);
				progress_bar_3->align(Align::ALIGN_BOTTOM);
				progress_bar_3->fill_size(10);
				progress_bar_3->radius(5);
				progress_bar_3->track_size(20);

			ProgressBar * progress_bar_4 = new ProgressBar(row_layout);
				progress_bar_4->min_value(0);
				progress_bar_4->max_value(100);
				progress_bar_4->step_value(5);	
				progress_bar_4->id(4);
				progress_bar_4->border_color(Color::BLUE,64);
				progress_bar_4->align(Align::ALIGN_TOP);
				progress_bar_4->extend(Extend::EXTEND_HEIGHT);
				progress_bar_4->fill_size(10);
				progress_bar_4->radius(5);
				progress_bar_4->track_size(20);

	int id = 0;
	for (int32_t value = progress_bar_1->min_value(); value <= progress_bar_1->max_value(); value ++)
	{
		//value = 50;
		progress_bar_1->value(value);
		progress_bar_1->thickness(value%5);
		progress_bar_2->value(value);
		progress_bar_2->thickness(value%5);
		progress_bar_3->value(value);
		progress_bar_3->thickness(value%5);
		progress_bar_4->value(value);
		progress_bar_4->thickness(value%5);

		if (value % 13 == 0)
		{
			String name;
			name.print("$(ui.tests)/out/progress_bar3_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}
}

void ProgressBar::test4()
{
}

void ProgressBar::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test4();
		test3();
		test2();
		test1();
	}
}
