#include "berialdraw_imp.hpp"

using namespace berialdraw;

Canvas::Canvas(Widget * parent):
	Widget("canvas",parent, sizeof(Canvas))
{
	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);
}

Canvas::~Canvas()
{
	clear();
}

/** Copy all styles of the canvas */
void Canvas::copy(const Canvas & canvas)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&canvas);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&canvas);
}

/** Copy all styles of the canvas */
void Canvas::copy(const Canvas * canvas)
{
	if(canvas)
	{
		copy(*canvas);
	}
}

/** Return the size of content without marges */
Size Canvas::content_size()
{
	if ((m_content_size.is_width_undefined() && m_content_size.is_height_undefined()) || m_geometry_modified)
	{
		m_geometry_modified = 0;
		if ((m_size.is_height_undefined() && m_size.is_width_undefined()))
		{
			Size size;

			// Compute the content size according to all shapes registered
			for(uint32_t i = 0; i < m_shapes.size(); i++)
			{
				if (m_shapes[i])
				{
					Size marged_size = m_shapes[i]->marged_size();
					Point position = m_shapes[i]->position();
					if (marged_size.width_() + position.x_() > size.width_())
					{
						size.width_(marged_size.width_() + position.x_());
					}
					if (marged_size.height_() + position.y_() > size.height_())
					{
						size.height_(marged_size.height_() + position.y_());
					}
				}
			}
			m_content_size = size;
		}
		else
		{
			m_content_size = m_size;
		}
	}
	return m_content_size;
}

void Canvas::place(const Area & area, bool in_layout)
{
	if (m_size.is_width_undefined() && m_size.is_height_undefined() && m_extend != Extend::EXTEND_NONE && 
		m_position.is_x_undefined() && m_position.is_y_undefined())
	{
		in_layout = true;
	}
	place_in_area(area, in_layout);
	Widget::place(area, in_layout);
}

void Canvas::paint(const Region & parent_region)
{
	Region region(parent_region);

	// Clipping canvas
	region.intersect(m_foreclip);

	// If widget visible
	if (region.is_inside(m_foreclip.position(), m_foreclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);

		Exporter * exporter = UIManager::exporter();
		if (exporter)
		{
			// Clipping canvas svg
			exporter->open_group(m_foreclip.position(), m_foreclip.size());
		}

		// Draw background color
		if(m_color)
		{
			Rect rect(0);
			rect.size(m_foreclip.size());
			rect.color(stated_color(m_color));
			rect.paint(m_foreclip.position());
		}

		// Redraw all shapes
		for(uint32_t i = 0; i < m_shapes.size(); i++)
		{
			if (m_shapes[i])
			{
				m_shapes[i]->paints(m_foreclip.position());
			}
		}

		if (exporter)
		{
			// Stop clipping canvas svg
			exporter->close_group();
		}
	}
}

/** Add shape into the canvas (all shapes added will be destroyed when the canvas destroy) */
void Canvas::add(Shape * shape, size_t shape_size)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	if(shape)
	{
		bool found = false;

		UIManager::invalidator()->add(this, shape, shape_size);

		m_content_size.clean();
		for(uint32_t i = 0; i < m_shapes.size(); i++)
		{
			if(m_shapes[i] == shape)
			{
				found = true;
				break;
			}
		}
		if(found == false)
		{
			m_shapes.push_back(shape);
		}
		else
		{
			found = true;
		}
	}
}

/** Remove shape from the canvas  */
void Canvas::remove(Shape * shape)
{
	if(shape)
	{
		m_content_size.clean();
		UIManager::invalidator()->remove(shape);
		uint32_t size = m_shapes.size();
		for(uint32_t i = 0; i < size; i++)
		{
			if(m_shapes[i] == shape)
			{
				m_shapes.remove(i);
				break;
			}
		}
	}
}

/** Clear all shape on canvas */
void Canvas::clear()
{
	m_content_size.clean();
	while (m_shapes.size() > 0)
	{
		UIManager::invalidator()->remove(m_shapes[0]);
		delete m_shapes[0];
	}
	m_shapes.clear();
}

/** Get the widget hovered */
Widget * Canvas::hovered(const Region & parent_region, const Point & position)
{
	Region region(parent_region);
	region.intersect(m_foreclip);

	// If the widget hovered
	if(region.is_inside(position) != Region::Overlap::OUT)
	{
		return this;
	}
	return 0;
}

/** Serialize the content of widget into json */
void Canvas::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Canvas::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
}


#ifdef _DEBUG
void Canvas::test1()
{
}

void Canvas::test2()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

		Grid * grid = new Grid(&window);
			Canvas * canvas = new Canvas(grid);
				canvas->cell(0,0);
				canvas->extend(Extend::EXTEND_ALL);
				//canvas->color(Color::WHITE);

				Text * text = new Text(canvas);
					text->text("hello\nworld\nI am\nHappy\nRegards");
					//text->text("hello\nhello\nhello\nhello\nhello\n");
					text->position(240,240);
					text->font_size(60,30);
					text->color(JADE,64);
					text->center(text->content_size().middle());
		

	int angle = 0;
	int id = 0;

	for (int angle = 0; angle <= 360; angle += 30)
	{
		text->angle(angle);
		String name;
		name.print("$(ui.tests)/out/canvas1_%d.svg", ++id);
		UIManager::desktop()->dispatch(name);
	}
}

void Canvas::test3()
{
	Window window;
		window.position(11,19);
		window.size(257,319);
		window.color(Color::BROWN,32);

	Canvas * canvas = new Canvas(&window);
		canvas->position(10,10);
		canvas->size(window.size().width()-20, window.size().height() -20);
		canvas->color(Color::AQUA,32);

		Polygon * arrow = new Polygon(canvas);
			arrow->color(Color::RED,30);
			arrow->position(30,30);
			arrow->angle(180);
					
			arrow->add_point(0,30);
			arrow->add_point(-30,0);
			arrow->add_point(-15,0);
			arrow->add_point(-15,-30);
			arrow->add_point(15,-30);
			arrow->add_point(15,0);
			arrow->add_point(30,0);
			arrow->next_contour();

		arrow = new Polygon(*arrow);
			arrow->position(arrow->position().x()+5,arrow->position().y()+5);

		Rect * rect = new Rect(canvas);
			rect->size(60,60);
			rect->position(60,3);
			rect->color(ORANGE,64);
			rect->radius(20);
			rect->thickness(0);
		rect = new Rect(*rect);
			rect->thickness(3);
			rect->color(ORANGE,128);

		Text * text = new Text(canvas);
			text->text("hello world");
			text->position(10,68);
			text->font_size(60,30);
			text->color(JADE,64);
		
		text = new Text(*text);
			text->position(text->position().x() + 4, text->position().y() + 4);
				
		Circle * circle = new Circle(canvas);
			circle->position(90+63,33);
			circle->radius(30);
			circle->color(PURPLE,64);
			circle->thickness(0);

		circle = new Circle(*circle);
			circle->thickness(3);
			circle->color(PURPLE,128);

		Triangle * triangle = new Triangle(canvas);
			triangle->position(90+63+63,40);
			triangle->radius(30);
			triangle->color(RED,64);
			triangle->thickness(0);

		triangle = new Triangle(*triangle);
			triangle->thickness(3);
			triangle->color(RED,128);

		Cross * cross = new Cross(canvas);
			cross->position(90+63+63+63,33);
			cross->radius(30);
			cross->color(GREEN,64);
			cross->thickness(3);

		Square * square = new Square(canvas);
			square->position(90+63+63+63+63,33);
			square->radius(30);
			square->color(BLUE,64);
			square->thickness(0);

		square = new Square(*square);
			square->thickness(3);
			square->color(BLUE,128);

		PolyLines * polylines = new PolyLines(canvas);
			polylines->position(33,126);
			polylines->append(30,15);
			polylines->append(30,-15);
			polylines->append( 15,-30);
			polylines->append(-15,-30);
			polylines->append(-30,-15);
			polylines->append(-30,15);
			polylines->append(-15,30);
			polylines->append(15,30);
			polylines->append(30,15);
			polylines->thickness(3);
			polylines->color(BLUE,64);

		polylines = new PolyLines(*polylines);
			polylines->position(polylines->position().x()+3,polylines->position().y()+3);

		Pie * pie = new Pie(canvas);
			pie->color(BLUE,64);
			pie->position(33+68, 126);
			pie->radius(30);
			pie->thickness(0);
			pie->rope(true);
			pie->start_angle(0);
			pie->end_angle(270);
		pie = new Pie(*pie);
			pie->thickness(3);
			pie->color(BLUE,128);

		Line * line = new Line(canvas);
			line->color(BLUE,64);
			line->position(63+63, 63+30);
			line->thickness(3);
			line->point1(0,0);
			line->point2(60,60);
		line = new Line(*line);
			line->position(line->position().x()+10,line->position().y());

	UIManager::desktop()->dispatch("$(ui.tests)/out/canvas3.svg");
}

void Canvas::test4()
{
	Window window;
		window.position(0,0);
		window.size(480,480);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());
		canvas->color(Color::WHITE);

	Coord x = 0;
	Coord y = 0;
	for (int i = 1; i <= 20; i++)
	{
		Polygon * polygon = new Polygon(canvas);
		polygon->position(x,y);
		x+= i*16;
		if (x > 480)
		{
			x = 0;
			y += 16*(i/2);
		}
		{
			VectorsScript vectors_script(*polygon);
				vectors_script.select(".843,932;.441,529;.441,529;^422,540;.412,558;.412,558;^402,575;.402,596;.402,596;^402,689;.458,758;.458,758;^514,828;.609,852;.609,852;^617,854;.621,862;.621,862;^625,869;.623,877;.623,877;^621,885;.614,889;.614,889;^607,893;.598,891;.598,891;^489,863;.425,783;.425,783;^361,703;.361,596;.361,596;^361,567;.374,543;.374,543;^388,518;.412,501;.412,501;.369,458;.369,458;^335,485;.317,520;.317,520;^299,555;.299,596;.299,596;^299,671;.324,729;.324,729;^350,786;.411,849;.411,849;^417,855;.417,864;.417,864;^417,872;.411,878;.411,878;^405,884;.396,884;.396,884;^388,884;.381,878;.381,878;^315,811;.287,747;.287,747;^258,682;.258,596;.258,596;^258,547;.281,503;.281,503;^303,459;.341,429;.341,429;.297,386;.297,386;^238,440;.216,486;.216,486;^194,531;.194,596;.194,596;^194,632;.200,669;.200,669;^207,707;.221,742;.221,742;^224,750;.221,757;.221,757;^218,764;.210,767;.210,767;^202,770;.194,767;.194,767;^187,764;.184,756;.184,756;^168,714;.161,674;.161,674;^154,635;.154,596;.154,596;^154,522;.180,467;.180,467;^206,412;.268,356;.268,356;.227,315;.227,315;^207,332;.190,350;.190,350;^174,368;.159,388;.159,388;^155,395;.147,396;.147,396;^139,398;.131,393;.131,393;^124,388;.123,380;.123,380;^122,372;.127,365;.127,365;^141,343;.159,324;.159,324;^178,305;.199,287;.199,287;.44,131;.44,131;.87,89;.87,89;.886,888;.886,888;.843,932;~;.696,616;^688,616;.682,610;.682,610;^676,605;.676,596;.676,596;^676,523;.625,474;.625,474;^573,424;.503,419;.503,419;.462,379;.462,379;^468,378;.475,378;.475,378;.487,378;.487,378;^581,378;.649,441;.649,441;^717,504;.717,596;.717,596;^717,605;.711,610;.711,610;^705,616;.696,616;.696,616;.696,616;~;.487,86;^552,86;.614,102;.614,102;^675,117;.733,146;.733,146;^742,152;.743,159;.743,159;^745,166;.742,173;.742,173;^739,180;.732,184;.732,184;^725,188;.715,183;.715,183;^661,156;.604,141;.604,141;^547,126;.487,126;.487,126;^429,126;.373,139;.373,139;^317,153;.265,182;.265,182;.236,153;.236,153;^294,120;.357,103;.357,103;^421,86;.487,86;~;.487,185;^594,185;.689,231;.689,231;^784,277;.848,363;.848,363;^855,373;.852,380;.852,380;^850,387;.844,392;.844,392;^838,397;.830,396;.830,396;^822,396;.816,388;.816,388;^760,309;.673,267;.673,267;^585,225;.487,225;.487,225;^448,225;.410,232;.410,232;^372,239;.337,253;.337,253;.307,223;.307,223;^349,204;.394,194;.394,194;^439,185;.487,185;~;.487,280;^624,280;.722,371;.722,371;^820,461;.820,596;.820,596;^820,625;.806,649;.806,649;^793,672;.772,689;.772,689;.744,660;.744,660;^760,649;.770,633;.770,633;^779,616;.779,596;.779,596;^779,479;.693,399;.693,399;^608,319;.488,319;.488,319;^468,319;.450,322;.450,322;^431,324;.413,329;.413,329;.380,297;.380,297;^406,289;.432,284;.432,284;^459,280;.487,280;.487,280;.487,280;~;.682,810;^592,810;.528,748;.528,748;^464,687;.464,597;.464,597;^464,589;.470,583;.470,583;^475,576;.484,576;.484,576;^493,576;.499,583;.499,583;^505,589;.505,597;.505,597;^505,669;.557,719;.557,719;^610,768;.682,768;.682,768;^688,768;.699,768;.699,768;^711,767;.723,765;.723,765;^732,763;.738,768;.738,768;^745,772;.747,781;.747,781;^749,789;.744,796;.744,796;^739,802;.731,804;.731,804;^713,809;.699,809;.699,809;^685,810;.682,810;.682,810;.682,810;~;.0,0;.0,30;.0,30;.0,1023;.0,1023;.1024,1023;.1024,1023;.1024,0;.1024,0;.0,0;~;.60,60;.964,60;.964,60;.964,963;.964,963;.60,963;.60,963;.60,60;~;");
				vectors_script.parse();
				polygon->color(Color::RED,16);
				polygon->zoom(i);
		}
	}
	UIManager::desktop()->dispatch("$(ui.tests)/out/canvas4.svg");
}

void Canvas::test5()
{
}

void Canvas::test()
{
	static bool done = false;
	if (done == false)
	{
test3();
		done = true;
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
#endif
