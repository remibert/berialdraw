#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleWindow class, representing a simple user interface with buttons and a window
class SampleWindow : public Dialog
{
public:
	/** Constructor initializes the SampleWindow .
	 * Sets up two separate windows with different layouts and properties. */
	SampleWindow() :
		Dialog(),    // Initializes the base Dialog
		m_window1(), // First sub-window
		m_window2()  // Second sub-window
	{
		// Configure the second window
		{
			m_window1.size(300, 190);                      // Sets the size of the second window
			m_window1.position(40, 100);                   // Positions the window at (40, 100)

			// Adds a pane container inside the second window
			Pane * pane = new Pane(&m_window1);
				pane->radius(15);                          // Rounded corners with a radius of 15
				pane->thickness(1);                        // Border thickness of 1
				pane->color(Color::WHITE_GRAY);            // Sets the pane background color
				pane->border_color(Color::THEME_COLOR_LIGHT); // Sets the border color
				pane->margin(5);                           // Adds a margin around the pane

				// Adds a column layout inside the pane
				Column * column = new Column(pane);
				
					// Adds a label inside the column
					Label * label = new Label(column);
						label->text("Window placed in \nabsolute"); // Sets the label text
						label->margin(5);                     // Adds a margin around the label

					// Adds a button inside the column
					Button * button = new Button(column);
						button->text("Ok");                   // Sets the button text
						button->margin(5);                    // Adds a margin around the button
		}

		// Configure the first window
		{
			m_window2.size(100, 90);                       // Sets the size of the first window
			m_window2.align(Align::ALIGN_BOTTOM);          // Aligns the window to the bottom
			m_window2.extend(Extend::EXTEND_WIDTH);        // Extends the window's width across the screen

			// Adds a pane container inside the first window
			Pane * pane = new Pane(&m_window2);
				pane->radius(15);                          // Rounded corners with a radius of 15
				pane->thickness(1);                        // Border thickness of 1
				pane->color(Color::WHITE_GRAY);            // Sets the pane background color
				pane->border_color(Color::THEME_COLOR_LIGHT); // Sets the border color
				pane->margin(5);                           // Adds a margin around the pane

				// Adds a column layout inside the pane
				Column * column = new Column(pane);
				
					// Adds a label inside the column
					Label * label = new Label(column);
						label->text("Extended window width at bottom"); // Sets the label text
						label->margin(5);                     // Adds a margin around the label

					// Adds a button inside the column
					Button * button = new Button(column);
						button->text("Ok");                   // Sets the button text
						button->margin(5);                    // Adds a margin around the button
		}
	}

protected:
	Window m_window2; // First sub-window
	Window m_window1; // Second sub-window
};


// Sample function to show Window widget
void sample_window(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleWindow sample;   // Creates a SampleWindow 
	sample.title("Window sample");  // Add dialog title
	sample.add_back_button("Back"); // Add back button
	sample.exec();                  // Starts the interface and its main loop
}
