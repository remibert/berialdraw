#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleScrollView class, representing a simple user interface with buttons and a scroll_view
class SampleScrollView : public Dialog
{
public:
	// Constructor initializes the SampleScrollView 
	SampleScrollView() :
		Dialog()  // Initializes m_window as the main application window
	{
		delete m_content;                                                       // Remove scroll view created in Dialog class
		m_content = new Column(m_main_layout);                                  // Creates a content column within the scroll view

		Label * label = new Label(m_content);                                   // Creates a label
			label->text("Horizontal scroll view");                              // Set text to label 

		ScrollView * horizontal_scroll_view = new ScrollView(m_content);        // Create scroll view
			horizontal_scroll_view->scroll_direction(SCROLL_HORIZONTAL);         // Set the scroll view in horizontal mode
			horizontal_scroll_view->size_policy(SizePolicy::ENLARGE_ALL);       // 
			Row * horizontal_layout = new Row(horizontal_scroll_view);          // Creates a content row within the scroll view
				for (int i = 0; i < 26; i++)
				{
					Button *button  = new Button(horizontal_layout);            // Creates a button within the row
						button->text("%c",i+0x41);                              // Sets label text
				}

		label = new Label(m_content);                                           // Creates a label
			label->text("Vertical scroll view");                                // Set text to label 

		ScrollView * vertical_scroll_view = new ScrollView(m_content);          // Create scroll view
			vertical_scroll_view->scroll_direction(SCROLL_VERTICAL);             // Set the scroll view in vertical mode
			vertical_scroll_view->size_policy(SizePolicy::ENLARGE_ALL);         // 
			Column  * vertical_scroll_layout = new Column(vertical_scroll_view);// Creates a content column within the scroll view
				for (int i = 0; i < 26; i++)
				{
					Button *button  = new Button(vertical_scroll_layout);       // Creates a button within the column
						button->text("%c",i+0x41);                              // Sets label text
				}
	}
};

// Sample function to show ScrollView widget
void sample_scroll_view(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleScrollView sample;    // Creates a SampleScrollView 
	sample.title("ScrollView sample");   // Add dialog title
	sample.add_back_button("Back");      // Add back button
	sample.exec();                       // Starts the interface and its main loop
}
