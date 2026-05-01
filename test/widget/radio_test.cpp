#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Radio::test1()
{
	Window window;
		window.position(64, 10);
		window.size(200, 470);
		window.color(Color::WHITE);

	Column * grid = new Column(&window);
	
	int row = 0;
	Radio * radio;

	// Group 1: Basic radios
	radio = new Radio(grid);
		radio->group("group1");
		radio->checked(true);

	radio = new Radio(grid);
		radio->group("group1");

	radio = new Radio(grid);
		radio->group("group1");
		radio->radio_size(24, 24);

	radio = new Radio(grid);
		radio->group("group1");
		radio->radio_size(32, 32);

	radio = new Radio(grid);
		radio->group("group1");
		radio->color(Color::BLUE);
		radio->extend(Extend::EXTEND_WIDTH);
		radio->size_policy(SizePolicy::SHRINK_HEIGHT);

	radio = new Radio(grid);
		radio->group("group1");
		radio->extend(Extend::EXTEND_WIDTH);
		radio->size_policy(SizePolicy::SHRINK_HEIGHT);
		radio->radio_size(16, 16);

	radio = new Radio(grid);
		radio->group("group1");
		radio->radio_size(24, 24);
		radio->extend(Extend::EXTEND_WIDTH);
		radio->size_policy(SizePolicy::SHRINK_HEIGHT);

	radio = new Radio(grid);
		radio->group("group1");
		radio->radio_size(32, 32);
		radio->thickness(2);
		radio->extend(Extend::EXTEND_WIDTH);
		radio->size_policy(SizePolicy::SHRINK_HEIGHT);

	Label * label = new Label(grid);
		label->text(" ");

	//while(1) UIManager::desktop()->dispatch();
	UIManager::desktop()->dispatch("$(ui.tests)/out/radio1.svg");
}

void Radio::test2()
{
	// Test with text in layout
	Window window;
		window.position(0,0);
		window.size(400,200);
		window.color(Color::WHITE);

	Column * col = new Column(&window);
	
		// Radio without text
		Radio * radio1 = new Radio(col);
			radio1->text("");
			radio1->margin(10);
			radio1->group("group2");
			radio1->checked(true);
		
		// Radio with text
		Radio * radio2 = new Radio(col);
			radio2->text("Option 1");
			radio2->margin(10);
			radio2->group("group2");
		
		// Radio with text and custom styling
		Radio * radio3 = new Radio(col);
			radio3->text("Option 2_|");
			radio3->text_color(Color::GREEN);
			radio3->font_size(20);
			radio3->margin(10);
			radio3->group("group2");

	//while(1) UIManager::desktop()->dispatch();	
	UIManager::desktop()->dispatch("$(ui.tests)/out/radio2.svg");
}

void Radio::test3()
{
	// Test absolute positioning with text
	Window window;

	Pane * pane = new Pane(&window);
		pane->size(350,50);
		pane->position(100,50);
		pane->color(Color::LIGHT_GRAY);

	// Radio with absolute position and text
	Radio * radio1 = new Radio(&window);
		radio1->position(100, 50);
		radio1->size(350, 50);
		radio1->text("Absolute position with text");
		radio1->text_color(Color::BLUE);
		radio1->font_size(16);
		radio1->color(Color::LIGHT_BLUE);
		radio1->border_color(Color::BLUE);
		radio1->thickness(2);
		radio1->group("group3");
		radio1->checked(true);

	// Radio with absolute position without text
	Radio * radio2 = new Radio(&window);
		radio2->position(20, 80);
		radio2->text("");
		radio2->color(Color::LIGHT_GREEN);
		radio2->border_color(Color::GREEN);
		radio2->thickness(2);
		radio2->group("group3");

	// Radio with absolute position and different text
	Radio * radio3 = new Radio(&window);
		radio3->position(20, 130);
		radio3->size(350, 50);
		radio3->text("Another absolute radio");
		radio3->text_color(Color::RED);
		radio3->font_size(14);
		radio3->color(Color::LIGHT_RED);
		radio3->border_color(Color::RED);
		radio3->thickness(2);
		radio3->group("group3");

	//while(1) UIManager::desktop()->dispatch();	
	UIManager::desktop()->dispatch("$(ui.tests)/out/radio3.svg");
}

void Radio::test4()
{
	Window window;

	Column * column = new Column(&window);
	Radio * radio;
		radio = new Radio(column);
		radio->text("Radio");
		radio->font_size(5);

		radio = new Radio(column);
		radio->text("Radio");
		radio->font_size(25);

		radio = new Radio(column);
		radio->text("Radio");
		radio->font_size(55);

		radio = new Radio(column);
		radio->text("Radio");
		radio->font_size(255);

	UIManager::desktop()->dispatch("$(ui.tests)/out/radio4.svg");
}

void Radio::test()
{
	test4();
	test3();
	test2();
	test1();
}
