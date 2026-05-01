#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleProgressBar class, representing a simple user interface with progress_bars and a label
class SampleProgressBar : public Dialog
{
public:
	// Constructor initializes the SampleProgressBar 
	SampleProgressBar() :
		Dialog()  // Initializes m_window as the main application window
	{
		Column * main_layout = new Column(m_content);              // Creates a content column within the scroll view
		ProgressBar * progress_bar;                                // Pointer for progress bar creation

		Label * label = new Label(main_layout);                    // Creates a label in the main layout
			label->text("Horizontal progress bars");               // Sets the label text to "Horizontal progress bars"

		Column * horizontal = new Column(main_layout);             // Creates a column layout for horizontal progress bars
			progress_bar = new ProgressBar(horizontal);            // Creates a horizontal progress bar
				progress_bar->id(++m_id);                          // Sets a unique ID for the progress bar
				progress_bar->value(33);                           // Sets the initial value to 33%

			progress_bar = new ProgressBar(horizontal);            // Creates another horizontal progress bar
				progress_bar->id(++m_id);                          // Sets a unique ID for the progress bar
				progress_bar->align(Align::ALIGN_RIGHT);           // Aligns the progress bar to the right
				progress_bar->value(33);                           // Sets the initial value to 33%

			progress_bar = new ProgressBar(horizontal);            // Creates another horizontal progress bar
				progress_bar->id(++m_id);                          // Sets a unique ID for the progress bar
				progress_bar->track_size(3);                       // Sets the track thickness to 3
				progress_bar->fill_size(20);                       // Sets the fill thickness to 20
				progress_bar->fill_color(Color::SUCCESS_COLOR);    // Sets the fill color to a success color
				progress_bar->value(33);                           // Sets the initial value to 33%

			progress_bar = new ProgressBar(horizontal);            // Creates another horizontal progress bar
				progress_bar->id(++m_id);                          // Sets a unique ID for the progress bar
				progress_bar->align(Align::ALIGN_RIGHT);           // Aligns the progress bar to the right
				progress_bar->track_size(3);                       // Sets the track thickness to 3
				progress_bar->fill_size(20);                       // Sets the fill thickness to 20
				progress_bar->fill_color(Color::ERROR_COLOR);      // Sets the fill color to an error color
				progress_bar->value(33);                           // Sets the initial value to 33%

		label = new Label(main_layout);                            // Creates a label in the main layout
			label->text("Vertical progress bars");                 // Sets the label text to "Vertical progress bars"

		Row * vertical = new Row(main_layout);                     // Creates a row layout for vertical progress bars
			vertical->min_size(0, 200);                            // Sets the minimum size for the vertical row

			progress_bar = new ProgressBar(vertical);              // Creates a vertical progress bar
				progress_bar->extend(Extend::EXTEND_HEIGHT);       // Allows the progress bar to extend vertically
				progress_bar->align(Align::ALIGN_BOTTOM);          // Aligns the progress bar to the bottom
				progress_bar->id(++m_id);                          // Sets a unique ID for the progress bar
				progress_bar->value(33);                           // Sets the initial value to 33%

			progress_bar = new ProgressBar(vertical);              // Creates another vertical progress bar
				progress_bar->extend(Extend::EXTEND_HEIGHT);       // Allows the progress bar to extend vertically
				progress_bar->align(Align::ALIGN_TOP);             // Aligns the progress bar to the top
				progress_bar->id(++m_id);                          // Sets a unique ID for the progress bar
				progress_bar->value(33);                           // Sets the initial value to 33%

			progress_bar = new ProgressBar(vertical);              // Creates another vertical progress bar
				progress_bar->extend(Extend::EXTEND_HEIGHT);       // Allows the progress bar to extend vertically
				progress_bar->track_size(3);                       // Sets the track thickness to 3
				progress_bar->fill_size(20);                       // Sets the fill thickness to 20
				progress_bar->fill_color(Color::SUCCESS_COLOR);    // Sets the fill color to a success color
				progress_bar->align(Align::ALIGN_BOTTOM);          // Aligns the progress bar to the bottom
				progress_bar->id(++m_id);                          // Sets a unique ID for the progress bar
				progress_bar->value(33);                           // Sets the initial value to 33%

			progress_bar = new ProgressBar(vertical);              // Creates another vertical progress bar
				progress_bar->extend(Extend::EXTEND_HEIGHT);       // Allows the progress bar to extend vertically
				progress_bar->align(Align::ALIGN_TOP);             // Aligns the progress bar to the top
				progress_bar->track_size(3);                       // Sets the track thickness to 3
				progress_bar->fill_size(20);                       // Sets the fill thickness to 20
				progress_bar->id(++m_id);                          // Sets a unique ID for the progress bar
				progress_bar->fill_color(Color::ERROR_COLOR);      // Sets the fill color to an error color
				progress_bar->value(33);                           // Sets the initial value to 33%

		m_label = new Label(main_layout);                          // Creates a label to display progress percentage
			m_label->text("%d%%", 33);                             // Sets the label text to "33%"

		Slider * slider = new Slider(main_layout);                 // Creates a slider in the main layout
			slider->margin(20);                                    // Adds margin around the slider
			slider->bind(this, &SampleProgressBar::on_slide_event);// Binds the slider to an event handler
			slider->value(33);                                     // Sets the initial slider value to 33
	}

	// Event handler for slider
	void on_slide_event(Widget * widget, const SlideEvent& slide_event)
	{
		for (uint32_t id = 0; id <= m_id; id++)
		{
			ProgressBar * progress_bar = dynamic_cast<ProgressBar*>(widget->root()->search(id));
			if (progress_bar)
			{
				progress_bar->value(slide_event.value());
			}
		}
		m_label->text("%d%%",slide_event.value());
	}

protected:
	// Member variables representing UI elements and state
	Label  * m_label  = 0; // Label for displaying messages
	uint32_t m_id = 0;
};

// Sample function to show ProgressBar widget
void sample_progress_bar(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleProgressBar sample;            // Creates a SampleProgressBar 
	sample.title("ProgressBar sample");  // Add dialog title
	sample.add_back_button("Back");      // Add quit progress_bar
	sample.exec();                       // Starts the interface and its main loop
}
