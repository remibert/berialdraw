#include "samples/samples.hpp"

using namespace berialdraw;

// Declaration of the SampleTabView class, with one labeled widget on each tab.
class SampleTabView : public Dialog
{
public:
	SampleTabView() :
		Dialog()                                             // Initializes the dialog and its content area
	{
		Column * content = new Column(m_content);            // Creates a column for the tab view
		content->margin(8);                                  // Adds spacing around the tab view

		m_tabs = new TabView(content);                       // Creates the tab container

		Column * page = create_tab_content("Button");        // Creates the Button tab and its label
		Button * button = new Button(page);                  // Adds a button to the tab page
			button->text("Click me");                        // Sets the button caption

		page = create_tab_content("Canvas");                 // Creates the Canvas tab and its label
		Canvas * canvas = new Canvas(page);                  // Creates a drawing area on the tab page

		// Draws a star and a triangle on the canvas.
		Star * star = new Star(canvas);                      // Creates a star marker on the canvas
			star->position(35, 55);                          // Positions the star
			star->radius(32);                                // Sets the star size
			star->thickness(0);                              // Fills the star without an outline

		Triangle * triangle = new Triangle(canvas);          // Creates a triangle marker on the canvas
			triangle->position(185, 65);                     // Positions the triangle beside the star
			triangle->radius(32);                            // Sets the triangle size
			triangle->thickness(0);                          // Fills the triangle without an outline

		page = create_tab_content("Combo");                  // Creates the Combo tab and its label
		Combo * combo = new Combo(page);                     // Creates a selection combo box
			combo->size(220, 45);                            // Sets the combo box size
			combo->editable(false);                          // Restricts the combo box to its choices
			combo->append("First option");                   // Adds the first choice
			combo->append("Second option");                  // Adds the second choice
			combo->append("Third option");                   // Adds the third choice
			combo->select(0);                                // Selects the first choice initially

		page = create_tab_content("Edit");                   // Creates the Edit tab and its label
		Edit * edit = new Edit(page);                        // Creates a text input field
			edit->text("Type here");                         // Sets its initial text
		Keyboard * keyboard = new Keyboard(page);            // Creates a virtual keyboard

		page = create_tab_content("Icon");                   // Creates the Icon tab and its label
		Icon * icon = new Icon(page);                        // Creates an icon widget
			icon->filename("$(ui.icons)/computer.icn");      // Loads the computer icon
			icon->text("Computer");                          // Adds a caption below the icon

		page = create_tab_content("List");                   // Creates the List tab and its label
		List * list = new List(page);                        // Creates a scrollable list
			list->append("First item");                      // Adds the first list item
			list->append("Second item");                     // Adds the second list item
			list->append("Third item");                      // Adds the third list item

		page = create_tab_content("Slider");                 // Creates the Slider tab and its label
		Slider * slider = new Slider(page);                  // Creates a value slider
			slider->value(60);                               // Sets the initial value

		page = create_tab_content("Switch");                 // Creates the Switch tab and its label
		Switch * toggle = new Switch(page);                  // Creates a toggle switch
			toggle->checked(true);                           // Starts the switch in the on state
	}

private:
	// Create a tab page with its descriptive label and a vertical content layout.
	Column * create_tab_content(const char * title)
	{
		Pane * tab_page = m_tabs->add_tab(title);            // Creates a tab and its content page
		Column * content = new Column(tab_page);             // Stacks the label and widget vertically
		content->margin(12);                                 // Adds spacing inside the tab page

		Label * label = new Label(content);                  // Creates a descriptive label for the widget
			label->text("%s widget example", title);         // Names the widget shown on this tab
			label->margin(8);                                // Separates the label from the widget

		return content;
	}

	TabView * m_tabs = 0;
};

// Sample function to show the TabView widget.
void sample_tab_view(Widget * widget, const ClickEvent & evt)
{
	(void)widget;                                            // The menu widget is not needed here
	(void)evt;                                               // The click details are not needed here
	SampleTabView sample;                                    // Creates the TabView sample dialog
	sample.title("TabView sample");                          // Sets the dialog title
	sample.add_back_button("Back");                          // Adds a button to return to the menu
	sample.exec();                                           // Runs the dialog event loop
}
