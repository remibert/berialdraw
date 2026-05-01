#include "berialdraw_imp.hpp"
using namespace berialdraw;


void clock_face(Canvas * canvas, const Point & center, Dim radius)
{
	// Background
	Circle * back = new Circle(canvas);
		back->position(center);
		back->radius(radius);
		back->color(Color::WHITE);
		back->thickness(0);

	Circle * border = new Circle(canvas);
		border->position(center);
		border->radius(radius);
		border->color(Color::COLD_METAL);
		border->thickness(8);
		
	// Time mark
	Line * time_marks = new Line(canvas);
		time_marks->point1(0,0);
		time_marks->point2(0,10);
		time_marks->position(center);
		time_marks->thickness(2);
		time_marks->center(0,radius-(border->thickness()/2));
		time_marks->color(Color::COLD_METAL);
		time_marks->repeat(Shape::REPEAT_ANGLE, 0, 360, 360/60);

	time_marks = new Line(canvas);
		time_marks->point1(0,0);
		time_marks->point2(0,20);
		time_marks->position(center);
		time_marks->thickness(6);
		time_marks->center(0,radius-(border->thickness()/2));
		time_marks->color(Color::SALAD_GREEN);
		time_marks->repeat(Shape::REPEAT_ANGLE, 0, 360, 30);
	
	// Digits
	{
		Compass compass;
		compass.center(center);
		compass.radius(radius-63);

		Text * text;
		
		int time = 1;

		int step_angle = (360/12);
		int start_angle = 360 + 90-step_angle;
		int end_angle = 90;

		Circle * back_text;
		uint32_t color;

		for(int angle = start_angle; angle >= end_angle; angle -= step_angle)
		{
			compass.angle(angle);
			color = Hsl::to_rgb(angle, 75, 50);

			back_text = new Circle(canvas);
				back_text->radius(33);
				back_text->thickness(0);
				back_text->position(compass.position());
				back_text->color(color);

			text = new Text(canvas);
				text->font_size(40);
				text->color(Color::WHITE);
				text->text("%d",time++);
				text->center(text->content_size().middle());
				text->position(compass.position());
		}
	}
}

// Clock
void PolyLines::test0()
{
	Window window;
		window.size(480,480);

	Canvas * canvas = new Canvas(&window);
		canvas->size(480,480);
		canvas->color(Color::LIGHT_BLACK);

	Point center(240,240);
	Compass compass_hour;
		compass_hour.center(center);
		compass_hour.radius(130);

	Compass compass_minute(compass_hour);
		compass_minute.radius(210);

	// Draw clock face
	clock_face(canvas, center, 220);

	PolyLines * clock_hand = new PolyLines(canvas);
		clock_hand->thickness(10);
		clock_hand->color(Color::BLURPLE);

	int c = 0;
	int id = 0;
	for(int32_t hour = 0; hour < 12; hour ++)
	{
		for(int32_t minute = 0; minute <= 60; minute += 10)
		{
			// Compute clock hand angle
			compass_hour.angle(90 - ((hour * 360)/12) - (((minute *360)/60)/12) );
			compass_minute.angle(90 - ((minute *360)/60));
			
			// Draw clock hand
			clock_hand->clear();
			clock_hand->append(compass_hour.position());
			clock_hand->append(center);
			clock_hand->append(compass_minute.position());

			if (++c % 3 == 0)
			{
				String name;
				name.print("$(ui.tests)/out/polyline0_%d.svg", ++id);
				UIManager::desktop()->dispatch(name);
			}
			UIManager::desktop()->dispatch();
		}
	}
}

void PolyLines::test1()
{
	Window window;
		window.color(Color::WHITE);
		window.size(480,480);

	Canvas * canvas = new Canvas(&window);
		canvas->size(480,480);
		canvas->color(Color::LAVENDER);

	srand(0);   // Initialization, should only be called once.
	double radius = 5000;
	int32_t offset = 10000;
	uint32_t thickness = 10;
	int id = 0;
	set_rand(1);
	for(int j = 0; j < 10; j++)
	{
		{
			int32_t x;
			int32_t y;
			PolyLines * poly = new PolyLines(canvas);
			poly->thickness(thickness);
			poly->color(Color::GREEN, 64);
			
			for(int i = 0; i < 20; i++)
			{
				x = get_rand(10,UIManager::renderer()->size().width()-10) ;
				y = get_rand(10,UIManager::renderer()->size().height()-10) ;
				{
					Line corner(canvas);
					corner.point1(x,y);
					corner.point2(x+(1),y+(1));
					corner.thickness(3);
					corner.color(Color::BLACK);
				}
				poly->append(x,y);
			}

			String name;
			name.print("$(ui.tests)/out/polylines1_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
			delete poly;
		}
	}
}

// Line graph
void PolyLines::test2()
{
	Window window;
		window.color(Color::WHITE);
		window.size(480,480);

	Canvas * canvas = new Canvas(&window);
		canvas->size(480,480);
		canvas->color(Color::LIGHT_YELLOW);

	// Axis
	{
		Line * axis = new Line(canvas);
			axis->point1(0,0);
			axis->point2(460,0);
			axis->color(Color::LIGHT_BLACK);
			axis->thickness_(1<<5);
			axis->center(0,0);
			axis->position(10,470);


		axis = new Line(canvas);
			axis->point1(0,0);
			axis->point2(460,0);
			axis->color(Color::LIGHT_BLACK);
			axis->thickness_(1<<5);
			axis->center(0,0);
			axis->position(10,470);
			axis->angle(90);

		Triangle * arrow = new Triangle(canvas);
			arrow->color(Color::LIGHT_BLACK);
			arrow->center(0,0);
			arrow->radius(5);
			arrow->angle(270);
			arrow->position(470,470);


		arrow = new Triangle(canvas);
			arrow->color(Color::LIGHT_BLACK);
			arrow->center(0,0);
			arrow->radius(5);
			arrow->angle(270);
			arrow->position(10,10);
			arrow->angle(0);
	}

	Point origin(10, 470);
	
	set_rand(1);   // Initialization, should only be called once.

	{
		PolyLines * lines = new PolyLines(canvas);
		lines->thickness(1);
		Triangle * marker;
		lines->color(Color::BEER);

		lines->append(origin);
		for(int32_t x = 10; x < 470; x+=10)
		{
			int32_t y = get_rand(10,130);
			lines->append(origin.x() + x, origin.y() - y);
			marker = new Triangle(canvas);
			marker->color(lines->color());
			marker->thickness(1);
			marker->radius(5);
			marker->position(origin.x() + x, origin.y() - y);
		}
	}

	{
		PolyLines * lines = new PolyLines(canvas);
		lines->thickness(1);
		Cross * marker;
		lines->color(Color::BLUE_LAGOON);

		lines->append(origin);
		for(int32_t x = 10; x < 470; x+=10)
		{
			int32_t y = get_rand(10,130);
			lines->append(origin.x() + x, origin.y() - y);
			marker = new Cross(canvas);
			marker->color(lines->color());
			marker->thickness(1);
			marker->radius(5);
			marker->position(origin.x() + x, origin.y() - y);
		}
	}

	UIManager::desktop()->dispatch("$(ui.tests)/out/polyline2.svg");
}

void PolyLines::test3()
{
}

void PolyLines::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test2();
		test1();
		test0();
	}
}
