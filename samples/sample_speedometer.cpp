#include "samples/samples.hpp" // Includes sample components for graphical elements

using namespace berialdraw; // Uses the berialdraw namespace to simplify calls to classes and functions

/** Declaration of the SampleSpeedometer class.
Represents a speedometer UI component that displays speed with a sliding control. */
class SampleSpeedometer : public Dialog
{
public:
	/** Constructor initializes the SampleSpeedometer interface.
	Links it to the sets up the main layout, and creates the speed slider. */
	SampleSpeedometer() : Dialog()  // Initializes the base dialog
	{
		// Creates the main layout for the speedometer interface
		Column * main_layout = new Column(m_content);

		// Initializes the speedometer interface components
		create(main_layout);

		// Creates a speed slider and configures its properties
		Slider * speed = new Slider(main_layout);
			speed->bind(this, &SampleSpeedometer::on_slide_event); // Bind slider events to handler
			speed->margin(20, 0);                                  // Add vertical margin
			speed->focusable(false);                               // Disable focus for the slider
	}

	/** Event handler for the slider's slide events.
	Updates the speedometer's needle and speed text based on the slider's value.
	@param widget      Pointer to the triggering widget (slider).
	@param slide_event Slide event containing the current value of the slider. */
	void on_slide_event(Widget * widget, const SlideEvent& slide_event)
	{
		// Updates the needle's angle based on the slider's value
		m_needle->angle_q6(0 - (slide_event.value() * m_angle_inc * 64) / m_speed_inc);

		// Updates the speed text to display the current slider value
		m_speed_text->text("%d", slide_event.value());
	}

protected:
	/** Draws the graduations on the speedometer dial. */
	void draw_graduations();

	/** Draws the text labels for the graduations. */
	void draw_text();

	/** Draws the dial of the speedometer. */
	void draw_dial(Column * column);

	/** Draws the needle of the speedometer. */
	void draw_needle();

	/** Draws the text displaying the current speed. */
	void draw_speed_text();

	/** Creates the speedometer's graphical components. */
	void create(Column * column);

	// Center point of the speedometer dial
	Point m_dial_center;

	// Components of the speedometer
	Polygon * m_needle = nullptr;           // Represents the speedometer's needle
	Text * m_speed_text = nullptr;          // Displays the current speed as text

	// Dimensions and styling for the speedometer
	Dim m_counter_radius = 200;             // Radius of the speedometer dial
	Dim m_font_size = m_counter_radius / 6; // Font size for the speed text

	// Canvas for custom drawing
	Canvas * m_canvas;

	// Speed configuration
	Dim m_speed_min = 0;          // Minimum speed value
	Dim m_speed_max = 100;        // Maximum speed value
	Dim m_speed_inc = 10;         // Speed increment for graduations

	// Angle configuration
	Coord m_angle_start = -90;    // Starting angle of the dial (leftmost point)
	Coord m_angle_end = 180;      // Ending angle of the dial (rightmost point)
	Coord m_angle_inc = (abs(m_angle_start) + m_angle_end) / ((m_speed_max - m_speed_min) / m_speed_inc); // Angle increment per speed unit
};

// Draws the dial background for the speedometer.
void SampleSpeedometer::draw_dial(Column * column)
{
	m_dial_center.move(240, 200);                                 // Move the dial center to the correct position

	m_canvas = new Canvas(column);                                // Create the canvas for the speedometer
		m_canvas->size(480, 400);                                 // Set the canvas size
		m_canvas->color(Color::TRANSPARENT);                      // Keep the canvas background transparent
		m_canvas->id(1234);
}

// Draws the graduations on the speedometer dial.
void SampleSpeedometer::draw_graduations()
{
	Line * major_graduations = new Line(m_canvas);                // Draw the major graduations
		major_graduations->color(Color::LABEL_TEXT_COLOR);        // Set the color of the lines
		major_graduations->point1(0,8 - m_counter_radius);        // Define the start point of the lines
		major_graduations->point2(0,8 - m_counter_radius + 14);   // Define the end point of the lines
		major_graduations->position(m_dial_center);               // Center position the lines
		major_graduations->thickness(3);                          // Set the thickness of the lines
		major_graduations->repeat(Shape::REPEAT_ANGLE,            // Repeat the lines across the dial
		                        m_angle_start, m_angle_end, 
		                        m_angle_inc);

	Line * medium_graduations = new Line(m_canvas);               // Draw the medium graduations
		medium_graduations->color(Color::LABEL_TEXT_COLOR);
		medium_graduations->point1(0, 8 - m_counter_radius);
		medium_graduations->point2(0, 8 - m_counter_radius + 10);
		medium_graduations->position(m_dial_center);
		medium_graduations->thickness(2);
		medium_graduations->repeat_q6(Shape::REPEAT_ANGLE,
			m_angle_start * 64,
			m_angle_end * 64 - m_angle_inc * 32,
			m_angle_inc * 32);

	for (int angle_tenths = m_angle_start * 10;
		angle_tenths <= m_angle_end * 10 - m_angle_inc;
		angle_tenths += m_angle_inc)
	{
		Line * minor_graduations = new Line(m_canvas);            // Draw the fine graduations
			minor_graduations->color(Color::LABEL_TEXT_COLOR);
			minor_graduations->point1(0, 8 - m_counter_radius);
			minor_graduations->point2(0, 8 - m_counter_radius + 5);
			minor_graduations->position(m_dial_center);
			minor_graduations->thickness(1);
			minor_graduations->angle_q6((angle_tenths - 900) * 64 / 10);
	}
}

// Draws the texts for the graduations on the dial.
void SampleSpeedometer::draw_text()
{
	Compass compass;                                              // Create a compass to position the texts
		compass.center(m_dial_center);                            // Set the center of the compass
		compass.radius(m_counter_radius - 22 - m_font_size);      // Set the radius to position the texts

	Text * graduation_texts;                                      // Draw texts for each graduation
	int speed = 0;                                                // Initialize the speed value

	for (int angle = m_angle_end; angle >= m_angle_start; angle -= m_angle_inc)
	{
		compass.angle(angle);                                     // Rotate the compass to the correct angle
		graduation_texts = new Text(m_canvas);                    // Create a new text object
			graduation_texts->font_size(m_font_size);             // Set the font size of the text
			graduation_texts->color(Color::LABEL_TEXT_COLOR);      // Set the text color
			graduation_texts->text("%d", speed);                  // Set the text to display the speed value
			graduation_texts->center(graduation_texts->           // Center the text in its bounding box
			                         content_size().middle());
			graduation_texts->position(compass.position());       // Position the text at the compass angle
		speed += m_speed_inc;                                     // Increment the speed value
	}
}

// Draws the speed indicator (needle) for the speedometer.
void SampleSpeedometer::draw_needle()
{
	m_needle = new Polygon(m_canvas);                           // Create the needle as a polygon
		m_needle->color(Color::RED);                            // Set the needle color
		m_needle->position(m_dial_center);                      // Position the needle at the dial center
		m_needle->angle(0);                                     // Set the initial rotation angle of the needle

		// Define the shape of the needle (elongated trapezoid)
		m_needle->add_point(0 - (m_counter_radius - 10),  2);   // Add the first point
		m_needle->add_cubic(0 - (m_counter_radius -  5),  2);   // Create a rounded edge
		m_needle->add_cubic(0 - (m_counter_radius -  5), -2);   // Add the next rounded edge
		m_needle->add_point(0 - (m_counter_radius - 10), -2);   // Add another point
		m_needle->add_point(0, -8);                             // Add the bottom of the needle
		m_needle->add_cubic(13, -8);                            // Round the bottom edge
		m_needle->add_cubic(13,  8);                            // Continue rounding the bottom edge
		m_needle->add_point(0,  8);                             // Close the bottom of the needle
		m_needle->next_contour();                               // Close the first and only contour

	Circle * circle = new Circle(m_canvas);                     // Add a red circle above the needle
		circle->radius(m_counter_radius / 20);                  // Set the radius of the circle
		circle->position(m_dial_center);                        // Position the circle at the dial center
		circle->color(Color::RED);                              // Set the circle color
		circle->thickness(0);                                   // Fill the circle
}

// Draws the speed text displayed on the speedometer.
void SampleSpeedometer::draw_speed_text()
{
	m_speed_text = new Text(m_canvas);                          // Create a text element for displaying the speed
		m_speed_text->position(                                 // Position the speed text below the dial
			m_dial_center.x() - (m_counter_radius / 2) - (m_font_size), 
			m_dial_center.y() + (m_counter_radius / 2) - (m_font_size * 2));
		m_speed_text->font_size(m_font_size * 2);               // Set the font size for the speed text
		m_speed_text->text("0");                                // Initialize the speed text to "0"
		m_speed_text->color(Color::LABEL_TEXT_COLOR);            // Set the text color
}

/** Creates a speedometer ranging from 0 to 100.
Animates the needle and updates the speed text dynamically. */
void SampleSpeedometer::create(Column * column)
{
	// Draw the various components of the speedometer
	draw_dial(column);                                         // Draw the dial background
	draw_graduations();                                        // Draw the graduations on the dial
	draw_text();                                               // Add graduation labels
	draw_needle();                                             // Draw the speed indicator needle
	draw_speed_text();                                         // Add the dynamic speed text
}

// Sample function to show speedometer on canvas widget
void sample_speedometer(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleSpeedometer sample;   // Creates a SampleSpeedometer 
	sample.title("Speedometer sample");  // Add dialog title
	sample.add_back_button("Back");      // Add quit speedometer
	sample.exec();                       // Starts the interface and its main loop
}
