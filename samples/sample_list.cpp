#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleList class, representing a simple user interface with a list widget
class SampleList : public Dialog
{
public:
	// Constructor initializes the SampleList
	SampleList() :
		Dialog()  // Initializes m_window as the main application window
	{
		Column * column = new Column(m_content);                  // Creates a content column within the scroll view

		Label * label = new Label(column);                        // Creates a label above the list
			label->text("List Widget Example");                   // Sets the label text

		m_list = new List(column);                                // Creates a list widget within the column
			m_list->size(400, 150);                               // Sets a fixed size for the list
			m_list->margin(10);                                   // Adds margin around the list

		m_list->append("Item 1");                                 // Populates the list with a few default items
		m_list->append("Item 2");
		m_list->append("Item 3");

		Button * button = new Button(column);                     // Creates a button to add new items dynamically
			button->text("add");                                  // Sets button text
			button->margin(20, 10);                               // Adds margin around the button
			button->bind(this, &SampleList::on_add_button);       // Binds click event to on_add_button handler
	}

	// Event handler for the add button; appends a new item decorated with a trailing icon
	void on_add_button(Widget * widget, const ClickEvent & evt)
	{
		(void)(widget);
		(void)(evt);
		m_count ++;                                               // Increments item counter
		ListItem * item = m_list->append("image");                // Appends a new item to the list
			item->trailing("$(ui.icons)/computer.icn");           // Sets a trailing icon
			item->text_color(Color::RED);                         // Sets the item text color
	}

protected:
	List * m_list  = 0;      // List widget displaying the items
	int    m_count = 0;      // Counter for the number of items added
};

// Sample function to show List widget
void sample_list(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleList sample;                    // Creates a SampleList
	sample.title("List sample");          // Add dialog title
	sample.add_back_button("Back");       // Add back button
	sample.exec();                        // Starts the interface and its main loop
}
