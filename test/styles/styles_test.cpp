#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Styles::test1()
{
	// Create initial window with a button
	Window window;
		window.size(200, 150);
		window.color(Color::ALICE_BLUE);

	Column * column = new Column(&window);
		Button * button = new Button(column);
			button->text("button");
			button->margin(30);
			button->id(1);

	// Display the window before style application
	UIManager::desktop()->dispatch("$(ui.tests)/out/styles1_1.svg");

	// Now create a JSON style with custom properties
	String style_properties = 
		"{"
			"'text-color':0xFFD5D6EA,"
			"'font-size':30,"
			"'text':'-$<\"font-size\":80,\"text-color\":0xFFFF0000>S$$tyle',"
			"'radius':100,"
			"'border-color':0xFFCD5C5C,"
			"'thickness':8,"
			"'color':0xFF7D0552"
		"}";

	UIManager::styles()->add_style("button_style",style_properties);
	Json style = UIManager::styles()->get_style("button_style")->properties();
	JsonIterator style_it(style);

	// Apply the style properties to the button
	button->unserialize(style_it);

	// Display the window after style application
	UIManager::desktop()->dispatch("$(ui.tests)/out/styles1_2.svg");
}

void Styles::test2()
{
}

void Styles::test3()
{
}

void Styles::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		test3();
		test2();
		test1();
	}
}
