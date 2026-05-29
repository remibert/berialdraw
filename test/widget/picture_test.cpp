#include "berialdraw_imp.hpp"
using namespace berialdraw;

// Test 1: Display PNG image with default settings (aspect fit)
void Picture::test1()
{
	Window window;
		window.size(480, 480);
		window.color(Color::WHITE);

	Picture * picture = new Picture(&window);
		picture->filename("$(ui.images)/house.png");

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture1_0.svg");
}

// Test 2: Display JPEG image with default settings
void Picture::test2()
{
	Window window;
		window.size(480, 480);
		window.color(Color::WHITE);

	Picture * picture = new Picture(&window);
		picture->filename("$(ui.images)/filleperle.jpg");

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture2_0.svg");
}

// Test 3: Test all fit modes with PNG
void Picture::test3()
{
	Window window;
		window.size(480, 480);
		window.color(Color::LIGHT_GRAY);

	Grid * grid = new Grid(&window);

	// STRETCH
	Picture * p1 = new Picture(grid);
		p1->cell(0, 0);
		p1->filename("$(ui.images)/house.png");
		p1->fit_mode(IMAGE_FIT_STRETCH);
		p1->size(220, 220);
		p1->border_color(Color::RED);
		p1->thickness(1);

	// ASPECT_FIT
	Picture * p2 = new Picture(grid);
		p2->cell(1, 0);
		p2->filename("$(ui.images)/house.png");
		p2->fit_mode(IMAGE_FIT_ASPECT_FIT);
		p2->size(220, 220);
		p2->border_color(Color::GREEN);
		p2->thickness(1);

	// ASPECT_FILL
	Picture * p3 = new Picture(grid);
		p3->cell(0, 1);
		p3->filename("$(ui.images)/house.png");
		p3->fit_mode(IMAGE_FIT_ASPECT_FILL);
		p3->size(220, 220);
		p3->border_color(Color::BLUE);
		p3->thickness(1);

	// NONE (native size)
	Picture * p4 = new Picture(grid);
		p4->cell(1, 1);
		p4->filename("$(ui.images)/house.png");
		p4->fit_mode(IMAGE_FIT_NONE);
		p4->size(220, 220);
		p4->border_color(Color::ORANGE);
		p4->thickness(1);

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture3_0.svg");
}

// Test 4: Test all fit modes with JPEG
void Picture::test4()
{
	Window window;
		window.size(480, 480);
		window.color(Color::LIGHT_GRAY);

	Grid * grid = new Grid(&window);

	// ASPECT_WIDTH
	Picture * p1 = new Picture(grid);
		p1->cell(0, 0);
		p1->filename("$(ui.images)/filleperle.jpg");
		p1->fit_mode(IMAGE_FIT_ASPECT_WIDTH);
		p1->size(220, 220);
		p1->border_color(Color::RED);
		p1->thickness(1);

	// ASPECT_HEIGHT
	Picture * p2 = new Picture(grid);
		p2->cell(1, 0);
		p2->filename("$(ui.images)/filleperle.jpg");
		p2->fit_mode(IMAGE_FIT_ASPECT_HEIGHT);
		p2->size(220, 220);
		p2->border_color(Color::GREEN);
		p2->thickness(1);

	// STRETCH
	Picture * p3 = new Picture(grid);
		p3->cell(0, 1);
		p3->filename("$(ui.images)/filleperle.jpg");
		p3->fit_mode(IMAGE_FIT_STRETCH);
		p3->size(220, 220);
		p3->border_color(Color::BLUE);
		p3->thickness(1);

	// ASPECT_FIT
	Picture * p4 = new Picture(grid);
		p4->cell(1, 1);
		p4->filename("$(ui.images)/filleperle.jpg");
		p4->fit_mode(IMAGE_FIT_ASPECT_FIT);
		p4->size(220, 220);
		p4->border_color(Color::ORANGE);
		p4->thickness(1);

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture4_0.svg");
}

// Test 5: Alpha transparency
void Picture::test5()
{
	Window window;
		window.size(480, 480);
		window.color(Color::LIGHT_BLUE);

	// Fully opaque
	Picture * p1 = new Picture(&window);
		p1->position(10, 10);
		p1->size(200, 200);
		p1->filename("$(ui.images)/house.png");
		p1->alpha(255);
		p1->border_color(Color::BLACK);
		p1->thickness(1);

	// Semi-transparent
	Picture * p2 = new Picture(&window);
		p2->position(120, 120);
		p2->size(200, 200);
		p2->filename("$(ui.images)/filleperle.jpg");
		p2->alpha(128);
		p2->border_color(Color::BLACK);
		p2->thickness(1);

	// Mostly transparent
	Picture * p3 = new Picture(&window);
		p3->position(240, 240);
		p3->size(200, 200);
		p3->filename("$(ui.images)/house.png");
		p3->alpha(64);
		p3->border_color(Color::BLACK);
		p3->thickness(1);

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture5_0.svg");
}

// Test 6: Resize and position changes
void Picture::test6()
{
	Window window;
		window.size(480, 480);
		window.color(Color::WHITE);

	Picture * picture = new Picture(&window);
		picture->filename("$(ui.images)/filleperle.jpg");
		picture->border_color(Color::DARK_GRAY);
		picture->thickness(2);

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture6_0.svg");

		picture->position(50, 50);
		picture->size(200, 150);

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture6_1.svg");

		picture->size(400, 300);

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture6_2.svg");
}

// Test 7: Both PNG and JPEG side by side in a Row
void Picture::test7()
{
	Window window;
		window.size(480, 240);
		window.color(Color::WHITE);

	Row * row = new Row(&window);

	Picture * png = new Picture(row);
		png->filename("$(ui.images)/house.png");
		png->size(230, 220);
		png->margin(5);
		png->border_color(Color::GREEN);
		png->thickness(1);

	Picture * jpg = new Picture(row);
		jpg->filename("$(ui.images)/filleperle.jpg");
		jpg->size(230, 220);
		jpg->margin(5);
		jpg->border_color(Color::BLUE);
		jpg->thickness(1);

	UIManager::desktop()->dispatch("$(ui.tests)/out/picture7_0.svg");
}



void Picture::test8()
{
	Window * window = new Window();
		window->size(120, 800);
		window->color(Color::WHITE);

	ScrollView * scroll = new ScrollView(window);

	Column * layout = new Column(scroll);

	//Label * label = new Label(layout);
	//	label->text("hello");

	//Edit * first_name = new Edit(layout);
	//	first_name->text("");
	//	first_name->place_holder("Test first name");

	//Edit * last_name = new Edit(layout);
	//	last_name->text("");
	//	last_name->place_holder("Test last name");

	//Edit * age = new Edit(layout);
	//	age->text("Age");
	//	age->place_holder("");

	//Slider * slider = new Slider(layout);

	//Slider * sliderv = new Slider(layout);
	//	sliderv->extend(Extend::EXTEND_HEIGHT);
	//	sliderv->min_size(100, 100);

	//Button * button = new Button(layout);
	//	button->text("hello\nworld");
	//	button->margin(20, 10);

	Radio * radio1 = new Radio(layout);
		radio1->text("Radio1");
		radio1->group("group1");

	Radio * radio2 = new Radio(layout);
		radio2->text("Radio2");
		radio2->group("group1");
		radio2->checked(true);

	//Checkbox * checkbox = new Checkbox(layout);
	//	checkbox->text("Checkbox");
	//	checkbox->checked(true);

	//Switch * switch_widget = new Switch(layout);
	//	switch_widget->text("Switch");
	//	switch_widget->checked(true);

	//// Table View Example
	//Label * table_label = new Label(layout);
	//	table_label->text("Table View Example");

	TableView * table = new TableView(layout);
		table->extend(Extend::EXTEND_ALL);
		table->min_size(400, 150);
		
	// Load table data
	String table_data("["
		"['Name','First Name','Age','City'],"
		"['Dupont','Jean','28','Paris'],"
		"['Martin','Marie','35','Lyon'],"
		"['Bernard','Pierre','42','Marseille'],"
		"['Thomas','Sophie','31','Toulouse'],"
		"['Robert','Luc','55','Nice']"
	"]");
	table->load(table_data);

	// Picture View Example
	Label * picture_label = new Label(layout);
		picture_label->text("Picture Example");

	Picture * png_picture = new Picture(layout);
		png_picture->filename("$(ui.images)/house.png");
		png_picture->fit_mode(IMAGE_FIT_ASPECT_FIT);
		png_picture->margin(10, 10);

	Switch * switch_widget2 = new Switch(layout);
		switch_widget2->text("Switch");
		switch_widget2->checked(true);
	Picture * jpg_picture = new Picture(layout);
		jpg_picture->filename("$(ui.images)/filleperle.jpg");
		jpg_picture->fit_mode(IMAGE_FIT_ASPECT_FIT);
		jpg_picture->margin(10, 10);
	Switch * switch_widget3 = new Switch(layout);
		switch_widget3->text("Switch");
		switch_widget3->checked(true);
UIManager::desktop()->mainloop();
while(1)
	UIManager::desktop()->dispatch("$(ui.tests)/out/picture8_0.svg");
}

void Picture::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
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
