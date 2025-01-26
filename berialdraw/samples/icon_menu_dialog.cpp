#include "samples/samples.hpp"

using namespace berialdraw;

IconMenuDialog::IconMenuDialog() : Dialog()
{
	// Initializes the main content row for the dialog
	m_row = new Row(m_content);                               // Creates a row layout within the dialog
		m_row->flow(true);                                    // Enables flow layout for automatic positioning of icons
}

void IconMenuDialog::on_menu_click(Widget * widget, const ClickEvent & evt)
{
	// Handles menu click events
	Icon * icon = dynamic_cast<Icon*>(widget);                // Safely casts the widget to an Icon
	if (icon)                                                 // Checks if the cast was successful
	{
		m_selected = icon->text();                            // Updates the selected text with the clicked icon's text
	}
	UIManager::desktop()->quit();                             // Closes the dialog after the menu selection
}

/** Create icon in menu */
Icon * IconMenuDialog::create_icon(const String & text, const String & icon_filename)
{
	// Creates a new icon for the menu choice
	Icon * icon = new Icon(m_row);                            // Adds a new icon to the content row
		// Text settings
		icon->text(text);                                     // Sets the icon's text
		icon->font_size(15);                                  // Sets the font size of the text
		icon->text_align(Align::CENTER);                      // Aligns the text to the center

		// Icon settings
		icon->filename(icon_filename);                        // Sets the filename for the icon image
		icon->zoom(3);                                        // Sets the zoom level for the icon
		icon->icon_padding(10);                               // Adds padding around the icon
		icon->icon_color(Color::WHITE);                       // Sets the color of icon
		icon->color(m_color);                                 // Sets the computed color of background icon
		icon->thickness(0);                                   // Sets the thickness of borders
		icon->radius(200);                                    // Sets the radius of rounded borders
		icon->icon_padding(20);                               // Adds paddings between icon and borders
		m_color ++;
		if (m_color > Color::LAST_THEME_COLOR)
		{
			m_color = Color::FIRST_THEME_COLOR;
		}

		// Other settings
		icon->focusable(false);                               // Disables focus for the icon
		icon->margin(5);                                      // Sets a margin of 5 around the icon
	return icon;
}


/** Add choice in menu */
Icon * IconMenuDialog::add_choice(const String & text, const String & icon_filename)
{
	// Creates a new icon for the menu choice
	Icon * icon = create_icon(text, icon_filename);
	icon->bind(this, &IconMenuDialog::on_menu_click); // Binds the click event to the `on_menu_click` handler
	return icon;                                              // Returns the newly created icon
}
