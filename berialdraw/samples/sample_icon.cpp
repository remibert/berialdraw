#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleIcon class, representing a simple user interface with icons and a label
class SampleIcon : public Dialog
{
public:
	// Constructor initializes the SampleIcon 
	SampleIcon() :
		Dialog()  // Initializes m_window as the main application window
	{
		m_row = new Row(m_content);                               // Creates a row layout within the scroll view
		m_row->flow(true);                                        // Enables flow layout for automatic positioning of elements

		// Creates a default icon
		m_icon = new Icon(m_row);                                  // Adds a new icon to the content row
			m_icon->zoom(4);                                       // Sets the zoom level of the icon to 4x
			m_icon->filename("${icons}/computer.icn");      // Sets the filename for the icon image
			m_icon->text("Default");                               // Sets the icon's text to "Default"
			m_icon->bind(this, &SampleIcon::on_icon_clicked);      // Binds the click event to the `on_icon_clicked` handler

		// Creates an icon without focus capability
		m_icon = new Icon(m_row);                                  // Adds another icon to the content row
			m_icon->zoom(4);                                       // Sets the zoom level of the icon to 4x
			m_icon->filename("${icons}/computer.icn");      // Sets the filename for the icon image
			m_icon->text("Without\nfocus");                        // Sets the icon's text to "Without\nfocus"
			m_icon->bind(this, &SampleIcon::on_icon_clicked);      // Binds the click event to the `on_icon_clicked` handler
			m_icon->focusable(false);                              // Disables focus for this icon

		// Creates a rounded icon
		m_icon = new Icon(m_row);                                  // Adds a rounded icon to the content row
			m_icon->zoom(4);                                       // Sets the zoom level of the icon to 4x
			m_icon->filename("${icons}/computer.icn");      // Sets the filename for the icon image
			m_icon->text("Rounded");                               // Sets the icon's text to "Rounded"
			m_icon->radius(1000);                                  // Sets a large radius for a fully rounded shape
			m_icon->bind(this, &SampleIcon::on_icon_clicked);      // Binds the click event to the `on_icon_clicked` handler

		// Creates a square icon
		m_icon = new Icon(m_row);                                  // Adds a square icon to the content row
			m_icon->zoom(4);                                       // Sets the zoom level of the icon to 4x
			m_icon->filename("${icons}/computer.icn");      // Sets the filename for the icon image
			m_icon->text("Square");                                // Sets the icon's text to "Square"
			m_icon->radius(0);                                     // Sets the radius to 0 for sharp edges
			m_icon->bind(this, &SampleIcon::on_icon_clicked);      // Binds the click event to the `on_icon_clicked` handler

		// Creates an icon with full-color appearance
		m_icon = new Icon(m_row);                                  // Adds another icon to the content row
			m_icon->zoom(4);                                       // Sets the zoom level of the icon to 4x
			m_icon->filename("${icons}/computer.icn");      // Sets the filename for the icon image
			m_icon->text("Full color");                            // Sets the icon's text to "Full color"
			m_icon->bind(this, &SampleIcon::on_icon_clicked);      // Binds the click event to the `on_icon_clicked` handler

		// Creates a label within the content row, displaying the initial message
		m_label = new Label(m_row);                                // Adds a new label to the content row
			m_label->text("Icon not\nclicked");                    // Sets the default text of the label
			m_label->font_size(16);                                // Sets the font size of the label text to 16
	}

	// Event handler for icon clicks; increments click count and updates label
	void on_icon_clicked(Widget * widget, const ClickEvent & evt)
	{
		m_clicked ++; // Increments click counter
		m_label->text("Icon\nclicked\n%d times", m_clicked); // Updates label text with click count
	}

protected:
	// Member variables representing UI elements and state
	Label  * m_label   = 0; // Label for displaying messages
	Icon   * m_icon    = 0; // Main clickable icon
	Row    * m_row = 0; 
	int      m_clicked = 0; // Counter for the number of icon clicks
};

// Sample function to show Icon widget
void sample_icon(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleIcon sample;                   // Creates a SampleIcon 
	sample.title("Icon sample");         // Add dialog title
	sample.add_back_button("Back");      // Add quit icon
	sample.exec();                       // Starts the interface and its main loop
}
