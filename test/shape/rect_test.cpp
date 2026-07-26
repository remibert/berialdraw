#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Rect::test1()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());
		canvas->color(Color::YELLOW,32);

	Rect * roundrect = new Rect(canvas);
		roundrect->position(10, 10);
		roundrect->radius(50);
		roundrect->size(460, 460);
		roundrect->thickness(10);
		roundrect->color(0x3FFF0000);

	Rect * fillroundrect = new Rect(canvas);
		fillroundrect->position(30, 30);
		fillroundrect->radius(30);
		fillroundrect->size(420, 420);
		fillroundrect->color(0x3F00FF00);

	Rect * rect = new Rect(canvas);
		rect->position(140, 140);
		rect->size(200, 200);
		rect->color(0x3F0000FF);

	UIManager::desktop()->dispatch("$(ui.tests)/out/rect1.svg");
}

void Rect::test2()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());
		canvas->color(Color::YELLOW,32);

	Point center(240,240);

	uint32_t angle = 10;

	Point radius(-120,0);

	Cross * cross = new Cross(canvas);
		cross->position(center);

	Circle * circle = new Circle(canvas);
		circle->radius(120);
		circle->position(center);
		circle->thickness(2);

	for (angle = 0; angle < 360; angle += 15)
	{
		Rect * background = new Rect(canvas);
		Text * label = new Text(canvas);
			label->font_size(20);
			label->text("Hello");
			label->center(radius);
			label->position(center);
			label->color(Color::BLUE);
			label->margin(3,3,3,3);
			label->angle(angle);

			background->center(radius);
			background->position(center);
			background->radius(8);
			background->size(label->marged_size());
			background->color(Color::CYAN, 32);
			background->thickness(0);
			background->angle(angle);
		
		background = new Rect(canvas);
			background->center(radius);
			background->position(center);
			background->radius(8);
			background->size(label->marged_size());
			background->angle(angle);
			background->thickness(2);
			background->color(Color::BLUE);
	}
	UIManager::desktop()->dispatch("$(ui.tests)/out/rect2.svg");
}

void Rect::test3()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE_ICE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());

	Point position(0,0);

	Coord row = 10;
	Coord column = 10;
	position.set(column,row);

	// Button normal
	{
		Rect * background = new Rect(canvas);
		Text * label = new Text(canvas);
			label->text("NORMAL");
			label->position(position);
			label->color(Color::WHITE);
			label->margin(10,10,10,10);
			label->font_size(20);

			background->position(position);
			background->radius(4);
			background->size(label->marged_size());
			background->color(Color::DODGER_BLUE);
			background->thickness(0);

		column += label->marged_size().width() + 10;
	}
	UIManager::desktop()->dispatch();

	position.set(column, row);

	// Button pressed
	{
		Rect * background = new Rect(canvas);
		Text * label = new Text(canvas);
			label->text("PRESSED");
			label->position(position);
			label->color(Color::DODGER_BLUE);
			label->margin(10,10,10,10);
			label->font_size(20);

			background->position(position);
			background->radius(4);
			background->size(label->marged_size());
			background->color(Color::SILVER_WHITE);
			background->thickness(0);

		row += label->marged_size().height() + 10;
		column = 10;
	}

	position.set(column, row);

	// Input text
	{
		Text * label = new Text(canvas);
			label->text("Input tex");
			label->position(position);
			label->color(Color::BLACK);
			label->margin(10,10,5,10);
			label->font_size(20);

		Rect * background = new Rect(canvas);
			background->position(position.x(), position.y() + label->marged_size().height());
			background->radius(0);
			background->size(label->marged_size().width(), 1);
			background->color(Color::DODGER_BLUE);
			background->thickness(2);

		Rect * cursor = new Rect(canvas);
			cursor->position(position.x() + label->content_size().width() + label->margin().left(), position.y() + label->margin().top());	
			cursor->size(1, label->content_size().height());
			cursor->color(Color::COLUMBIA_BLUE);

		row += label->marged_size().height() + 10;
		column = 10;
	}
	UIManager::desktop()->dispatch();

	position.set(column, row);

	// Switch off
	{
		Rect * back = new Rect(canvas);
			back->color(Color::GRAY_CLOUD);
			back->size(50, 20);
			back->radius(10);
			back->position(position.x()+5, position.y()+5);

		Circle * switch_ = new Circle(canvas);
			switch_->color(Color::BLACK,16);
			switch_->radius(15);
			switch_->thickness(0);
			switch_->position(position.x() + 15+2, position.y()+15+2);
	
			switch_ = new Circle(canvas);
			switch_->color(Color::PLATINUM);
			switch_->radius(15);
			switch_->thickness(0);
			switch_->position(position.x() + 15, position.y()+15);
	}

	row += 40;
	position.set(column, row);

	// Switch on
	{
		Rect * back = new Rect(canvas);
			back->color(Color::JEANS_BLUE);
			back->size(50, 20);
			back->radius(10);
			back->position(position.x()+5, position.y()+5);

		Circle * switch_ = new Circle(canvas);
			switch_->color(Color::BLACK,32);
			switch_->radius(15);
			switch_->thickness(0);
			switch_->position(position.x() + 15+2+30, position.y()+15+2);
	
			switch_->color(Color::DODGER_BLUE);
			switch_->radius(15);
			switch_->thickness(0);
			switch_->position(position.x() + 15+30, position.y()+15);
	}

	// Footer
	{
		Rect * background = new Rect(canvas);
			background->position(0,425);
			background->color(Color::BLACK);
			background->size(480,55);
			background->thickness(0);

		Triangle * triangle = new Triangle(canvas);
			triangle->color(Color::WHITE);
			triangle->radius(13);
			triangle->position(110,452);
			triangle->thickness(3);
			triangle->angle(90);

		Circle * circle = new Circle(canvas);
			circle->color(Color::WHITE);
			circle->radius(13);
			circle->position(240,452);
			circle->thickness(3);
			circle->angle(90);

		Square * square = new Square(canvas);
			square->color(Color::WHITE);
			square->radius(18);
			square->position(240+130,452);
			square->thickness(3);
			square->angle(90);
	}
 
	UIManager::desktop()->dispatch("$(ui.tests)/out/rect3.svg");
}

void Rect::test4()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());

	Rect * rect = new Rect(canvas);
		rect->position(10, 10);
		rect->size(460, 460);
		rect->color(Color::RED,32);

		rect = new Rect(canvas);
		rect->position(5, 5);
		rect->size(470, 470);
		rect->thickness(10);
		rect->color(Color::BLUE,32);

		canvas->clear();

		rect = new Rect(canvas);
		rect->position(10, 10);
		rect->size(460, 460);
		rect->color(Color::RED,32);
		rect->radius(35);

		rect = new Rect(canvas);
		rect->position(5, 5);
		rect->size(470, 470);
		rect->thickness(10);
		rect->radius(40);
		rect->color(Color::BLUE,32);

	UIManager::desktop()->dispatch("$(ui.tests)/out/rect4.svg");
}

void Rect::test5()
{
	int row = 0;
	int column = 0;
	Window window;
	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::WHITE);
		canvas->extend(Extend::EXTEND_ALL);
		canvas->clear();
	Rect * rect = 0;
	int id = 0;

	for (Dim thickness = 0; thickness <= 16; )
	{
		for (Dim radius = 0; radius <= 16; )
		{
			row = 0;
			column = 0;
			for (int border = NO_BORDER; border <= ALL_BORDERS; border++)
			{
				rect = new Rect(canvas);
					rect->position(15+column*120, 15+row*60);
					rect->size(80, 35);
					rect->color(Color::RED,58);
					rect->thickness(thickness);
					rect->radius(radius);

				rect = new Rect(*rect);
					rect->color(Color::BLUE,64);
					rect->borders(border);

				rect = new Rect(*rect);
					rect->color(Color::GREEN,64);
					rect->borders(border | INNER_AREA);


				rect = new Rect(canvas);
					rect->position(15+column*120, 15+(row+4)*58);
					rect->size(80, 35);
					rect->color(Color::RED,64);
					rect->thickness(thickness);
					rect->radius(radius);
					
				rect = new Rect(*rect);
					rect->color(Color::BLUE,64);
					rect->borders(border | RIGHT_ANGLE_END);

				rect = new Rect(*rect);
					rect->color(Color::GREEN,64);
					rect->borders(border | RIGHT_ANGLE_END | INNER_AREA);

				column ++;
				if (column >= 4)
				{
					row ++;
					column = 0;
				}
			}
			if (radius == 0)
			{
				radius = 1;
			}
			else
			{
				radius <<= 1;
			}
			{
				String name;
				name.print("$(ui.tests)/out/rect5_%d.svg", ++id);
				UIManager::desktop()->dispatch(name);
			}
			canvas->clear();
		}
		if (thickness == 0)
		{
			thickness = 1;
		}
		else
		{
			thickness <<= 1;
		}
	}
}

void Rect::test6()
{
	int row = 0;
	int column = 0;
	Window window;
	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::WHITE);
		canvas->extend(Extend::EXTEND_ALL);
		canvas->clear();
	Rect * rect = 0;
	int id = 0;

	for (Dim thickness=1; thickness <= 32; )
	{
		for (Dim radius = 0; radius <= 32; )
		{
			row = 0;
			column = 0;
			for (int border = NO_BORDER; border <= ALL_BORDERS; border++)
			{
				rect = new Rect(canvas);
					rect->position(15+column*120, 15+row*100);
					rect->size(80, 60);
					rect->thickness(thickness);
					rect->radius(radius);
					rect->color(Color::BLUE,64);
					rect->borders(border| ROUNDED_END);

				rect = new Rect(*rect);
					rect->color(Color::GREEN,64);
					rect->borders(border | INNER_AREA | ROUNDED_END);

				column ++;
				if (column >= 4)
				{
					row ++;
					column = 0;
				}
			}
			if (radius < 8)
			{
				radius ++;
			}
			else
			{
				radius <<= 1;
			}
			{
				String name;
				name.print("$(ui.tests)/out/rect6_%d.svg", ++id);
				UIManager::desktop()->dispatch(name);
			}
			canvas->clear();
		}
		if (thickness < 8)
		{
			thickness++;
		}
		else
		{
			thickness <<= 1;
		}
	}
}

void Rect::test7()
{
	int row = 0;
	int column = 0;
	Window window;
	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::WHITE);
		canvas->extend(Extend::EXTEND_ALL);
		canvas->clear();
	Rect * rect = 0;
	int id = 0;

	for (Dim thickness=1; thickness <= 16; )
	{
		for (Dim radius = 0; radius <= 16; )
		{
			row = 0;
			column = 0;
			for (int border = NO_BORDER; border <= ALL_BORDERS; border++)
			{
				rect = new Rect(canvas);
					rect->position(15+column*120, 15+row*100);
					rect->size(80, 60);
					rect->thickness(thickness);
					rect->radius(radius);
					rect->color(Color::BLUE,64);
					rect->borders(border);

				rect = new Rect(*rect);
					rect->color(Color::GREEN,64);
					rect->borders(border | INNER_AREA);

				column ++;
				if (column >= 4)
				{
					row ++;
					column = 0;
				}
			}
			if (radius < 8)
			{
				radius ++;
			}
			else
			{
				radius <<= 1;
			}
			{
				String name;
				name.print("$(ui.tests)/out/rect7_%d.svg", ++id);
				UIManager::desktop()->dispatch(name);
			}
			canvas->clear();
		}
		if (thickness < 8)
		{
			thickness++;
		}
		else
		{
			thickness <<= 1;
		}
	}
}

void Rect::test8()
{
	int row = 0;
	int column = 0;
	Window window;
	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::WHITE);
		canvas->extend(Extend::EXTEND_ALL);
		canvas->clear();
	Rect * rect = 0;
	int id = 0;

	for (Dim thickness=1; thickness <= 16; )
	{
		for (Dim radius = 0; radius <= 16; )
		{
			row = 0;
			column = 0;
			for (int border = NO_BORDER; border <= ALL_BORDERS; border++)
			{
				rect = new Rect(canvas);
					rect->position(15+column*120, 15+row*100);
					rect->size(80, 60);
					rect->thickness(thickness);
					rect->radius(radius);
					rect->color(Color::BLUE,64);
					rect->borders(border | RIGHT_ANGLE_WITHOUT_BORDER);

				rect = new Rect(*rect);
					rect->color(Color::GREEN,64);
					rect->borders(border | INNER_AREA | RIGHT_ANGLE_WITHOUT_BORDER);

				column ++;
				if (column >= 4)
				{
					row ++;
					column = 0;
				}
			}
			if (radius < 8)
			{
				radius ++;
			}
			else
			{
				radius <<= 1;
			}
			{
				String name;
				name.print("$(ui.tests)/out/rect8_%d.svg", ++id);
				UIManager::desktop()->dispatch(name);
			}
			canvas->clear();
		}
		if (thickness < 8)
		{
			thickness++;
		}
		else
		{
			thickness <<= 1;
		}
	}
}


void Rect::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
	test5();

		test8();
		test7();
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
