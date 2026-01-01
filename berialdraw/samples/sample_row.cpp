#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleRow class, representing a simple user interface with buttons and a label
class SampleRow : public Dialog
{
public:
	// Constructor initializes the SampleRow 
	SampleRow() :
		Dialog()  // Initializes m_window as the main application window
	{
		ScrollView * scroll = dynamic_cast<ScrollView*>(m_content);
		scroll->scroll_direction(ScrollDirection::ScrollHorizontal); // Set the scroll view in horizontal mode
		Row * row = new Row(m_content);                 // Creates a content row within the scroll view
		for (int i = 0; i < 26; i++)
		{
			Button *button  = new Button(row);          // Creates a button within the row
				button->text("%c",i+0x41);              // Sets label text
		}
	}
};

// Sample function to show Row widget
void sample_row(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleRow sample;                    // Creates a SampleRow 
	sample.title("Row sample");          // Add dialog title
	sample.add_back_button("Back");      // Add back button
	sample.exec();                       // Starts the interface and its main loop
}
