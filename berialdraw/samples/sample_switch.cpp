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

		label = new Label(main_layout);                            // Creates a label in the main layout
			label->text("Radio buttons (group 1)");                // Sets the label text

		Radio * radio;                                             // Pointer for radio creation

		radio = new Radio(main_layout);                            // Creates a radio button
			radio->bind(this, &SampleSwitch::on_radio_event);      // Binds the radio to an event handler
			radio->text("Option 1");                               // Sets the radio text
			radio->group("group1");                                // Assigns to group 1
			radio->margin(10);                                     // Adds margin around the radio
			radio->focusable(false);                               // Remove the focus border

		radio = new Radio(main_layout);                            // Creates a radio button
			radio->bind(this, &SampleSwitch::on_radio_event);      // Binds the radio to an event handler
			radio->text("Option 2");                               // Sets the radio text
			radio->group("group1");                                // Assigns to group 1
			radio->margin(10);                                     // Adds margin around the radio
			radio->focusable(false);                               // Remove the focus border

		label = new Label(main_layout);                            // Creates a label in the main layout
			label->text("Checkbox");                               // Sets the label text

		Checkbox * checkbox = new Checkbox(main_layout);           // Creates a checkbox
			checkbox->bind(this, &SampleSwitch::on_check_event);   // Binds the checkbox to an event handler
			checkbox->text("Accept terms");                        // Sets the checkbox text
			checkbox->margin(10);                                  // Adds margin around the checkbox
			checkbox->focusable(false);                            // Remove the focus border

		m_label = new Label(main_layout);                          // Creates a label for displaying the widget value
			m_label->text("");                                     // Sets the initial label text to empty string
	}

	// Event handler for radio buttons
	void on_radio_event(Widget * widget, const ClickEvent& click_event)
	{
		m_label->text("Radio selected in group 1");
	}

	// Event handler for switch and checkbox
	void on_check_event(Widget * widget, const CheckEvent& check_event)
	{
		m_label->text("%s", check_event.checked() ? "Checked" : "Unchecked");
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
