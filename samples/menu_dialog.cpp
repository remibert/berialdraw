#include "samples/samples.hpp"      // Includes sample graphical components

using namespace berialdraw;         // Uses the berialdraw namespace to simplify access to classes and functions

// Constructor for the MenuDialog class, initializing the main layout and header
MenuDialog::MenuDialog() : Dialog()
{
	m_list = new List(m_content);;                    // Creates a list view
	m_list->size_policy(SizePolicy::ENLARGE_ALL);
	m_list->focusable(true);
	m_list->margin(0);
	m_list->thickness(0);
}

// Event handler for menu item clicks
void MenuDialog::on_menu_click(Widget * widget, const ClickEvent & evt)
{
	ListItem * item = dynamic_cast<ListItem*>(widget);   // Casts widget to list item
	if (item)
	{
		m_selected = item->text();                   // Stores the text of the selected button
	}
	UIManager::desktop()->quit();                      // Exits the main loop, closing the dialog
}

/** Create item in menu */
ListItem* MenuDialog::create_menu(const String& text, const String& icon_filename)
{
	ListItem* result = m_list->append([text, icon_filename](ListItem* item) 
	{
		item->text(String(" ")+text);
		item->padding(9);
		item->leading(icon_filename);
		item->trailing(">");
	});
	return result;                                     // Returns the item menu
}


