#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleSwitch class, representing a simple user interface with switchs and a label
class SampleSwitch : public Dialog
{
public:
	// Constructor initializes the SampleSwitch 
	SampleSwitch() :
		Dialog()  // Initializes m_window as the main application window
	{
		Column * main_layout = new Column(m_content);      // Creates a content column within the scroll view
		Switch * switch_;                                         // Pointer for switch creation

		Label * label = new Label(main_layout);                    // Creates a label in the main layout
			label->text("Normal switch");                          // Sets the label text

		switch_ = new Switch(main_layout);                         // Creates a switch
			switch_->bind(this, &SampleSwitch::on_check_event);    // Binds the switch to an event handler
			switch_->margin(10);                                   // Adds margin around the switch
			switch_->focusable(false);                             // Remove the focus border

		label = new Label(main_layout);                            // Creates a label in the main layout
			label->text("Customized switch");                      // Sets the label text

		switch_ = new Switch(main_layout);                         // Creates a switch
			switch_->bind(this, &SampleSwitch::on_check_event);    // Binds the switch to an event handler
			switch_->margin(10);                                   // Adds margin around the switch
			switch_->radius(40);                                   // No rounded border
			switch_->switch_size(200,80);                          // Enlarge size of switch
			switch_->thumb_padding(5);                             // Set the thumb padding in track
			switch_->on_track_color(Color::GREEN);                 // Change the track color when the switch is checked
			switch_->off_track_color(Color::LIGHT_GRAY);           // Change the track color when the switch is not checked
			switch_->focusable(false);                             // Remove the focus border

		m_label = new Label(main_layout);                          // Creates a label for displaying the switch value
			m_label->text("");                                     // Sets the initial label text to empty string
	}

	// Event handler for switch
	void on_check_event(Widget * widget, const CheckEvent& check_event)
	{
		m_label->text("%s",check_event.checked() ? "Checked" : "Unchecked");
	}

protected:
	// Member variables representing UI elements and state
	Label  * m_label  = 0; // Label for displaying messages
};

// Sample function to show Switch widget
void sample_switch(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleSwitch sample;  // Creates a SampleSwitch 
	sample.title("Switch sample"); // Add dialog title
	sample.add_back_button("Back");      // Add quit switch
	sample.exec();                 // Starts the interface and its main loop
}
