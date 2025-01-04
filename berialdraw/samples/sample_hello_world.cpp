#include "berialdraw.hpp"

using namespace berialdraw;

// Click on button callback
void on_click_button(Widget * widget, const ClickEvent & evt)
{
	UIManager::desktop()->quit();                        // Exits the main loop, closing the dialog
}

// Example of a window displaying a text "hello world" with an "ok" button.
// The example contains the initialization of the library, as well as the deinitialization
void sample_hello_world()
{
	// Initializes the UI manager with a new DeviceScreen named "BerialDraw", 
	// setting the screen width 320, height 480, pixel format, and zoom size
	UIManager::init(new DeviceScreen("Sample"), 320, 480, Framebuf::ARGB8888, 2);

		Window window;                                  // Creates a new window
			window.size(150,100);                       // Sizes the window (without a defined position it will be centered on the screen)

			Column * column = new Column(&window);      // Creates a column layout

				Label * label = new Label(column);      // Creates a label
					label->text("Hello world");         // Sets the text of the label

				Button * button = new Button(column);   // Creates a new button within the window
					button->text("OK");                 // Sets the text of the button
					button->bind(&on_click_button);     // Binds the button to the on_click_button event handler
					button->margin(10);                 // Adds a margin around the button
	
	// IMPORTANT : 
	// All widgets attached to a window must be created by a new, 
	// they do not need to be deleted, they will be automatically 
	// when the window is destroyed

	// Starts the main loop of the UI manager
	UIManager::desktop()->mainloop();

	// Terminates use of the library, destroys all objects and frees the allocated memory
	UIManager::deinit();
}
