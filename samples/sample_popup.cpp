#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SamplePopup class, representing a counter controlled by a popup menu
class SamplePopup : public Dialog
{
public:
	// Constructor initializes the SamplePopup
	SamplePopup() :
		Dialog() // Initializes m_window as the main application window
	{
		Column * column = new Column(m_content);                    // Creates a content column within the scroll view

		Label * instruction = new Label(column);                    // Creates an instruction label
			instruction->text("Click anywhere to change the counter"); // Explains how to open the popup
			instruction->color(Color::GRAY);

		m_counter_label = new Label(column);                        // Creates a label to display the counter
			m_counter_label->text("Count: %d", m_counter);          // Sets the initial counter text

		m_popup = new Popup(&m_window);                             // Creates a popup owned by the dialog window

		List * list = new List(m_popup);                            // Creates a list inside the popup
			list->extend(Extend::EXTEND_ALL);                       // Extends the list to the popup bounds
			list->scroll_direction(SCROLL_VERTICAL);                // Keeps the action list vertically scrollable
			list->margin(2);

		ListItem * decrease_item = list->append("Decrease");        // Adds the decrease action
			decrease_item->bind(this, &SamplePopup::on_decrease);   // Binds the decrease handler

		ListItem * increase_item = list->append("Increase");        // Adds the increase action
			increase_item->bind(this, &SamplePopup::on_increase);   // Binds the increase handler

		m_window.bind(this, &SamplePopup::on_dialog_click);         // Binds clicks on the dialog window
	}

	// Open the popup when the user clicks anywhere except the Back button or popup.
	void on_dialog_click(Widget * widget, const ClickEvent & evt)
	{
		Button * button = dynamic_cast<Button*>(widget);             // Checks whether the click hit a button
		bool is_back_button = button && widget->parent() == m_header; // Identifies the dialog's Back button
		if (is_back_button == false && m_popup->contains(widget) == false)
		{
			m_popup->open_at(evt.position());                        // Opens the popup at the click position
		}
	}

	// Close the popup and increment the displayed counter.
	void on_increase(Widget * widget, const ClickEvent & evt)
	{
		(void)(widget);                                             // The clicked button is not needed by this handler
		(void)(evt);                                                // The click details are not needed by this handler
		m_popup->close();                                           // Closes the popup after selecting an action
		m_counter++;                                                // Increments the counter
		m_counter_label->text("Count: %d", m_counter);              // Refreshes the displayed counter
	}

	// Close the popup and decrement the displayed counter.
	void on_decrease(Widget * widget, const ClickEvent & evt)
	{
		(void)(widget);                                             // The clicked button is not needed by this handler
		(void)(evt);                                                // The click details are not needed by this handler
		m_popup->close();                                           // Closes the popup after selecting an action
		m_counter--;                                                // Decrements the counter
		m_counter_label->text("Count: %d", m_counter);              // Refreshes the displayed counter
	}

protected:
	Label * m_counter_label = 0;                                    // Label for displaying the counter
	Popup * m_popup = 0;                                            // Popup containing the counter actions
	int m_counter = 0;                                              // Current counter value
};

// Sample function to show the Popup widget
void sample_popup(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);                                                 // The menu item widget is not needed here
	(void)(evt);                                                    // The menu click details are not needed here
	SamplePopup sample;                                             // Creates a SamplePopup
	sample.title("Popup sample");                                   // Adds the dialog title
	sample.add_back_button("Back");                                 // Adds a back button
	sample.exec();                                                  // Starts the interface and its main loop
}