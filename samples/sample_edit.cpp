#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleEdit class, representing a simple user interface with buttons and a edit
class SampleEdit : public Dialog
{
public:
	// Constructor initializes the SampleEdit 
	SampleEdit() :
		Dialog()  // Initializes m_window as the main application window
	{
		// Creates a content column within the scroll view
		Column * column = new Column(m_content);

		// Default edit field
		Label * label = new Label(column);                        // Creates a label for the default edit field
			label->text("Default edit");                          // Sets the label text
			label->text_align(Align::ALIGN_LEFT);                 // Aligns the text to the left

		Edit * edit = new Edit(column);                           // Creates a default text edit field
			edit->place_holder("Enter your text");                // Sets a placeholder text

		// Time input field
		label = new Label(column);                                // Creates a label for the time input field
			label->text("\nTime");                                // Sets the label text
			label->text_align(Align::ALIGN_LEFT);                 // Aligns the text to the left

		edit = new Edit(column);                                  // Creates an edit field for time input
			edit->text("00:00:00");                               // Sets the default text to "00:00:00"
			edit->mask("%02[0..23]d:%02[0..59]d:%02[0..59]d");    // Applies a mask for time input (HH:MM:SS)

		// Temperature input field
		label = new Label(column);                                // Creates a label for the temperature input field
			label->text("\nTemperature");                         // Sets the label text
			label->text_align(Align::ALIGN_LEFT);                 // Aligns the text to the left

		edit = new Edit(column);                                  // Creates an edit field for temperature input
			edit->text("-100");                                   // Sets the default text to "-100"
			edit->mask("%[-99..99]d.%02[0..99]d");                // Applies a mask for temperature input (-99.00 to 99.99)

		// IP address input field
		label = new Label(column);                                // Creates a label for the IP address input field
			label->text("\nIp address");                          // Sets the label text
			label->text_align(Align::ALIGN_LEFT);                 // Aligns the text to the left

		edit = new Edit(column);                                  // Creates an edit field for IP address input
			edit->text("0.0.0.0");                                // Sets the default text to "0.0.0.0"
			edit->mask("%3[0..255]d.%3[0..255]d.%3[0..255]d.%3[0..255]d"); // Applies a mask for IPv4 input
	}
};

// Sample function to show Edit widget
void sample_edit(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleEdit sample;                   // Creates a SampleEdit 
	sample.title("Edit sample");         // Add dialog title
	sample.add_back_button("Back");      // Add back button
	sample.exec();                       // Starts the interface and its main loop
}
