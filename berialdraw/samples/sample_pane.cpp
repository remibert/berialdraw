#include "samples/samples.hpp"
#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SamplePane class, representing a simple user interface with buttons and a pane
class SamplePane : public Dialog
{
public:
	// Constructor initializes the SamplePane 
	SamplePane() :
		Dialog()  // Initializes m_window as the main application window
	{
		{
			Pane * pane = new Pane(m_content);                // Creates a pane container within the scroll view
				pane->radius(10);                             // Sets the border radius of the pane
				pane->thickness(1);                           // Sets the border thickness of the pane
				pane->position(50, 10);                       // Positions the pane at (50, 10)
				pane->extend(Extend::EXTEND_NONE);            // Configures the pane to not extend

			Column * column = new Column(pane);               // Adds a column layout inside the pane
				Label * label = new Label(column);            // Creates a label inside the column
					label->text("Pane 1");                    // Sets the label text to "Pane 1"
					label->margin(5);                         // Adds a margin around the label

				Button * button = new Button(column);         // Creates a button inside the column
					button->text("Ok");                       // Sets the button text to "Ok"
					button->margin(5);                        // Adds a margin around the button
		}

		{
			Pane * pane = new Pane(m_content);                // Creates a pane container within the scroll view
				pane->radius(0);                              // Sets the border radius of the pane
				pane->thickness(1);                           // Sets the border thickness of the pane
				pane->position(200, 200);                     // Positions the pane at (200, 200)
				pane->extend(Extend::EXTEND_NONE);            // Configures the pane to not extend

			Column * column = new Column(pane);               // Adds a column layout inside the pane
				Label * label = new Label(column);            // Creates a label inside the column
					label->text("Pane 2");                    // Sets the label text to "Pane 2"
					label->margin(5);                         // Adds a margin around the label

				Button * button = new Button(column);         // Creates a button inside the column
					button->text("Ok");                       // Sets the button text to "Ok"
					button->margin(5);                        // Adds a margin around the button
		}

		{
			Pane * pane = new Pane(m_content);                // Creates a pane container within the scroll view
				pane->radius(5);                              // Sets the border radius of the pane
				pane->thickness(1);                           // Sets the border thickness of the pane
				pane->position(350, 390);                     // Positions the pane at (350, 390)
				pane->extend(Extend::EXTEND_NONE);            // Configures the pane to not extend

			Column * column = new Column(pane);               // Adds a column layout inside the pane
				Label * label = new Label(column);            // Creates a label inside the column
					label->text("Pane 3");                    // Sets the label text to "Pane 3"
					label->margin(5);                         // Adds a margin around the label

				Button * button = new Button(column);         // Creates a button inside the column
					button->text("Ok");                       // Sets the button text to "Ok"
					button->margin(5);                        // Adds a margin around the button
		}
	}
};

// Sample function to show Pane widget
void sample_pane(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SamplePane sample;             // Creates a SamplePane 
	sample.title("Pane sample");   // Add dialog title
	sample.add_back_button("Back");// Add back button
	sample.exec();                 // Starts the interface and its main loop
}
