#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SamplePicture class, representing a simple user interface with picture display
class SamplePicture : public Dialog
{
public:
	// Constructor initializes the SamplePicture 
	SamplePicture() :
		Dialog()  // Initializes m_window as the main application window
	{
		Row * row = new Row(m_content);                           // Creates a row layout within the scroll view
			row->margin(20);                                      // Adds margin around the row

		// Display PNG image
		Picture * png_image = new Picture(row);                   // Creates a picture for PNG image
			png_image->filename("$(ui.images)/house.png");        // Sets the PNG filename
			png_image->fit_mode(IMAGE_FIT_ASPECT_FIT);            // Keep aspect ratio
			png_image->margin(10);                                // Adds margin

		// Display JPG image
		Picture * jpg_image = new Picture(row);                   // Creates a picture for JPG image
			jpg_image->filename("$(ui.images)/filleperle.jpg");   // Sets the JPG filename
			jpg_image->fit_mode(IMAGE_FIT_ASPECT_FIT);            // Keep aspect ratio
			jpg_image->margin(10);                                // Adds margin
	}

protected:
	// No member variables needed for this simple example
};

// Sample function to show Picture widget
void sample_picture(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SamplePicture sample;                 // Creates a SamplePicture 
	sample.title("Picture sample");       // Add dialog title
	sample.add_back_button("Back");       // Add back button
	sample.exec();                        // Starts the interface and its main loop
}
