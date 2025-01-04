#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleForms class, representing a simple user interface with buttons and a forms
class SampleForms : public Dialog
{
public:
	// Constructor initializes the SampleForms 
	SampleForms() :
		Dialog()  // Initializes m_window as the main application window
	{
		Column * page = new Column(m_content);
			Pane * pane = new Pane(page);
				pane->radius(10);                             // Sets the border radius of the pane
				pane->thickness(2);                           // Sets the border thickness of the pane
				pane->margin(100,10);
				pane->extend(Extend::EXTEND_ALL);            // Configures the pane to not extend

			// Creates a content column within the scroll view
			Column * column = new Column(pane);
				column->margin(10);

			Edit * login = new Edit(column);                          // Creates a default text edit field
				login->place_holder("Username");                      // Sets a placeholder text
				//login->max_columns(20);

			Edit * password = new Edit(column);                       // Creates a default text edit field
				password->place_holder("Password");                   // Sets a placeholder text
				password->password(true);                             // Hide the entry
				//password->max_columns(20);

			Row * row = new Row(column);
				Switch * remember_me = new Switch(row);
					remember_me->align(Align::ALIGN_RIGHT);
					remember_me->size_policy(SizePolicy::SHRINK_ALL);
				Label * label = new Label(row);
					label->text("Remember me");
					label->text_align(Align::ALIGN_LEFT);
					//label->size_policy(SizePolicy::SHRINK_ALL);

			Button * sign_in = new Button(column);
				sign_in->text("Sign in");
				sign_in->margin(30,10);
				//sign_in->size_policy(SizePolicy::SHRINK_ALL);

		Keyboard * keyboard = new Keyboard(page);
	}
};

// Sample function to show forms
void sample_forms(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleForms sample;                   // Creates a SampleForms 
	sample.title("Forms sample");         // Add dialog title
	sample.add_back_button("Back");      // Add back button
	sample.exec();                       // Starts the interface and its main loop
}
