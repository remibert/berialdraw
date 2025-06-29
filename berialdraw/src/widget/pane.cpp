#include "berialdraw_imp.hpp"

using namespace berialdraw;

Pane::Pane(Widget * parent):
	Widget("pane", parent, sizeof(Pane))
{
	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);
	UIManager::styles()->apply(m_classname, (BorderStyle*)this);
}

Pane::~Pane()
{
}

/** Copy all styles of the pane */
void Pane::copy(const Pane & pane)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&pane);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&pane);
	*((BorderStyle*)this)   = *(BorderStyle*)(&pane);
}


/** Return the size of content without marges */
Size Pane::content_size()
{
	return Widget::children_size();
}

void Pane::place(const Area & area, bool in_layout)
{
	// If size not defined
	if (m_size.is_width_undefined() && m_size.is_height_undefined() && m_extend != Extend::EXTEND_NONE && 
		m_position.is_x_undefined() && m_position.is_y_undefined())
	{
		// Consider the placement in layout
		in_layout = true;
	}

	// Place the rectangle in area
	place_in_area(area, in_layout);

	// Save backclip
	Area backclip(m_backclip);

	// Place all child in area
	Widget::place(m_foreclip,true);

	// Restore backclip
	m_backclip = m_foreclip;

	// Add border for the backclip
	m_backclip.position().move_(0-(m_thickness),0-(m_thickness));
	m_backclip.size().increase_(m_thickness+m_thickness,m_thickness+m_thickness);
}

void Pane::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);
		Rect::build_polygon(m_foreclip, m_radius, m_thickness, 0, m_sides, stated_color(m_color), stated_color(m_border_color));
		Widget::paint(region);
	}
}

/** Serialize the content of widget into json */
void Pane::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	BorderStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Pane::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	BorderStyle::unserialize(it);
}


#ifdef _DEBUG
void Pane::test1()
{
	Window window;

	window.position(64,32);
	window.size(1+58,3+58);
	window.color(Color::RED_BLOOD,32);

	Grid * grid = new Grid(&window);
	
	Pane *
	pane = new Pane(grid);
		pane->color(Color::ICEBERG, 128);
		pane->border_color(Color::CARAMEL, 128);
		pane->radius(5);
		pane->thickness(5);
		pane->cell(0,0);
		pane->extend(EXTEND_ALL);
		pane->margin(10,20,30,40);

	int id = 0;
	for(int i = 0; i < 40; i += 1)
	{
		window.size(window.size().width()+1, window.size().height()+1);
		
		if (i % 3 == 0)
		{
			String name;
			name.print("test/out/pane1_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}
}

void Pane::test2()
{
	Window window;
		window.position(40,50);
		window.size(256,256);
		window.color(Color::WHITE_BLUE);

	Pane * pane = new Pane(&window);
		pane->position(64,64);
		pane->radius(5);
		pane->color(Color::DODGER_BLUE);

	Label * label = new Label(&window);
		label->position(64,64);
		label->text("NORMAL");
		label->margin(3,3,3,3);
		label->text_color(Color::WHITE);
		pane->size(label->marged_size());

	pane = new Pane(&window);
		pane->position(128,128);
		pane->radius(5);
		pane->thickness(1);
		pane->color(Color::SILVER_WHITE);
		pane->border_color(Color::DODGER_BLUE);
		pane->color();

	label = new Label(&window);
		label->position(128,128);
		label->text("PRESSED");
		label->margin(3,3,3,3);
		label->text_color(Color::DODGER_BLUE);
		pane->size(label->marged_size());

	UIManager::desktop()->dispatch("test/out/pane2.svg");
}

void Pane::test3()
{
	Window window;

	window.position(16,32);
	window.size(1,1);
	window.color(Color::RED_BLOOD,32);

	Grid * grid = new Grid(&window);
	
	Pane *
	pane1 = new Pane(grid);
		pane1->color(Color::ICEBERG, 128);
		pane1->border_color(Color::CARAMEL, 128);
		pane1->radius(5);
		pane1->thickness(5);
		pane1->cell(0,0);
		pane1->extend(EXTEND_ALL);
		pane1->margin(3,3,3,3);

	Label * 
	label1 = new Label(pane1);
		label1->text_color(Color::NIGHT_BLUE);
		label1->text("Helgo");
		label1->cell(0,0);
		label1->margin(5,5,5,5);
		pane1->size(label1->marged_size());

	Pane *
	pane2 = new Pane(grid);
		pane2->color(Color::ICEBERG, 128);
		pane2->border_color(Color::CARAMEL, 128);
		pane2->radius(5);
		pane2->thickness(5);
		pane2->cell(0,1);
		pane2->extend(EXTEND_ALL);
		pane2->margin(3,3,3,3);

	Label * 
	label2 = new Label(pane2);
		label2->text_color(Color::NIGHT_BLUE);
		label2->text("Bonjour");
		label2->cell(0,1);
		label2->margin(5,5,5,5);
		pane2->size(label2->marged_size());

	int id = 0;
	for(int i = 0; i < 50; i += 3)
	{
		window.size(window.size().width()+11, window.size().height()+7);
		switch(i %4)
		{
		case 0: 
			pane1->extend(EXTEND_NONE);
			pane2->extend(EXTEND_NONE);
			break;
		case 1: 
			pane1->extend(EXTEND_WIDTH);
			pane2->extend(EXTEND_WIDTH);
			break;
		case 2: 
			pane1->extend(EXTEND_HEIGHT);
			pane2->extend(EXTEND_HEIGHT);
			break;
		case 3: 
			pane1->extend(EXTEND_ALL);
			pane2->extend(EXTEND_ALL);
			break;
		}
	
		if ((i >= 0 && i <=15)||(i>=39 && i<=48))
		{
			String name;
			name.print("test/out/pane3_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}
}

void Pane::test4()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Grid * grid = new Grid(&window);
	Pane * pane;
	Label * label;
	Dim thickness = 1;
	Dim radius = 5;
	
	for (Dim row = 0; row < 5; row ++)
	{
		for (Dim column = 0; column < 5; column++)
		{
			pane = new Pane(grid);
				pane->radius(radius);
				pane->thickness(thickness);
				pane->cell(row,column);
				pane->extend(EXTEND_ALL);
				pane->margin(2+((thickness+1)/2));
				pane->border_color(Color::PURPLE_MIMOSA, 128);
				pane->color(Color::VIOLET, 128);
				pane->id(thickness);
			label = new Label(grid);
				label->cell(row,column);
				label->text("thick\nness\n%d",thickness);
				label->font_size(15);
				label->text_align(CENTER);
				label->text_color(Color::CLEMATIS_VIOLET);
			thickness++;
			if (radius < 15) radius++;
		}
	}
	UIManager::desktop()->dispatch("test/out/pane4_1.svg");
}

void Pane::test5()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Pane * pane = new Pane(&window);
		pane->radius(10);
		pane->border_color(Color::PURPLE_MIMOSA, 128);
		pane->thickness(3);
		pane->color(Color::PASTEL_RED);
		pane->position(13,17);
		pane->extend(Extend::EXTEND_NONE);
	Label * label = new Label(pane);
		label->text("helloworld");
		label->text_color(Color::PASTEL_YELLOW);
		label->margin(20);


	pane = new Pane(&window);
		pane->radius(10);
		pane->border_color(Color::PURPLE_MIMOSA, 128);
		pane->thickness(3);
		pane->color(Color::PASTEL_BLUE);
		pane->position(10,117);
		pane->extend(Extend::EXTEND_NONE);

	Column * column = new Column(pane);
		
		label = new Label(column);
			label->text("hello");
			label->text_color(Color::BLUE);
			label->margin(3);

		label = new Label(column);
			label->text("word");
			label->text_color(Color::BLUE);
			label->margin(3);

	pane = new Pane(&window);
		pane->radius(10);
		pane->border_color(Color::PURPLE_MIMOSA, 128);
		pane->thickness(3);
		pane->color(Color::PASTEL_BLUE);
		pane->position(10,217);
		pane->extend(Extend::EXTEND_NONE);

	Row * row = new Row(pane);
		
		label = new Label(row);
			label->text("hello");
			label->text_align(Align::ALIGN_TOP_RIGHT);
			label->text_color(Color::BLUE);
			label->margin(2);

		label = new Label(row);
			label->text("word");
			label->text_align(Align::ALIGN_BOTTOM_LEFT);
			label->text_color(Color::BLUE);
			label->margin(10);

	pane = new Pane(&window);
		pane->radius(10);
		pane->border_color(Color::PURPLE_MIMOSA, 128);
		pane->thickness(3);
		pane->color(Color::PASTEL_BLUE);
		pane->position(10,317);
		pane->size(200,100);
		pane->extend(Extend::EXTEND_NONE);

	row = new Row(pane);
		
		label = new Label(row);
			label->text("hello");
			label->text_align(Align::ALIGN_TOP_RIGHT);
			label->text_color(Color::BLUE);

		label = new Label(row);
			label->text("word");
			label->text_align(Align::ALIGN_BOTTOM_LEFT);
			label->text_color(Color::BLUE);
		
	// UIManager::desktop()->mainloop();
	UIManager::desktop()->dispatch("test/out/pane5_1.svg");
}

void Pane::test6()
{
}

void Pane::test7()
{
}

void Pane::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		test7();
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
#endif
