#include "berialdraw_imp.hpp"

using namespace berialdraw;

Grid::Grid(Widget * parent):
	Widget("grid", parent, sizeof(Grid))
{
	m_color = Color::TRANSPARENT;
}

Grid::~Grid()
{
}

void Grid::place(const Area & area, bool in_layout)
{
	if (UIManager::invalidator()->is_dirty(this))
	{
		Area marged_area(area);
		marged_area.decrease(margin());
		m_cells.place(this, marged_area);
		UIManager::invalidator()->undirty(this,Invalidator::GEOMETRY);
	}
}

Size Grid::content_size()
{
	m_cells.rebound(this);
	Size result = m_cells.calc_sizes(this);
	result.increase(margin());
	return result;
}

/** Get the widget hovered */
Widget * Grid::hovered(const Region & parent_region, const Point & position)
{
	Widget * result = 0;
	Widget* child = m_children;

	// Scan all widget children
	while (child && result == 0)
	{
		result = child->hovered(parent_region, position);
		child = child->next();
	}
	return result;
}

/** Serialize the content of widget into json */
void Grid::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	WidgetStyle::serialize(it);
	CommonStyle::serialize(it);
	Widget::serialize(it);
}

/** Unserialize the content of widget from json */
void Grid::unserialize(JsonIterator & it)
{
	WidgetStyle::unserialize(it);
	CommonStyle::unserialize(it);
}

#ifdef _DEBUG
void Grid::test1()
{
	Label * label;
	Pane * rect;
	uint8_t alpha = 128;
	Window window;
		window.color(Color::WHITE);

	Grid * grid = new Grid(&window);

	rect = new Pane(grid);
		rect->cell(0,0);
		rect->color(Color::RED, alpha);
		rect->id(123);
		rect->extend();

	label = new Label(grid);
		label->text("0^0");
		label->cell(0,0);
		label->id(0);
		label->align(ALIGN_TOP);

	label = new Label(grid);
		label->text("0v0");
		label->cell(0,0);
		label->id(0);
		label->align(ALIGN_BOTTOM);

	label = new Label(grid);
		label->text("0<0");
		label->cell(0,0);
		label->id(0);
		label->align(ALIGN_LEFT);

	label = new Label(grid);
		label->text("0>0");
		label->cell(0,0);
		label->id(0);
		label->align(ALIGN_RIGHT);

	rect = new Pane(grid);
		rect->cell(0,1);
		rect->color(Color::GREEN, alpha);
		rect->extend();
		rect->id(1);

	label = new Label(grid);
		label->text("0,1");
		label->cell(0,1);
		label->min_size(32,0);
		label->id(1);

	rect = new Pane(grid);
		rect->cell(0,2);
		rect->color(Color::BLUE, alpha);
		rect->extend();

	label = new Label(grid);
		label->text("0,2");
		label->cell(0,2);
		label->min_size(16,0);
		label->id(2);

	rect = new Pane(grid);
		rect->cell(0,3);
		rect->color(Color::MAUVE, alpha);
		rect->extend();

	label = new Label(grid);
		label->text("0,3");
		label->cell(0,3);
		label->min_size(8,0);
		label->id(3);

	rect = new Pane(grid);
		rect->cell(1,0);
		rect->color(Color::VIOLET, alpha);
		rect->extend();

	label = new Label(grid);
		label->text("1,0");
		label->cell(1,0);

	rect = new Pane(grid);
		rect->cell(1,1);
		rect->color(Color::CYAN, alpha);
		rect->extend();

	label = new Label(grid);
		label->text("1,1");
		label->cell(1,1);

	rect = new Pane(grid);
		rect->cell(1,2);
		rect->color(Color::ORANGE, alpha);
		rect->extend();

	label = new Label(grid);
		label->text("1,2");
		label->cell(1,2);

	rect = new Pane(grid);
		rect->cell(1,3);
		rect->color(Color::RED, alpha);
		rect->extend();

	label = new Label(grid);
		label->text("1,3");
		label->cell(1,3);

	int id = 0;
	for(int i = 0; i <= 480; i += 60)
	{
		window.size(i+1, i+1);
		window.position(0,0);
		if (i % 30 == 0)
		{
			String name;
			name.print("${ui.tests}/out/grid1_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
			window.size(i+1, i+2);
			window.size(i+1, i+1);
			//UIManager::desktop()->dispatch();
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}
}

void Grid::test2()
{
	uint8_t alpha = 128;
	Window window;
		window.color(Color::WHITE);
		window.position(10,100);
		window.size(200,100);

	Grid * grid = new Grid(&window);
	Label * label;
	Pane * pane = new Pane(grid);
		pane->cell(0,0);
		pane->id(0);
		pane->min_size(100,0);
		pane->color(Color::RED, 128);

	label = new Label(grid);
		label->cell(0,0);
		label->text("0,0");

	pane = new Pane(grid);
		pane->cell(0,1);
		pane->min_size(32,0);
		pane->id(1);
		pane->color(Color::GREEN, 128);

	label = new Label(grid);
		label->cell(0,1);
		label->text("0,1");

	pane = new Pane(grid);
		pane->cell(0,2);
		pane->min_size(16,0);
		pane->id(2);
		pane->color(Color::BLUE, 128);

	label = new Label(grid);
		label->cell(0,2);
		label->text("0,2");

	pane = new Pane(grid);
		pane->cell(0,3);
		pane->min_size(8,0);
		pane->id(3);
		pane->color(Color::MAUVE, 128);

	label = new Label(grid);
		label->cell(0,3);
		label->text("0,3");

	pane = new Pane(grid);
		pane->cell(1,0);
		pane->color(Color::VIOLET, 128);

	label = new Label(grid);
		label->cell(1,0);
		label->text("1,0");

	pane = new Pane(grid);
		pane->cell(1,1);
		pane->color(Color::CYAN, 128);

	label = new Label(grid);
		label->cell(1,1);
		label->text("1,1");

	pane = new Pane(grid);
		pane->cell(1,2);
		pane->color(Color::ORANGE, 128);

	label = new Label(grid);
		label->cell(1,2);
		label->text("1,2");

	pane = new Pane(grid);
		pane->cell(1,3);
		pane->color(Color::RED, 128);

	label = new Label(grid);
		label->cell(1,3);
		label->text("1,3");

	int id=0;
	int j = 0;
	for(int i = 0; i < 470; i += 30)
	{
		window.size(i, window.size().height());
		j ++;
		if (j%2 == 0)
		{
			String name;
			name.print("${ui.tests}/out/grid2_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}
}

void Grid::test3()
{
	uint8_t alpha = 128;
	Window window;
		window.color(Color::WHITE);
		window.position(10,100);
		window.size(200,100);

	Grid * grid = new Grid(&window);
	Label * label;
	Pane * pane = new Pane(grid);
		pane->cell(0,0);
		pane->id(0);
		pane->max_size(128,Size::MAX_SIZE);
		pane->color(Color::RED, 128);

		label = new Label(grid);
			label->cell(0,0);
			label->text("0");

	pane = new Pane(grid);
		pane->cell(0,1);
		pane->max_size(32,Size::MAX_SIZE);
		pane->id(1);
		pane->color(Color::GREEN, 128);

		label = new Label(grid);
			label->cell(0,1);
			label->text("1");

	pane = new Pane(grid);
		pane->cell(0,2);
		pane->max_size(64,Size::MAX_SIZE);
		pane->id(2);
		pane->color(Color::BLUE, 128);

		label = new Label(grid);
			label->cell(0,2);
			label->text("2");

	pane = new Pane(grid);
		pane->cell(0,3);
		pane->max_size(16,Size::MAX_SIZE);
		pane->id(3);
		pane->color(Color::MAUVE, 128);

		label = new Label(grid);
			label->cell(0,3);
			label->text("3");
			label->id(33);

	pane = new Pane(grid);
		pane->cell(1,0);
		pane->color(Color::VIOLET, 128);

		label = new Label(grid);
			label->cell(1,0);
			label->text("A");

	pane = new Pane(grid);
		pane->cell(1,1);
		pane->color(Color::CYAN, 128);

		label = new Label(grid);
			label->cell(1,1);
			label->text("B");

	pane = new Pane(grid);
		pane->cell(1,2);
		pane->color(Color::ORANGE, 128);

		label = new Label(grid);
			label->cell(1,2);
			label->text("C");

	pane = new Pane(grid);
		pane->cell(1,3);
		pane->color(Color::RED, 128);

		label = new Label(grid);
			label->cell(1,3);
			label->text("D");

	int i = 9;
	int id = 0;
	int j = 0;
	for(; i < 470; i += 30)
	{
		window.size(i, window.size().height());
		j ++;
		if (j%2 == 0)
		{
			String name;
			name.print("${ui.tests}/out/grid3_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}
}

void Grid::test4()
{
	uint8_t alpha = 128;
	Window window;
		window.color(Color::WHITE);
		window.position(0,0);
		window.size(480,480);

	Grid * grid = new Grid(&window);
	Button * button0 = new Button(grid);
		button0->cell(0,0);
		button0->id(0);
		button0->color(Color::RED, 128);
		button0->text("0");

	Button * button1 = new Button(grid);
		button1->cell(0,1);
		button1->id(1);
		button1->color(Color::GREEN, 128);
		button1->text("1");
		button1->size_policy(SizePolicy::ENLARGE_WIDTH);

	Button * button2 = new Button(grid);
		button2->cell(0,2);
		button2->id(2);
		button2->color(Color::BLUE, 128);
		button2->text("2");

	Button * button3 = new Button(grid);
		button3->cell(0,3);
		button3->id(3);
		button3->color(Color::MAUVE, 128);
		button3->text("3");
		button3->id(33);

	Button * button_a = new Button(grid);
		button_a->cell(1,0);
		button_a->color(Color::VIOLET, 128);
		button_a->text("A");

	Button * button_b = new Button(grid);
		button_b->cell(1,1);
		button_b->color(Color::CYAN, 128);
		button_b->text("B");

	Button * button_c = new Button(grid);
		button_c->cell(1,2);
		button_c->color(Color::ORANGE, 128);
		button_c->text("C");
		button_c->size_policy(SizePolicy::ENLARGE_HEIGHT);

	Button * button_d = new Button(grid);
		button_d->cell(1,3);
		button_d->color(Color::RED, 128);
		button_d->text("D");
	

	int i;
	int id = 0;
	int j;
	int increment;

	button1->size_policy(SizePolicy::ENLARGE_WIDTH);
	button_c->size_policy(SizePolicy::ENLARGE_HEIGHT);
	for(i = 20, increment = 3, j = 0; i <= 480; i += increment)
	{
		window.size(i, i);
		j ++;
		increment += 3;
		
		if (j%2 == 0)
		{
			String name;
			name.print("${ui.tests}/out/grid4_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}

	button1->size_policy(SizePolicy::ENLARGE_ALL);
	button_c->size_policy(SizePolicy::ENLARGE_ALL);
	for(i = 20, increment = 3, j = 0; i <= 480; i += increment)
	{
		window.size(i, i);
		j ++;
		increment += 3;
		
		if (j%2 == 0)
		{
			String name;
			name.print("${ui.tests}/out/grid4_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}
}


void Grid::test5()
{
	uint8_t alpha = 128;
	Window window;
		window.color(Color::WHITE);
		window.position(0,0);
		window.size(480,480);

	Grid * grid = new Grid(&window);
	Button * button0 = new Button(grid);
		button0->cell(0,0);
		button0->id(0);
		button0->color(Color::RED, 128);
		button0->text("0");

	Button * button1 = new Button(grid);
		button1->cell(0,1);
		button1->id(1);
		button1->color(Color::GREEN, 128);
		button1->text("1");

	Button * button2 = new Button(grid);
		button2->cell(0,2);
		button2->id(2);
		button2->color(Color::BLUE, 128);
		button2->text("2");

	Button * button3 = new Button(grid);
		button3->cell(0,3);
		button3->id(3);
		button3->color(Color::MAUVE, 128);
		button3->text("3");
		button3->id(33);

	Button * button_a = new Button(grid);
		button_a->cell(1,0);
		button_a->color(Color::VIOLET, 128);
		button_a->text("A");

	Button * button_b = new Button(grid);
		button_b->cell(1,1);
		button_b->color(Color::CYAN, 128);
		button_b->text("B");

	Button * button_c = new Button(grid);
		button_c->cell(1,2);
		button_c->color(Color::ORANGE, 128);
		button_c->text("C");

	Button * button_d = new Button(grid);
		button_d->cell(1,3);
		button_d->color(Color::RED, 128);
		button_d->text("D");
	

	int i;
	int id = 0;
	int j;
	int increment;


	button1->size_policy(SizePolicy::SHRINK_WIDTH);
	button_c->size_policy(SizePolicy::SHRINK_HEIGHT);
	for(i = 20, increment = 3, j = 0; i <= 480; i += increment)
	{
		window.size(i, i);
		j ++;
		increment += 3;
		
		if (j%2 == 0)
		{
			String name;
			name.print("${ui.tests}/out/grid5_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}

	button1->size_policy(SizePolicy::SHRINK_ALL);
	button_c->size_policy(SizePolicy::NORMAL_SIZE);
	for(i = 20, increment = 3, j = 0; i <= 480; i += increment)
	{
		window.size(i, i);
		j ++;
		increment += 3;
		
		if (j%2 == 0)
		{
			String name;
			name.print("${ui.tests}/out/grid5_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}
}

void Grid::test6()
{
}

void Grid::test()
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
