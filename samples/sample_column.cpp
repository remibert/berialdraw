#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleColumn class, representing a simple user interface with buttons and a label
class SampleColumn : public Dialog
{
public:
	// Constructor initializes the SampleColumn 
	SampleColumn() :
		Dialog()  // Initializes m_window as the main application window
	{
		Column * column = new Column(m_content);                    // Creates a content column within the scroll view

		for (int i = 0; i < 26; i++)
		{
			Button *button  = new Button(column);                   // Creates a button within the column
				button->text("%c",i+0x41);                          // Sets label text
		}
	}
};

// Sample function to show Column widget
void sample_column(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleColumn sample;                 // Creates a SampleColumn 
	sample.title("Column sample");       // Add dialog title
	sample.add_back_button("Back");      // Add back button
	sample.exec();                       // Starts the interface and its main loop
}
