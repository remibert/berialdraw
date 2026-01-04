#include "berialdraw_imp.hpp"

using namespace berialdraw;

inline void calc_point(Point & p, Dim radius, FT_Vector & sincos)
{
	p.x_(p.x_() + (Coord)((((int64_t)(sincos.x)*(int64_t)(radius)) + (1 << 15))>>16));
	p.y_(p.y_() + (Coord)((((int64_t)(sincos.y)*(int64_t)(radius)) + (1 << 15))>>16));
}

inline void calc_cubic(Point & p, Dim radius, FT_Vector & sincos)
{
	p.x_(p.x_() + (Coord)((((int64_t)(sincos.y)*(int64_t)(radius)) + (1 << 15))>>16));
	p.y_(p.y_() + (Coord)((((int64_t)(sincos.x)*(int64_t)(radius)) + (1 << 15))>>16));
}

Pie::Pie(Canvas * canvas) : 
	Shape(canvas, sizeof(Pie)),
	m_polygon(0)
{
	m_thickness = 0;
}

Pie::Pie(const Pie & other):
	Shape(other),
	RoundStyle(other),
	PieStyle(other),
	m_polygon(0)
{
}

void Pie::get_slice(const Point & center, Dim radius, Coord start_angle, Coord sweep_angle, Point & p1, Point & c1, Point &c2, Point &p2)
{
	FT_Vector sincos;
	Coord d;

	d = ((int64_t)radius * 4521);
	d = (Coord)(((((int64_t)sweep_angle) * d) / (90<<6)) + (1)) >> 13;

	FT_Vector_Unit(&sincos, start_angle<<10);
	p1 = center;
	calc_point(p1, radius, sincos);
	calc_cubic(c1, d, sincos);
	c1.x_(p1.x_() - c1.x_());
	c1.y_(p1.y_() + c1.y_());

	FT_Vector_Unit(&sincos, (sweep_angle + start_angle)<<10);
	p2 = center;
	calc_point(p2, radius, sincos);
	calc_cubic(c2, d, sincos);
	c2.x_(p2.x_() + c2.x_());
	c2.y_(p2.y_() - c2.y_());
}

void Pie::build_filled(Dim radius)
{
	Coord start_angle;
	Coord sweep_angle;
	Coord angle;


	if (m_start_angle < m_end_angle)
	{
		sweep_angle = m_end_angle - m_start_angle;
	}
	else
	{
		sweep_angle = m_start_angle - m_end_angle;
	}

	start_angle = - m_start_angle - sweep_angle;

	while (sweep_angle > 0)
	{
		if (sweep_angle > (90<<6))
		{
			angle = (90<<6);
		}
		else
		{
			angle = sweep_angle;
		}
		Point p1;
		Point c1;
		Point c2;
		Point p2;
		get_slice(m_center, radius, start_angle, angle, p1, c1, c2, p2);
		m_polygon.add_point(p1);
		m_polygon.add_cubic(c1);
		m_polygon.add_cubic(c2);
		m_polygon.add_point(p2);
		start_angle += angle;
		sweep_angle -= angle;
	}
	m_polygon.add_point(m_center);
}

void Pie::build_empty()
{
	Dim radius = m_radius + (m_thickness>>1);
	Coord sweep_angle;
	Coord end_angle;
	Coord angle;

	if (m_start_angle < m_end_angle)
	{
		sweep_angle = m_end_angle - m_start_angle;
	}
	else
	{
		sweep_angle = m_start_angle - m_end_angle;
	}

	end_angle = -m_start_angle;

	while (sweep_angle > 0)
	{
		if (sweep_angle > (90<<6))
		{
			angle = (90<<6);
		}
		else
		{
			angle = sweep_angle;
		}
		Point p1;
		Point c1;
		Point c2;
		Point p2;
		get_slice(m_center, radius, end_angle-angle, angle, p1, c1, c2, p2);
		m_polygon.add_point(p2);
		m_polygon.add_cubic(c2);
		m_polygon.add_cubic(c1);
		m_polygon.add_point(p1);
		end_angle -= angle;
		sweep_angle -= angle;
	}
}

void Pie::paint(const Point & shift)
{
	if (UIManager::invalidator()->is_dirty(m_canvas))
	{
		Coord sweep_angle;
		if (m_end_angle > (360<<6))
		{
			m_end_angle =  m_end_angle % (360<<6);
		}
		m_start_angle =  m_start_angle % (360<<6);

		if (m_start_angle < m_end_angle)
		{
			sweep_angle = m_end_angle - m_start_angle;
		}
		else
		{
			sweep_angle = m_start_angle - m_end_angle;
		}

		m_polygon.clear();
		if (m_thickness == 0)
		{
			build_filled(m_radius);
			m_polygon.add_point(m_center);
		}
		else
		{
			build_empty();
			build_filled(m_radius -(m_thickness>>1));

			if(m_rope)
			{
				Point p1;
				Point c1;
				Point c2;
				Point p2;

				get_slice(m_center, m_radius + (m_thickness >>1), - m_start_angle - sweep_angle, sweep_angle, p1, c1, c2, p2);

				PolyLines lines(0);
				lines.thickness_(m_thickness);
				lines.append(p1);
				lines.append(m_center);
				lines.append(p2);
				lines.build();
				m_polygon.next_contour();
				m_polygon.outline().append(lines.outline());
			}
		}
	}
	UIManager::renderer()->draw(*this, shift);
}

#ifdef _DEBUG
void Pie::test1()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());
		canvas->color(Color::YELLOW, 32);
	
	Pie * red_pie = new Pie(canvas);
		red_pie->color(Color::RED,32);
		red_pie->position(240, 240);
		red_pie->radius(220);
		red_pie->thickness(10);
		red_pie->rope(true);

	Pie * green_pie = new Pie(canvas);
		green_pie->color(Color::GREEN,32);
		green_pie->position(240, 240);
		green_pie->radius(220);
		green_pie->thickness(10);
		green_pie->rope(true);

	Pie * blue_pie = new Pie(canvas);
		blue_pie->position(240, 240);
		blue_pie->radius(220);
		blue_pie->thickness(10);
		blue_pie->rope(true);
		blue_pie->thickness(0);
		blue_pie->color(Color::BLUE,32);

	Cross  * cross = new Cross(canvas);
		cross->thickness(1);
		cross->color(Color::BLACK,32);
		cross->position(red_pie->center());

	Circle * circle = new Circle(canvas);
		circle->position(240,240);
		circle->color(Color::BLACK,32);
		circle->radius(240);
		circle->thickness(1);

	for(uint32_t angle = 0; angle <= 360; angle += 20)
	{
		blue_pie->start_angle(0);
		blue_pie->end_angle(angle);

		green_pie->start_angle(0);
		green_pie->end_angle(angle);

		red_pie->start_angle(angle);
		red_pie->end_angle(angle+angle);

		if (angle %60 == 0)
		{
			String test_name;
			test_name.print("$(ui.tests)/out/pie1_%d.svg",angle/60);
			UIManager::desktop()->dispatch(test_name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}
}

void Pie::dial(const Point & origin, Canvas * canvas)
{
	uint32_t percent =0;
	Text * text;
	Pie * border = new Pie(canvas);
		border->thickness(1);
		border->position(origin);
		border->start_angle(0);
		border->end_angle(180);
		border->color(0x3f000000);
		border->radius(90);

	Line * marks = new Line(canvas);
		marks->color(Color::BLACK,64);
		marks->point1(0,0);
		marks->point2(0,10);
		marks->position(origin);
		marks->thickness_(1<<5);
		marks->center(0,90);
		marks->repeat(Shape::REPEAT_ANGLE, 180, 0, 18);

	for(Coord angle=90; angle>= -90; angle -=18)
	{
		text = new Text(canvas);
		text->text("%d",percent);
		text->font_size(20);
		percent += 10;
		text->position(origin);
		text->center(text->content_size().width()/2, 110);
		text->angle(angle);
	}
}

void Pie::test2()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(10,10);
		canvas->size(window.size().width()-20, window.size().height()-20);
		canvas->color(Color::YELLOW, 32);
	
	Point origin(200, 200);
	Polygon * indicator = new Polygon(canvas);
	indicator->color(Color::RED);
	indicator->position(origin);
		indicator->add_point(-8, 0 );
		indicator->add_cubic(-8, 15);
		indicator->add_cubic( 8, 15);
		indicator->add_point( 8, 0 );
		indicator->add_point( 2, -80);
		indicator->add_point( 0, -83);
		indicator->add_point(-2, -80);
		indicator->next_contour();

		indicator->add_point(2,0);
		indicator->add_point(0,2);
		indicator->add_point(-2,0);
		indicator->add_point(0,-2);
		indicator->next_contour();
		
	dial(origin, canvas);
	
	int id =0;
	for(int32_t angle=90; angle>= -90; angle -=18)
	{
		indicator->angle(angle);
		String name;
		name.print("$(ui.tests)/out/pie2_%d.svg", ++id);
		UIManager::desktop()->dispatch(name);
	}
}

void Pie::test3()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(10,10);
		canvas->size(window.size().width()-20, window.size().height()-20);
		canvas->color(Color::YELLOW, 32);

	Pie * pie;
	uint32_t color;
	for (Coord angle  = 0; angle < 360; angle += 30)
	{
		color = Hsl::to_rgb(angle, 75, 75);
		pie = new Pie(canvas);
			pie->color(color);
			pie->position(230, 230);
			pie->radius(220);
			pie->thickness(5);
			pie->rope(true);
			pie->start_angle(angle);
			pie->end_angle(angle+30);

		pie = new Pie(*pie);
			pie->color(color, 196);
			pie->thickness(0);
	}
	UIManager::desktop()->dispatch("$(ui.tests)/out/pie3.svg");
}

void Pie::test4()
{
	Window window;
		window.position(0,0);
		window.size(480,480);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());
		canvas->color(Color::BLACK);

	// Background
	{
		Circle * border = new Circle(canvas);
			border->color(Color::GRAY);
			border->position(canvas->size().middle());
			border->radius(240);
			border->thickness(1);

		border = new Circle(canvas);
			border->color(Color::DARK_GRAY,128);
			border->position(canvas->size().middle());
			border->radius(225);
			border->thickness(4);

		border = new Circle(canvas);
			border->color(Color::DARK_GRAY,96);
			border->position(canvas->size().middle());
			border->radius(225);
			border->thickness(0);
	}

	// Graduations
	{
		Pie * pie = new Pie(canvas);
			pie->color(Color::NEON_BLUE);
			pie->position(canvas->size().middle());
			pie->radius(210);
			pie->thickness(5);
			pie->rope(false);
			pie->start_angle(-84);
			pie->end_angle(180);

		Line * marks = new Line(canvas);
			marks->color(Color::NEON_BLUE);
			marks->point1(0,0);
			marks->point2(0,14);
			marks->position(canvas->size().middle());
			marks->thickness(3);
			marks->center(0,212);
			marks->repeat(Shape::REPEAT_ANGLE, -84, 180, 22);

		marks = new Line(canvas);
			marks->color(Color::NEON_BLUE);
			marks->point1(0,0);
			marks->point2(0,10);
			marks->position(canvas->size().middle());
			marks->thickness(1);
			marks->center(0,212);
			marks->repeat(Shape::REPEAT_ANGLE, -84+11, 180-11, 22);
	}

	// Texts
	{
		Compass compass;
		compass.center(canvas->size().middle());
		compass.radius(170);

		Text * text;
		int speed = 0;

		for(int angle = 180; angle >= -84; angle -= 22)
		{
			compass.angle(angle);
			text = new Text(canvas);
				text->font_size(30);
				text->color(WHITE);
				text->text("%d",speed);
				text->center(text->content_size().middle());
				text->position(compass.position());
			speed += 20;
		}
	}

	Polygon * speed_indicator = new Polygon(canvas);
		speed_indicator->color(Color::RED);
		speed_indicator->position(canvas->size().middle());
		speed_indicator->add_point(-210, 3 );
		speed_indicator->add_cubic(-215,3);
		speed_indicator->add_cubic(-215,-3);
		speed_indicator->add_point(-210, -3 );

		speed_indicator->add_point(30, -8);
		speed_indicator->add_cubic(43, -8);
		speed_indicator->add_cubic(43,  8);
		speed_indicator->add_point(30,  8);
		speed_indicator->next_contour();
		speed_indicator->angle(0);

	Circle * circle = new Circle(canvas);
		circle->radius(20);
		circle->position(canvas->size().middle());
		circle->color(Color::WHITE);
		circle->thickness(0);

	Text * speed_text = new Text(canvas);
		speed_text->position(canvas->size().middle().width()/3, canvas->size().middle().height() + canvas->size().middle().height()/4);
		speed_text->font_size(70);
		speed_text->text("200");
		speed_text->color(Color::WHITE);

	
	int id = 0;
	for (int speed = 0; speed <= 240; speed+=20)
	{
		speed_indicator->angle(-(speed/20)*22);
		speed_text->text("%d",speed);

		if (speed % 40 == 0)
		{
			String name;
			name.print("$(ui.tests)/out/pie4_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}
}

void Pie::test5()
{
}


void Pie::test6()
{
}

void Pie::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}

#endif
