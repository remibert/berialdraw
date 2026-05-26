#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleTableView class, representing a simple user interface with a table view
class SampleTableView : public Dialog
{
public:
	// Constructor initializes the SampleTableView 
	SampleTableView() :
		Dialog()  // Initializes m_window as the main application window
	{
		Column * column = new Column(m_content);                  // Creates a content column within the scroll view

		m_label  = new Label(column);                             // Creates a label within the column
			m_label->text("Select a row");                        // Sets default label text
			m_label->margin(10);                                  // Adds margin

		m_table = new TableView(column);                          // Creates a table view within the column
			m_table->margin(10);                                  // Adds margin around the table

		// Load table data
		String table_data(
			"["
			"['Name','Age','City'],"
			"['Alice','28','Paris'],"
			"['Bob','35','Lyon'],"
			"['Charlie','42','Marseille'],"
			"['Diana','31','Toulouse'],"
			"['Eve','55','Nice']"
			"]"
		);
		m_table->load(table_data);                               // Load the data into the table
		m_table->bind(this, &SampleTableView::on_row_clicked);   // Bind click event
	}

	// Event handler for table row clicks; updates label
	void on_row_clicked(Widget * widget, const ClickEvent & evt)
	{
		(void)(widget);
		(void)(evt);
		m_label->text("Row selected");                            // Updates label when row is clicked
	}

protected:
	Label     * m_label  = 0;      // Label for displaying messages
	TableView * m_table  = 0;      // TableView widget for displaying data
};

// Sample function to show TableView widget
void sample_tableview(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleTableView sample;                 // Creates a SampleTableView 
	sample.title("TableView sample");       // Add dialog title
	sample.add_back_button("Back");         // Add back button
	sample.exec();                          // Starts the interface and its main loop
}
