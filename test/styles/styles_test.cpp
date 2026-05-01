#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Styles::test1()
{
	// Create initial window with a button
	Window window;
		window.size(200, 150);

	Column * column = new Column(&window);
		Button * button = new Button(column);
			button->text("button");
			button->margin(5);
			button->id(1);

	// Display the window before style application
	UIManager::desktop()->dispatch("$(ui.tests)/out/styles1_1.svg");

	// Now create a JSON style with custom properties
	String style_properties = 
		"{"
			"'text-color':0xFFFF0000,"             // Red text
			"'font-size':24,"                      // Font size 24
			"'text':'Styled Button',"              // New text content
			"'radius':20,"                         // Radius (64ths of pixels, 128 = 2px)
			"'border-color':0xFF0000FF,"           // Blue border
			"'thickness':6,"                       // Thickness (6 in 64ths)
			"'color':0xFFCFCFCF"
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
