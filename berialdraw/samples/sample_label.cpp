#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleLabel class, representing a simple user interface with buttons and a label
class SampleLabel : public Dialog
{
public:
	// Constructor initializes the SampleLabel 
	SampleLabel() :
		Dialog()  // Initializes m_window as the main application window
	{
		m_column = new Column(m_content);                     // Creates a content column within the scroll view

		Label * label = new Label(m_column);                  // Creates a text label on the layout
			label->text("Default label");                     // Sets the default text for the label

		label = new Label(m_column);                          // Creates a text label on the layout
			label->text("With value : %d", 1234);             // Sets the label text with a formatted integer value

		label = new Label(m_column);                          // Creates a text label on the layout
			label->text("Utf8 encoded : rémi");               // Sets UTF-8 encoded text with special characters

		label = new Label(m_column);                          // Creates a text label on the layout
			label->text("Large", 1);                          // Sets the label text with a value
			label->font_size(80, 30);                         // Sets font size: large width, smaller height

		label = new Label(m_column);                          // Creates a text label on the layout
			label->text("Narrow", 1);                         // Sets the label text with a value
			label->font_size(30, 80);                         // Sets font size: narrow width, taller height

		label = new Label(m_column);                          // Creates a text label on the layout
			label->text_align(Align::ALIGN_LEFT);             // Aligns text to the left
			label->text("Align left");                        // Sets the label text

		label = new Label(m_column);                          // Creates a text label on the layout
			label->text_align(Align::ALIGN_RIGHT);            // Aligns text to the right
			label->text("Align right");                       // Sets the label text

		label = new Label(m_column);                          // Creates a text label on the layout
			label->text("Many\nlines\nlabel");                // Sets multiline text for the label
	}

protected:
	// Member variables representing UI elements and state
	int      m_clicked = 0;     // Counter for the number of button clicks
	Column * m_column = 0;
};

// Sample function to show Label widget
void sample_label(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleLabel sample;             // Creates a SampleLabel 
	sample.title("Label sample");   // Add dialog title
	sample.add_back_button("Back"); // Add back button
	sample.exec();                  // Starts the interface and its main loop
}
