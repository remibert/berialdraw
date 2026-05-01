#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleButton class, representing a simple user interface with buttons and a label
class SampleButton : public Dialog
{
public:
	// Constructor initializes the SampleButton 
	SampleButton() :
		Dialog() // Initializes m_window as the main application window
	{
		Column * column = new Column(m_content);                  // Creates a content column within the scroll view

		m_label  = new Label(column);                             // Creates a label within the column
			m_label->text("Button not clicked");                  // Sets default label text
			m_label->id(1234);
		
		Button * button = new Button(column);                     // Creates a main button that the user can click
			button->text("Click\non me");                         // Sets button text with line break
			button->font_size(50);                                // Sets font size
			button->margin(20);                                   // Adds margin around the button
			button->radius(100);                                  // Rounds corners with radius of 100
			button->thickness(4);                                 // Sets border thickness
			button->bind(this, &SampleButton::on_button_clicked); // Binds click event to on_button_clicked handler
	}

	// Event handler for button clicks; increments click count and updates label
	void on_button_clicked(Widget * widget, const ClickEvent & evt)
	{
		m_clicked ++;                                        // Increments click counter
		m_label->text("Button clicked %d times", m_clicked); // Updates label text with click count
	}

protected:
	Label  * m_label  = 0;      // Label for displaying messages
	int      m_clicked = 0;     // Counter for the number of button clicks
};

// Sample function to show Button widget
void sample_button(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleButton sample;                 // Creates a SampleButton 
	sample.title("Button sample");       // Add dialog title
	sample.add_back_button("Back");      // Add back button
	sample.exec();                       // Starts the interface and its main loop
}
