#include "berialdraw_imp.hpp"
using namespace berialdraw;
#include "styles/style_manager.hpp"

/** Test 1: Basic style registration and retrieval */
void StyleManager::test1()
{
	StyleManager manager;
	
	// Create and register a simple style
	manager.add_style("primary", "{}");
	
	// Verify it exists and can be retrieved
	assert(manager.has_style("primary"));
	assert(manager.get_style("primary") != nullptr);
	assert(manager.style_count() == 1);
	
	// Create and register another style
	manager.add_style("secondary", "{}");
	
	assert(manager.has_style("secondary"));
	assert(manager.style_count() == 2);
}

/** Test 2: Get all style names */
void StyleManager::test2()
{
	StyleManager manager;
	
	manager.add_style("header", "{}");
	manager.add_style("footer", "{}");
	manager.add_style("body", "{}");
	
	Vector<String> names = manager.get_style_names();
	assert(names.size() == 3);
	
	// Test contains expected names
	bool found_header = false, found_footer = false, found_body = false;
	for (uint32_t i = 0; i < names.size(); i++)
	{
		if (names[i] == "header") found_header = true;
		if (names[i] == "footer") found_footer = true;
		if (names[i] == "body") found_body = true;
	}
	
	assert(found_header && found_footer && found_body);
}

void StyleManager::test3()
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
	UIManager::desktop()->dispatch("$(ui.tests)/out/style_manager3_1.svg");

	// Now create a JSON style with custom properties
	String style_properties = 
		"{"
		"'text-color':0xFFFF0000,"             // Red text
		"'font-size':24,"                      // Font size 24
		"'text':'Styled Button',"              // New text content
		"'radius':20,"                          // Radius (64ths of pixels, 128 = 2px)
		"'border-color':0xFF0000FF,"           // Blue border
		"'thickness':6,"                        // Thickness (6 in 64ths)
		"'color':0xFFCFCFCF"
		"}";

	UIManager::styles()->add_style("button_style",style_properties);
	Json style = UIManager::styles()->get_style("button_style")->properties();
	JsonIterator style_it(style);

	// Apply the style properties to the button
	button->unserialize(style_it);

	// Display the window after style application
while(1)
	UIManager::desktop()->dispatch("$(ui.tests)/out/style_manager3_2.svg");
}

/** Main test runner - execute all tests */
void StyleManager::test()
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

