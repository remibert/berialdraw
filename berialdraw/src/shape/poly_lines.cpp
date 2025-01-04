#include "berialdraw_imp.hpp"

using namespace berialdraw;

PolyLines::PolyLines(Canvas * canvas):
	Shape(canvas),
	m_count(0)
{
}

PolyLines::PolyLines(const PolyLines & other):
	Shape(other),
	m_count(0),
	m_shape(other.m_shape),
	m_points(other.m_points)
{
}

void PolyLines::prepend(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this);
	Point p(x,y,true);
	m_points.prepend(p);
}

void PolyLines::prepend_(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this);
	Point p(x,y,false);
	m_points.prepend(p);
}

void PolyLines::prepend(const Point & p)
{
	UIManager::invalidator()->dirty(this);
	m_points.prepend(p);
}

void PolyLines::append(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this);
	Point p(x,y,true);
	m_points.append(p);
}

void PolyLines::append_(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this);
	Point p(x,y,false);
	m_points.append(p);
}

void PolyLines::append(const Point & p)
{
	UIManager::invalidator()->dirty(this);
	m_points.append(p);
}

void PolyLines::add_right(const Point & p)
{
	m_shape.prepend(p);
}

void PolyLines::add_left(const Point & p)
{
	m_shape.append(p);
}

void PolyLines::clear()
{
	m_shape.clear();
	m_points.clear();
}

void PolyLines::get_side(const Point& p1, const Point& p2, Dim thickness, bool right, Linear& linear)
{
	Linear result;
	Coord dx;
	Coord dy;
	Line::get_thickness(p1.x_(), p1.y_(), p2.x_(), p2.y_(), thickness, dx, dy);
	int64_t angle  = Linear::cross_product(p1.x_(), p1.y_(), p2.x_(), p2.y_(), p1.x_() - dx, p1.y_() + dy);

	if ((angle < 0 && right) || (angle >= 0 && !right))
	{
		linear.set_coef(p1.x_() - dx, p1.y_() + dy, p2.x_() - dx, p2.y_() + dy);
	}
	else
	{
		linear.set_coef(p1.x_() + dx, p1.y_()- dy, p2.x_() + dx, p2.y_() - dy);
	}
}

void PolyLines::add_junction(const Point &p1, const Point &p2, const Point & p3)
{
	int64_t cross_product = Linear::cross_product(p1, p2, p3);
	
	// Parallel lines
	if (cross_product == 0 && Linear::is_in_segment(p2,p1,p3))
	{
		cross_product = cross_product;
	}
	// Crossed lines
	else
	{
		int64_t dot_product = Linear::dot_product(p1, p2, p3);
		int64_t norms1 = Linear::norms(p1,p2);
		int64_t norms2 = Linear::norms(p2,p3);
		int64_t cos = 255;

		if (norms1 != 0 && norms2 != 0)
		{
			cos = dot_product / ((norms1*norms2)>>8);
		}

		// Right angle or obtuse angle
		if (dot_product <= 0 || (cos < 200))
		{
			Point right;
			Point left;
			Linear linear1;
			Linear linear2;
			get_side(p1, p2, m_thickness, true, linear1);
			get_side(p2, p3, m_thickness, true, linear2);
			linear1.get_intersection(linear2, right);
			add_right(right);

			get_side(p1, p2, m_thickness, false, linear1);
			get_side(p2, p3, m_thickness, false, linear2);
			linear1.get_intersection(linear2, left);
			add_left(left);
		}
		// Acute angle
		else
		{
			Linear line1(p1,p2);
			Linear line2(p2,p3);
			Point right; Point left;
				
			// Add end of first line
			get_end(p1, p2,right,left);
			add_right(right); // Add right border
			add_left(left);   // Add left border

			// Add the start of second line
			get_start(p2, p3,right,left);
			add_right(right); // Add right border
			add_left(left);   // Add left border
		}
	}
}

void PolyLines::get_start(const Point& p1,const Point& p2,Point & right,Point & left)
{
	Coord dx;
	Coord dy;

	Line::get_thickness(p1.x_(), p1.y_(), p2.x_(), p2.y_(), m_thickness, dx, dy);

	int64_t angle = Linear::cross_product(p1.x_(), p1.y_(), p2.x_(), p2.y_(), p1.x_() - dx, p1.y_() + dy);

	if (angle > 0)
	{
		left.x_ (p1.x_() - dx);
		left.y_ (p1.y_() + dy);
		right.x_(p1.x_() + dx);
		right.y_(p1.y_() - dy);
	}
	else
	{
		left.x_ (p1.x_() + dx);
		left.y_ (p1.y_() - dy);
		right.x_(p1.x_() - dx);
		right.y_(p1.y_() + dy);
	}
}

void PolyLines::add_start(const Point& p1, const Point& p2)
{
	Point right;
	Point left;
	get_start(p1, p2, right, left);
	add_left(left);
	add_right(right);
	///bd_printf("start %d,%d %d,%d\n",left.x,left.y_(),right.x,right.y_());
}

void PolyLines::get_end(const Point& p1,const Point& p2,Point & right,Point & left)
{
	Coord dx;
	Coord dy;

	Line::get_thickness(p1.x_(), p1.y_(), p2.x_(), p2.y_(), m_thickness, dx, dy);

	int64_t angle = Linear::cross_product(p1.x_(), p1.y_(), p2.x_(), p2.y_(), p1.x_() - dx, p1.y_() + dy);

	if (angle > 0)
	{
		left. x_(p2.x_() - dx);
		left. y_(p2.y_() + dy);
		right.x_(p2.x_() + dx);
		right.y_(p2.y_() - dy);
	}
	else
	{
		left. x_(p2.x_() + dx);
		left. y_(p2.y_() - dy);
		right.x_(p2.x_() - dx);
		right.y_(p2.y_() + dy);
	}
}

void PolyLines::add_end(const Point& p1, const Point& p2)
{
	Point right;
	Point left;
	get_end(p1, p2, right, left);
	add_left(left);
	add_right(right);
	//bd_printf("end %d,%d %d,%d\n",left.x,left.y_(),right.x,right.y_());
}

void PolyLines::build()
{
	Point p1, p2, p3;

	m_shape.clear();
	m_polygon.clear();
	int32_t index=0;

	if (m_points.get_first(p1) && m_points.get_next(p2))
	{
		add_start(p1, p2);
		while (m_points.get_next(p3))
		{
			add_junction(p1, p2, p3);
			p1 = p2;
			p2 = p3;
		}
		add_end(p1, p2);

		m_polygon.clear();
		Point p;
		if (m_shape.get_first(p))
		{
			Point first(p);
			do
			{
				m_polygon.add_point(p);
			}
			while (m_shape.get_next(p));
			m_polygon.next_contour();
		}
	}
}

// Render outline
void PolyLines::paint(const Point & shift)
{
	if (UIManager::invalidator()->is_dirty(this)  || UIManager::invalidator()->is_dirty(m_canvas) || ShapeStyle::is_dirty())
	{
		build();
	}
	UIManager::renderer()->draw(*this, shift);
}

#ifdef _DEBUG

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
				name.print("test/out/polyline0_%d.svg", ++id);
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
					Line corner;
					corner.point1(x,y);
					corner.point2(x+(1),y+(1));
					corner.thickness(3);
					corner.color(Color::BLACK);
				}
				poly->append(x,y);
			}

			String name;
			name.print("test/out/polylines1_%d.svg", ++id);
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

	UIManager::desktop()->dispatch("test/out/polyline2.svg");
}

void PolyLines::test3()
{
}

void PolyLines::test()
{
	test2();
	test1();
	test0();
}
#endif
