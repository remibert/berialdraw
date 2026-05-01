#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleSlider class, representing a simple user interface with sliders and a label
class SampleSlider : public Dialog
{
public:
	// Constructor initializes the SampleSlider 
	SampleSlider() :
		Dialog()  // Initializes m_window as the main application window
	{
		Column * main_layout = new Column(m_content);      // Creates a content column within the scroll view
		Slider * slider;                                          // Pointer for slider creation

		Label * label = new Label(main_layout);                   // Creates a label in the main layout
			label->text("Horizontal slider");                     // Sets the label text to "Horizontal slider"

		Column * horizontal = new Column(main_layout);            // Creates a column layout for horizontal sliders
			slider = new Slider(horizontal);                      // Creates a horizontal slider
				slider->id(++m_id);                               // Sets a unique ID for the slider
				slider->bind(this, &SampleSlider::on_slide_event);// Binds the slider to an event handler
				slider->margin(10);                               // Adds margin around the slider

			slider = new Slider(horizontal);                      // Creates another horizontal slider
				slider->id(++m_id);                               // Sets a unique ID for the slider
				slider->track_size(20);                           // Sets the thickness of the slider track
				slider->handle_size(16);                          // Sets the size of the slider handle
				slider->track_color(Color::SUCCESS_COLOR);        // Sets the track color to a success color
				slider->radius(0);                                // Sets the slider corners to sharp edges
				slider->bind(this, &SampleSlider::on_slide_event);// Binds the slider to an event handler
				slider->margin(10);                               // Adds margin around the slider

		label = new Label(main_layout);                           // Creates a label in the main layout
			label->text("Vertical slider");                       // Sets the label text to "Vertical slider"

		Row * vertical = new Row(main_layout);                    // Creates a row layout for vertical sliders
			vertical->min_size(0, 240);                           // Sets the minimum size of the vertical row

			slider = new Slider(vertical);                        // Creates a vertical slider
				slider->extend(Extend::EXTEND_HEIGHT);            // Allows the slider to extend vertically
				slider->id(++m_id);                               // Sets a unique ID for the slider
				slider->bind(this, &SampleSlider::on_slide_event);// Binds the slider to an event handler
				slider->margin(10);                               // Adds margin around the slider

			slider = new Slider(vertical);                        // Creates another vertical slider
				slider->extend(Extend::EXTEND_HEIGHT);            // Allows the slider to extend vertically
				slider->track_size(30);                           // Sets the thickness of the slider track
				slider->handle_size(26);                          // Sets the size of the slider handle
				slider->track_color(Color::SUCCESS_COLOR);        // Sets the track color to a success color
				slider->id(++m_id);                               // Sets a unique ID for the slider
				slider->margin(10);                               // Adds margin around the slider
				slider->radius(100);                              // Sets the slider corners to rounded edges
				slider->bind(this, &SampleSlider::on_slide_event);// Binds the slider to an event handler

		m_label = new Label(main_layout);                         // Creates a label for displaying the slider value
			m_label->text("0%%");                                 // Sets the initial label text to "0%"
	}

	// Event handler for slider
	void on_slide_event(Widget * widget, const SlideEvent& slide_event)
	{
		for (uint32_t id = 0; id <= m_id; id++)
		{
			Slider * slider = dynamic_cast<Slider*>(widget->root()->search(id));
			if (slider)
			{
				slider->value(slide_event.value());
			}
		}
		m_label->text("%d%%",slide_event.value());
	}

protected:
	// Member variables representing UI elements and state
	Label  * m_label  = 0; // Label for displaying messages
	uint32_t m_id = 0;
};

// Sample function to show Slider widget
void sample_slider(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleSlider sample;  // Creates a SampleSlider 
	sample.title("Slider sample"); // Add dialog title
	sample.add_back_button("Back");      // Add quit slider
	sample.exec();                 // Starts the interface and its main loop
}
