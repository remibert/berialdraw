#include "samples/samples.hpp"      // Includes sample graphical components

using namespace berialdraw;         // Uses the berialdraw namespace to simplify access to classes and functions

// Constructor for the MenuDialog class, initializing the main layout and header
MenuDialog::MenuDialog() : Dialog()
{
	m_column = new Column(m_content);                    // Creates a content column within the scroll view
}

// Event handler for menu item clicks
void MenuDialog::on_menu_click(Widget * widget, const ClickEvent & evt)
{
	Button * button = dynamic_cast<Button*>(widget);   // Casts widget to Button type to access button-specific properties
	if (button)
	{
		m_selected = button->text();                   // Stores the text of the selected button
	}
	UIManager::desktop()->quit();                      // Exits the main loop, closing the dialog
}

/** Adds a selectable choice to the menu */
Button * MenuDialog::add_choice(const String & text)
{
	// Creates a new button for the menu
	Button * button = create_button(text);
	button->bind(this, &MenuDialog::on_menu_click); // Binds the click event to the `on_menu_click` handler
	return button;                                              // Returns the newly created icon
}

/** Create button in menu */
Button * MenuDialog::create_button(const String & text)
{
	Button * button = new Button(m_column);            // Creates a new button in the content area for a menu choice
		button->text(text);                            // Sets button text to the provided item
		button->text_align(Align::ALIGN_LEFT);         // Aligns text to the left
		button->border_color(Color::LIGHT_BLACK,64);   // Sets border color to light black
		button->sides(Sides::BOTTOM_SIDE);       // Draws border only on the bottom side
		button->margin(0,0,1,0);                       // Adds margin around the button
		button->sides(Sides::BOTTOM_SIDE);
		
		button->thickness(1);                          // Sets border thickness to 1
		button->radius(0);                             // Sets corner radius to 0 (square corners)

	Label * label = new Label(button);                 // Adds a label inside the button for a right arrow
		label->text(">");                              // Sets label text to right arrow
		label->text_color(BUTTON_TEXT_COLOR);          // Sets arrow text color to light black
		label->text_align(Align::ALIGN_RIGHT);         // Aligns arrow text to the right
		label->font_size(button->font_size());         // Matches label font size to button font size

	return button;                                     // Returns the created button
}


