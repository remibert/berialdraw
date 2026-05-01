#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleGrid class, representing a simple user interface with buttons and a label
class SampleGrid : public Dialog
{
public:
	// Constructor initializes the SampleGrid 
	SampleGrid() :
		Dialog()  // Initializes m_window as the main application window
	{
		m_grid = new Grid(m_content);                    // Creates a content grid within the scroll view

		for (int row = 0; row < 12; row ++)
		{
			for (int column = 0; column < 6; column++)
			{
				Button *button  = new Button(m_grid);     // Creates a button within the grid
					button->text("R%d C%d",row,column);   // Sets label text
					button->cell(row,column);             // Set the position in grid layout
			}
		}
	}

protected:
	// Member variables representing UI elements and state
	Grid * m_grid = 0;
};

// Sample function to show Grid widget
void sample_grid(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleGrid sample;                   // Creates a SampleGrid 
	sample.title("Grid sample");         // Add dialog title
	sample.add_back_button("Back");      // Add back button
	sample.exec();                       // Starts the interface and its main loop
}
