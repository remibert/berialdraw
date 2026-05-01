#include "samples/samples.hpp"

using namespace berialdraw;

Dialog::Dialog()
{
	m_main_layout = new Column(&m_window);                // Sets up a main layout as a vertical column

	m_header = new Pane(m_main_layout);                   // Creates a header pane within the main layout
	m_header->color(Color::HEADER_COLOR);                 // Sets header background color to medium blue
	m_header->size_policy(SizePolicy::SHRINK_HEIGHT);     // Configures header to shrink in height based on content

	ScrollView * scroll = new ScrollView(m_main_layout);  // Adds a scrollable view for menu items
	scroll->scroll_direction(SCROLL_VERTICAL);             // Scroll view height to fill available space
	m_content = scroll;
	m_content->align(Align::ALIGN_TOP);                   // Aligns scroll view to the top
}

void Dialog::on_menu_quit(Widget * widget, const ClickEvent & evt)
{
	Button * button = dynamic_cast<Button*>(widget);     // Casts widget to Button type to access button-specific properties
	if (button)
	{
		m_selected = "<quit>";                           // Stores the text of the selected button
	}
	UIManager::desktop()->quit();                        // Exits the main loop, closing the dialog
}

/** Add title menu */
void Dialog::title(const String & title)
{
	Label * label = new Label(m_header);               // Creates a label inside the header pane
		label->text(title);                            // Sets the title text
		label->text_color(Color::HEADER_TEXT_COLOR);   // Sets text color to white
}

/** Add close button */
void Dialog::add_back_button(const String & text)
{
	Button * button = new Button(m_header);            // Creates a button inside the header pane
		button->text("< %s", text.c_str());            // Sets button text with a back arrow and specified label
		button->text_color(Color::HEADER_TEXT_COLOR);  // Sets text color to white
		button->text_align(Align::ALIGN_LEFT);         // Aligns text to the left
		button->border_color(Color::TRANSPARENT);      // Sets border color to transparent
		button->color(Color::TRANSPARENT);             // Sets background color to medium blue
		button->thickness(0);                          // Removes border thickness
		button->align(Align::ALIGN_LEFT);              // Aligns button to the left
		button->extend(Extend::EXTEND_HEIGHT);         // Extends button height to fill available space
		button->bind(this, &Dialog::on_menu_quit);     // Binds the click event to the on_menu_click handler
		button->focusable(false);                      // Remove button focus border
}

// Executes the dialog and returns the selected item as a string
const String & Dialog::exec()
{
	m_selected = "";                                   // Initializes selected item to an empty string
	UIManager::desktop()->mainloop();                  // Runs the main event loop for the desktop
	return m_selected;                                 // Returns the selected item after loop ends
}
