#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleKeyboard class, representing a simple user interface with buttons and a keyboard
class SampleKeyboard : public Dialog
{
public:
	// Constructor initializes the SampleKeyboard 
	SampleKeyboard() :
		Dialog()  // Initializes m_window as the main application window
	{
		m_column = new Column(m_content);                     // Creates a content column within the scroll view

		Label * label = new Label(m_column);                  // Creates label
			label->text("Enter text with keyboard");          // Set text to label

		Edit * edit = new Edit(m_column);                     // Create edit field
			edit->margin(10);                                 // Add margins around edit field
			edit->max_lines(7);                               // Set the edit field on many lines

		Keyboard * keyboard = new Keyboard(m_column);         // Creates a text keyboard on the layout
	}

protected:
	// Member variables representing UI elements and state
	Column * m_column = 0;
};

// Sample function to show Keyboard widget
void sample_keyboard(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleKeyboard sample;            // Creates a SampleKeyboard 
	sample.title("Keyboard sample");  // Add dialog title
	sample.add_back_button("Back");   // Add back button
	sample.exec();                    // Starts the interface and its main loop
}
