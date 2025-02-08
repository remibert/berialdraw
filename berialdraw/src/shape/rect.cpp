#include "berialdraw_imp.hpp"

using namespace berialdraw;

Rect::Rect(Canvas * canvas) : 
	Shape(canvas, sizeof(Rect)),
	m_polygon(0)
{
	m_thickness = 0;
}

Rect::Rect(const Rect & other):
	Shape(other),
	m_polygon(0)
{
}

Coord round_(Coord value)
{
	if ((value % 64) != 0)
	{
		value = ((value + 33) >> 6) << 6;
	}
	return value;
}

void Rect::build_polygon(const Area & area, const Point & shift_, Dim radius, Dim thickness, Dim gap, uint8_t sides, uint32_t backcolor, uint32_t bordercolor)
{
	thickness = min(area.size().width_(), min(area.size().height_(), thickness));

	Size  siz = area.size();
	Point pos = area.position();

	if(backcolor)
	{
		Dim delta = thickness>>7;
		pos.move(delta,delta);
		siz.decrease(delta<<1,delta<<1);
		Rect background(0);
			background.size(siz);
			background.radius_(radius);
			background.color(backcolor);
			background.paint(pos);
	}

	if(bordercolor && thickness)
	{
		Dim delta = (thickness+32+gap+gap)>>7;
		pos.move(0-delta,0-delta);
		siz.increase_(thickness+gap+gap,thickness+gap+gap);
		Rect border(0);
			border.thickness_(thickness);
			border.radius_(radius+gap);
			border.color(bordercolor);
			border.sides(sides);
			if ((thickness >> 6) % 2)
			{
				pos.move_(0- 32,0-32);
			}
			border.size(siz);
			border.paint(pos);
	}
}

// Render outline
void Rect::paint(const Point & shift)
{
	if (m_radius == 0 && m_thickness == 0 && m_angle == 0 && 
		m_center.x_() == 0 && m_center.y_() == 0 && UIManager::exporter() == 0)
	{
		Point move(shift);
		move.move(m_position);
		UIManager::renderer()->fillrect(move.x(),move.y(), m_size.width(),m_size.height(), color());
	}
	else
	{
		create_part();
		UIManager::renderer()->draw(*this, shift);
	}
}


void Rect::top_right_corner(Coord x, Coord y, Coord r, bool clockwise)
{
	Coord d = (r*27146)>>16;
	m_polygon.add_point_(x    , y    ); 
	if (r != 0)
	{
		if (clockwise)
		{
			m_polygon.add_conic_(x+d  , y    );
			m_polygon.add_conic_(x+r  , y+r-d);
			m_polygon.add_point_(x+r  , y+r  );
		}
		else
		{
			m_polygon.add_conic_(x    , y-d  );
			m_polygon.add_conic_(x-r+d, y-r  );
			m_polygon.add_point_(x-r  , y-r  ); 
		}
	}
}

void Rect::top_left_corner(Coord x, Coord y, Coord r, bool clockwise)
{
	Coord d = (r*27146)>>16;
	m_polygon.add_point_(x    , y    ); 
	if (r != 0)
	{
		if (clockwise)
		{
			m_polygon.add_conic_(x    , y-d );
			m_polygon.add_conic_(x+r-d, y-r );
			m_polygon.add_point_(x+r  , y-r );
		}
		else
		{
			m_polygon.add_conic_(x-d  ,y    );
			m_polygon.add_conic_(x-r  ,y+r-d);
			m_polygon.add_point_(x-r  ,y+r  );
		}
	}
}

void Rect::bottom_left_corner(Coord x, Coord y, Coord r, bool clockwise)
{
	Coord d = (r*27146)>>16;
	m_polygon.add_point_(x    , y    ); 
	if (r != 0)
	{
		if (clockwise)
		{
			m_polygon.add_conic_(x-d  , y    );
			m_polygon.add_conic_(x-r  , y-r+d);
			m_polygon.add_point_(x-r  , y-r  );
		}
		else
		{
			m_polygon.add_conic_(x    , y+d  );
			m_polygon.add_conic_(x+r-d, y+r  );
			m_polygon.add_point_(x+r  , y+r  ); 
		}
	}
}

void Rect::bottom_right_corner(Coord x, Coord y, Coord r, bool clockwise)
{
	Coord d = (r*27146)>>16;
	m_polygon.add_point_(x    ,y    );
	if (r != 0)
	{
		if (clockwise)
		{
			m_polygon.add_conic_(x    ,y+d  );
			m_polygon.add_conic_(x-r+d,y+r  );
			m_polygon.add_point_(x-r  ,y+r  );
		}
		else
		{
			m_polygon.add_conic_(x+d   ,y    );
			m_polygon.add_conic_(x+r   ,y-r+d);
			m_polygon.add_point_(x+r   ,y-r  );
		}
	}
}

void Rect::draw_rect(Coord x, Coord y, Coord width, Coord height, bool clockwise)
{
	m_polygon.add_point_(x        , y);
	if (clockwise)
	{
		m_polygon.add_point_(x + width, y);
		m_polygon.add_point_(x + width, y + height);
		m_polygon.add_point_(x        , y + height);
	}
	else
	{
		m_polygon.add_point_(x        , y + height);
		m_polygon.add_point_(x + width, y + height);
		m_polygon.add_point_(x + width, y);
	}
	m_polygon.next_contour();
}

void Rect::create_part()
{
	Coord w = m_size.width_();
	Coord h = m_size.height_();
	Coord r = m_radius;
	Coord t = m_thickness;

	if(r > (Coord)m_size.width_()/2)
	{
		r = m_size.width_()/2;
	}
	if(r > (Coord)m_size.height_()/2)
	{
		r = m_size.height_()/2;
	}
	if(t > (Coord)m_size.width_())
	{
		t = m_size.width_();
	}
	if(t > (Coord)m_size.height_())
	{
		t = m_size.height_();
	}

	Coord R = r + t;
	t = t>>1;

	if (r == 0)
	{
		R = 0;
	}

	m_polygon.clear();
	if (m_radius == 0)// && m_thickness == 0)
	{
		switch(sides())
		{
		case CommonStyle::NO_SIDE               : 
			break;

		// One side
		case CommonStyle::TOP_SIDE              : 
			draw_rect(-t, -t, w+t+t, t+t, true);
			break;

		case CommonStyle::RIGHT_SIDE            : 
			draw_rect(w-t, -t, t+t, t+t+h, true);
			break;

		case CommonStyle::BOTTOM_SIDE           : 
			draw_rect(-t, h-t, w+t+t, t+t, true);
			break;

		case CommonStyle::LEFT_SIDE             : 
			draw_rect(-t, -t, t+t, t+t+h, true);
			break;

		// Two sides parallel
		case CommonStyle::TOP_BOTTOM_SIDE       : 
			draw_rect(-t,  -t, w+t+t, t+t, true);
			draw_rect(-t, h-t, w+t+t, t+t, true);
			break;

		case CommonStyle::LEFT_RIGHT_SIDE       : 
			draw_rect(w-t, -t, t+t, t+t+h, true);
			draw_rect(-t,  -t, t+t, t+t+h, true);
			break;

		default:
			draw_rect(-t, -t, w+t+t, h+t+t, true);
			if (m_thickness > 0)
			{
				draw_rect(t, t, w-t-t, h-t-t, false);
			}
			break;
		}
	}
	else
	{
		// Fill completly
		if (m_thickness == 0)
		{
			top_right_corner   (w+t-R  ,-t     ,R, true);
			bottom_right_corner(w+t    ,h+t-R  ,R, true);
			bottom_left_corner (-t+R   ,h+t    ,R, true);
			top_left_corner    (-t     ,R-t    ,R, true);
			m_polygon.next_contour();
		}
		// Empty rectangle
		else
		{
			switch(sides())
			{
			case CommonStyle::NO_SIDE               : 
				break;

			// One side
			case CommonStyle::TOP_SIDE              : 
				draw_rect(-t, -t, w+t+t, t+t, true);
				break;

			case CommonStyle::RIGHT_SIDE            : 
				draw_rect(w-t, -t, t+t, t+t+h, true);
				break;

			case CommonStyle::BOTTOM_SIDE           : 
				draw_rect(-t, h-t, w+t+t, t+t, true);
				break;

			case CommonStyle::LEFT_SIDE             : 
				draw_rect(-t, -t, t+t, t+t+h, true);
				break;

			// Two sides parallel
			case CommonStyle::TOP_BOTTOM_SIDE       : 
				draw_rect(-t,  -t, w+t+t, t+t, true);
				draw_rect(-t, h-t, w+t+t, t+t, true);
				break;

			case CommonStyle::LEFT_RIGHT_SIDE       : 
				draw_rect(w-t, -t, t+t, t+t+h, true);
				draw_rect(-t,  -t, t+t, t+t+h, true);
				break;

			// Two sides corner
			case CommonStyle::TOP_RIGHT_SIDE        : 
				m_polygon.add_point_(-t    ,-t   );
				top_right_corner    (w+t-R ,-t   ,R, true);
				m_polygon.add_point_(w+t   ,h+t  );
				m_polygon.add_point_(w-t   ,h+t  );
				top_right_corner    (w-t   ,R-t  ,r, false);
				m_polygon.add_point_(-t    ,t    );
				m_polygon.next_contour();
				break;

			case CommonStyle::TOP_LEFT_SIDE         : 
				m_polygon.add_point_(-t    ,h+t  );
				top_left_corner     (-t    ,R-t  ,R, true);
				m_polygon.add_point_(w+t   ,-t   );
				m_polygon.add_point_(w+t   ,t    );
				top_left_corner     (R-t   ,t    ,r, false);
				m_polygon.add_point_(t     ,h+t  );
				m_polygon.next_contour();
				break;

			case CommonStyle::BOTTOM_RIGHT_SIDE     : 
				m_polygon.add_point_(w+t   ,-t    );
				bottom_right_corner (w+t   ,h+t-R ,R, true);
				m_polygon.add_point_(-t    ,h+t   );
				m_polygon.add_point_(-t    ,h-t   );
				bottom_right_corner (w+t-R ,h-t   ,r, false);
				m_polygon.add_point_(w-t   ,-t    );
				m_polygon.next_contour();
				break;

			case CommonStyle::BOTTOM_LEFT_SIDE      :
				m_polygon.add_point_(w+t  ,h+t    );
				bottom_left_corner  (-t+R ,h+t    ,R, true);
				m_polygon.add_point_(-t   ,-t     );
				m_polygon.add_point_(t    ,-t     );
				bottom_left_corner  (t    ,h+t-R  ,r, false);
				m_polygon.add_point_(w+t  ,h-t    );
				m_polygon.next_contour();
				break;
		
			// Three sides U
			case CommonStyle::TOP_RIGHT_BOTTOM_SIDE : 
				m_polygon.add_point_(-t   ,-t     );
				top_right_corner    (w+t-R,-t     ,R, true);
				bottom_right_corner (w+t  ,h+t-R  ,R, true);
				m_polygon.add_point_(-t   ,h+t    );

				m_polygon.add_point_(-t   ,h-t    );
				bottom_right_corner (w+t-R,h-t    ,r, false);
				top_right_corner    (w-t  ,R-t    ,r, false);
				m_polygon.add_point_(-t   ,t      );
				m_polygon.next_contour();
				break;

			case CommonStyle::RIGHT_BOTTOM_LEFT_SIDE: 
				m_polygon.add_point_(w+t  ,-t     );
				bottom_right_corner (w+t  ,h+t-R  ,R, true);
				bottom_left_corner  (-t+R ,h+t    ,R, true);
				m_polygon.add_point_(-t   ,-t     );

				m_polygon.add_point_(t    ,-t     );
				bottom_left_corner  (t    ,h+t-R  ,r, false);
				bottom_right_corner (w+t-R,h-t    ,r, false);
				m_polygon.add_point_(w-t  ,-t     );
				m_polygon.next_contour();
				break;

			case CommonStyle::BOTTOM_LEFT_TOP_SIDE  : 
				m_polygon.add_point_(w+t  ,h+t    );
				bottom_left_corner  (-t+R ,h+t    ,R, true);
				top_left_corner     (-t   ,R-t    ,R, true);
				m_polygon.add_point_(w+t  ,-t     );
				m_polygon.add_point_(w+t  , t     );
				top_left_corner     (R-t  ,t      ,r, false);
				bottom_left_corner  (t    ,h+t-R  ,r, false);
				m_polygon.add_point_(w+t  ,h-t    );
				m_polygon.next_contour();
				break;

			case CommonStyle::LEFT_TOP_RIGHT_SIDE   : 
				m_polygon.add_point_(-t   ,h+t    );
				top_left_corner     (-t   ,R-t    ,R, true);
				top_right_corner    (w+t-R,-t     ,R, true);
				m_polygon.add_point_(w+t  ,h+t    );
				m_polygon.add_point_(w-t  , h+t   );
				top_right_corner    (w-t  ,R-t    ,r, false);
				top_left_corner     (R-t  ,t      ,r, false);
				m_polygon.add_point_(t    ,h+t    );
				break;

			// All sides
			case CommonStyle::ALL_SIDES             : 
				top_right_corner   (w+t-R ,-t     ,R, true);
				bottom_right_corner(w+t   ,h+t-R  ,R, true);
				bottom_left_corner (-t+R  ,h+t    ,R, true);
				top_left_corner    (-t    ,R-t    ,R, true);
				m_polygon.next_contour();
				top_left_corner    (R-t   ,t      ,r, false);
				bottom_left_corner (t     ,h+t-R  ,r, false);
				bottom_right_corner(w+t-R ,h-t    ,r, false);
				top_right_corner   (w-t   ,R-t    ,r, false);
				break;
			}
		}
	}
}


#ifdef _DEBUG
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

	UIManager::desktop()->dispatch("test/out/rect1.svg");
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
	UIManager::desktop()->dispatch("test/out/rect2.svg");
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
 
	UIManager::desktop()->dispatch("test/out/rect3.svg");
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
		rect->radius(40);

		rect = new Rect(canvas);
		rect->position(5, 5);
		rect->size(470, 470);
		rect->thickness(10);
		rect->radius(40);
		rect->color(Color::BLUE,32);

	UIManager::desktop()->dispatch("test/out/rect4.svg");
}

void Rect::test5()
{
	Window window;
	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::WHITE);
		canvas->extend(Extend::EXTEND_ALL);

		int row = 0;
		int column = 0;
		for (int side = Rect::NO_SIDE; side <= Rect::ALL_SIDES; side++)
		{
			Rect * rect = new Rect(canvas);
				rect->position(15+column*120, 15+row*120);
				rect->size(90, 90);
				rect->color(Color::RED,64);
				rect->thickness(20);
				rect->radius(20);

			rect = new Rect(*rect);
				rect->sides(side);
				rect->color(Color::BLUE,64);

				column ++;
				if (column >= 4)
				{
					row ++;
					column = 0;
				}
		}
	//UIManager::desktop()->mainloop();
	UIManager::desktop()->dispatch("test/out/rect5.svg");
}

void Rect::test6()
{
}

void Rect::test()
{
	test5();
	test6();
	test4();
	test3();
	test2();
	test1();
}

#endif
