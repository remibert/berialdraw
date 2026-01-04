#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

#define PROGRESS_BAR_ID 123
#define THEME_NAME_ID 124
// Declaration of the SampleTheme class, representing a theme selection
class SampleTheme : public Dialog
{
public:
	/** Constructor initializes the SampleTheme .
	 * Sets up two separate windows with different layouts and properties. */
	SampleTheme() :
		Dialog()    // Initializes the base Dialog
	{
		Column * column = new Column(m_content);                  // Creates a content row within the scroll view

			Row * head = new Row(column);                         // Creates a new row within the column
				Label * label = new Label(head);                  // Creates a new label within the row
					label->text("Theme color : ");                // Sets the text of the label
					label->text_align(Align::ALIGN_LEFT);         // Aligns the text to the left
					label->size_policy(SizePolicy::SHRINK_WIDTH); // Sets the size policy to shrink width

				label = new Label(head);                          // Creates another label within the row
					label->text("");                              // Sets the text of the label to an empty string
					label->id(THEME_NAME_ID);                     // Sets the ID of the label
					label->text_align(Align::ALIGN_LEFT);         // Aligns the text to the left

				Switch * dark_light = new Switch(head);           // Creates a new switch within the row
					dark_light->size_policy(SizePolicy::SHRINK_WIDTH); // Sets the size policy to shrink width
					dark_light->focusable(false);                 // Sets the switch to be non-focusable
					dark_light->checked(true);                    // Sets the switch to be checked
					dark_light->bind(this, &SampleTheme::on_dark_light_event); // Binds the switch to an event handler

				label = new Label(head);                           // Creates another label within the row
					label->text("Dark/Light");                     // Sets the text of the label
					label->size_policy(SizePolicy::SHRINK_WIDTH);  // Sets the size policy to shrink width

		// Theme color picker
		{
			Row * color_picker = new Row(column);                  // Creates a content row within the scroll view
			for (uint32_t i = FIRST_THEME_COLOR; i <= LAST_THEME_COLOR; i++)
			{
				Button *color  = new Button(color_picker);           // Creates a button within the row
					color->color(i);                                 // Sets the theme color
					color->text(" ");                                // Sets label text
					color->focusable(false);                         // Sets the button to be non-focusable
					color->radius(0);                                // Sets the radius of the button to 0
					color->margin(1);                                // Sets the margin of the button to 1
					color->id(i);                                    // Sets the ID of the button
					color->bind(this, &SampleTheme::on_color_event); // Binds the button to an event handler
					color->thickness(0);
			}
		}

		label = new Label(column);
			label->text("\nRendering Widgets");
		
		// Show top widgets
		{
			Row * row = new Row(column);                                         // Creates a new row within the column
				Column * col = new Column(row);                                  // Creates a new column within the row
					Button * button = new Button(col);                           // Creates a new button within the column
						button->text("Button");                                  // Sets the text of the button
						button->id(1122);
					Edit * edit = new Edit(col);                                 // Creates a new edit box within the column
						edit->text("Hello World");                               // Sets the text of the edit box
						edit->select_all();                                      // Selects all text in the edit box
						edit->place_holder("Edit");                              // Sets the placeholder text of the edit box
						edit->max_columns(15);                                   // Sets the maximum number of columns for the edit box
						edit->focused(true);                                     // Sets the edit box to be focused

				col = new Column(row);                                           // Creates another column within the row
					Switch * switch_ = new Switch(col);                          // Creates a new switch within the column
						switch_->checked(true);                                  // Sets the switch to be checked
					Slider * slider = new Slider(col);                           // Creates a new slider within the column
						slider->bind(this, &SampleTheme::on_slide_event);        // Binds the slider to an event handler
						slider->value(50);                                       // Sets the value of the slider
					ProgressBar * progress_bar = new ProgressBar(col);           // Creates a new progress bar within the column
						progress_bar->id(PROGRESS_BAR_ID);                       // Sets a unique ID for the progress bar
						progress_bar->value(50);                                 // Sets the value of the progress bar

				col = new Column(row);                                           // Creates another column within the row
					col->size_policy(SizePolicy::SHRINK_WIDTH);                  // Sets the size policy to shrink width
					Icon * icon = new Icon(col);                                 // Creates a new icon within the column
						icon->text("Icon");                                      // Sets the text of the icon
						icon->filename("$(ui.icons)/sentiment_excited.icn"); // Sets the filename of the icon
		}
		Keyboard * keyboard = new Keyboard(column);     // Create keyboard

		Canvas * canvas = new Canvas(column);           // Create canvas
			canvas->size(200,40);                       // Set size of canvas
			Circle * circle = new Circle(canvas);       // Creates a circle marker
				circle->position(70+18, 20);            // Sets new position for the circle
				circle->thickness(2);                   // Sets thickness of the border
				circle->radius(14);                     // Adjusts radius of the circle

			Triangle * triangle = new Triangle(canvas); // Creates a triangle marker
				triangle->thickness(2);                 // Sets thickness of the border
				triangle->position(70+18+32, 24);       // Sets new position for the triangle
				triangle->radius(16);                   // Adjusts radius of the triangle

			Square * square = new Square(canvas);       // Creates a square marker
				square->thickness(2);                   // Sets thickness of the border
				square->position(70+18+64, 20);         // Sets new position for the square
				square->radius(18);                     // Adjusts radius of the square

			Text * text = new Text(canvas);             // Create a text
				text->position(0,10);                   // Sets position of text
				text->text("Canvas");                   // Sets text 
				text->font_size(20);                    // Adjusts fonts size
	}

	// Event handler for slider
	void on_slide_event(Widget * widget, const SlideEvent& slide_event)
	{
		ProgressBar * progress_bar = dynamic_cast<ProgressBar*>(widget->root()->search(PROGRESS_BAR_ID));
		if (progress_bar)
		{
			progress_bar->value(slide_event.value());
		}
	}

	// Event on dark or light changed
	void on_dark_light_event(Widget * widget, const CheckEvent & check_event)
	{
		if (check_event.checked())
		{
			UIManager::colors()->appearance("light");
		}
		else
		{
			UIManager::colors()->appearance("dark");
		}
	}

	// Event handler for color clicked
	void on_color_event(Widget * widget, const ClickEvent& click_event)
	{
		Label * theme_name = dynamic_cast<Label*>(widget->root()->search(THEME_NAME_ID)); // Search the label for display theme name

		UIManager::colors()->theme(widget->id());                     // Change the color theme
		
		switch(widget->id())                                          // According to the theme selected
		{
		case Color::THEME_RED      :theme_name->text("RED     "); break;
		case Color::THEME_ORANGE   :theme_name->text("ORANGE  "); break;
		case Color::THEME_YELLOW   :theme_name->text("YELLOW  "); break;
		case Color::THEME_LIME     :theme_name->text("LIME    "); break;
		case Color::THEME_GREEN    :theme_name->text("GREEN   "); break;
		case Color::THEME_JADE     :theme_name->text("JADE    "); break;
		case Color::THEME_CYAN     :theme_name->text("CYAN    "); break;
		case Color::THEME_AZURE    :theme_name->text("AZURE   "); break;
		case Color::THEME_BLUE     :theme_name->text("BLUE    "); break;
		case Color::THEME_INDIGO   :theme_name->text("INDIGO  "); break;
		case Color::THEME_PURPLE   :theme_name->text("PURPLE  "); break;
		case Color::THEME_VIOLET   :theme_name->text("VIOLET  "); break;
		case Color::THEME_FUSHIA   :theme_name->text("FUSHIA  "); break;
		case Color::THEME_PINK     :theme_name->text("PINK    "); break;
		case Color::THEME_ROSEWOOD :theme_name->text("ROSEWOOD"); break;
		case Color::THEME_SAND     :theme_name->text("SAND    "); break;
		case Color::THEME_ASH      :theme_name->text("ASH     "); break;
		case Color::THEME_GRAY     :theme_name->text("GRAY    "); break;
		case Color::THEME_ZINC     :theme_name->text("ZINC    "); break;
		case Color::THEME_SLATE    :theme_name->text("SLATE   "); break;
		default:theme_name->text("       "); break;
		}
	}
};


// Sample function to show all colors themes
void sample_theme(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleTheme sample;   // Creates a SampleTheme 
	sample.title("Theme sample");   // Add dialog title
	sample.add_back_button("Back"); // Add back button
	sample.exec();                  // Starts the interface and its main loop
}
