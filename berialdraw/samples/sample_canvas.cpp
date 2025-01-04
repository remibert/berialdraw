#include "samples/samples.hpp"     // Includes sample components for graphical elements

using namespace berialdraw;        // Uses the berialdraw namespace to simplify calls to classes and functions

// Declaration of the SampleCanvas class, representing a simple user interface with a canvas and a label
class SampleCanvas : public Dialog
{
public:
	// Constructor initializes the SampleCanvas 
	SampleCanvas() :
		Dialog()                             // Initializes m_window as the main application window
	{
		m_column = new Column(m_content);    // Creates a content column within the scroll view

		// Creates the main canvas area that the user can click on
		m_canvas = new Canvas(m_column);    
		m_canvas->extend(Extend::EXTEND_ALL);// Extends the canvas to occupy the full available area

		add_shapes();                         // Calls the function to add shapes to the canvas
	}

	// Function to add graphical shapes to the canvas
	void add_shapes()
	{
		m_canvas->margin(5);                 // Adds a 5-pixel margin around the canvas

		Text  * text;                        // Pointer to Text object for adding labels

		Rect * frame = new Rect(m_canvas);   // Creates a rectangle frame on the canvas
			frame->position(2, 2);           // Sets position of the frame
			frame->size(228, 427);           // Sets size of the frame
			frame->thickness(1);             // Sets frame border thickness
			frame->radius(10);               // Sets rounded corner radius

		frame = new Rect(*frame);            // Duplicates frame object with the same properties
			frame->position(240, 2);         // Sets position for the duplicated frame
			frame->size(228, 300);           // Sets size for the duplicated frame

		text = new Text(m_canvas);           // Creates a text label on the canvas
			text->text("Shape");             // Sets label text
			text->position(74, 10);          // Positions label on the canvas
			text->font_size(30);             // Sets font size for label text

		text = new Text(*text);              // Duplicates text label with the same properties
			text->text("Marker");            // Sets new text for duplicated label
			text->position(308, 10);         // Positions duplicated label on canvas

		text = new Text(m_canvas);           // Creates a new text label on the canvas
			text->text("Hello");             // Sets label text
			text->position(20, 55);          // Positions label
			text->font_size(25);             // Sets font size for label
			text->angle(-20);                // Sets rotation angle for label

		text = new Text(m_canvas);           // Creates another text label
			text->text("Text");              // Sets label text
			text->position(120, 70);         // Positions label
			text->font_size(20);             // Sets font size for label

		Polygon * polygon = new Polygon(m_canvas); // Creates a polygon shape
			polygon->position(20, 105);      // Sets position of the polygon
			polygon->add_point(0, 35);       // Adds a point to the polygon
			polygon->add_point(0, 18);       // Adds another point
			polygon->add_point(30, 24);      // Adds another point
			polygon->add_point(46, 6);       // Adds another point
			polygon->add_cubic(72, 6);       // Adds a cubic curve point
			polygon->add_cubic(72, 50);      // Adds another cubic curve point
			polygon->add_point(46, 50);      // Adds final point for the contour
			polygon->next_contour();         // Begins a new contour for the polygon

		text = new Text(m_canvas);           // Label for the polygon shape
			text->text("Polygon");           // Sets label text
			text->position(120, 130);        // Positions label
			text->font_size(20);             // Sets font size for label

		// PolyLines shape with multiple points
		PolyLines * polylines = new PolyLines(m_canvas);
			polylines->position(20,175);     // Sets position for the polyline
			polylines->append(5,20);         // Adds the starting point
			polylines->append(10,50);        // Adds more points
			polylines->append(30,10);
			polylines->append(40,40);
			polylines->append(50,0);
			polylines->append(70,50);
			polylines->thickness(4);         // Sets frame line thickness

		text = new Text(m_canvas);           // Creates a text label on the canvas
			text->text("PolyLines");         // Sets label text
			text->position(120, 190);        // Positions the label on the canvas
			text->font_size(20);             // Sets the font size for the label text

		Line * line_ = new Line(m_canvas);   // Creates a line shape
			line_->position(20, 235);        // Sets position for the line
			line_->point1(4, 4);             // Sets the starting point of the line
			line_->point2(70, 45);           // Sets the ending point of the line
			line_->thickness(4);             // Sets the thickness of the line

		text = new Text(m_canvas);           // Creates a text label on the canvas
			text->text("Line");              // Sets label text
			text->position(120, 250);        // Positions the label on the canvas
			text->font_size(20);             // Sets the font size for the label text

		Rect * rect = new Rect(m_canvas);    // Creates a rectangle shape
			rect->size(33, 46);              // Sets size of the rectangle
			rect->position(18, 300);         // Sets position for the rectangle
			rect->radius(10);                // Sets corner radius for rounded edges
			rect->thickness(0);              // Sets the border thickness to 0 (no border)

		rect = new Rect(*rect);              // Creates a rectangle shape
			rect->thickness(2);              // Sets the border thickness
			rect->size(33, 44);              // Adjusts the size of the rectangle
			rect->position(62, 301);         // Sets new position for the rectangle

		text = new Text(m_canvas);           // Creates a text label on the canvas
			text->text("Rect");              // Sets label text
			text->position(120, 310);        // Positions the label on the canvas
			text->font_size(20);             // Sets the font size for the label text

		Pie * pie = new Pie(m_canvas);       // Creates a pie shape
			pie->position(36, 384);          // Sets position for the pie shape
			pie->radius(19);                 // Sets radius of the pie shape
			pie->thickness(0);               // Sets thickness of the border to 0
			pie->rope(true);                 // Enables rope effect for pie segments
			pie->start_angle(0);             // Sets start angle for the pie segment
			pie->end_angle(300);             // Sets end angle for the pie segment

		pie = new Pie(*pie);                 // Creates a pie shape
			pie->thickness(2);               // Sets thickness of the border
			pie->radius(18);                 // Adjusts radius of the pie segment
			pie->position(80, 384);          // Sets new position for the pie shape

		text = new Text(m_canvas);           // Creates a text label on the canvas
			text->text("Pie");               // Sets label text
			text->position(120, 370);        // Positions the label on the canvas
			text->font_size(20);             // Sets the font size for the label text

		Circle * circle = new Circle(m_canvas); // Creates a circle marker
			circle->position(266, 80);       // Sets position for the circle
			circle->radius(17);              // Sets radius of the circle
			circle->thickness(0);            // Sets thickness of the border to 0

		circle = new Circle(*circle);        // Creates a circle marker
			circle->position(309, 80);       // Sets new position for the circle
			circle->thickness(2);            // Sets thickness of the border
			circle->radius(16);              // Adjusts radius of the circle

		text = new Text(m_canvas);           // Creates a text label on the canvas
			text->text("Circle");            // Sets label text
			text->position(350, 70);         // Positions the label on the canvas
			text->font_size(20);             // Sets the font size for the label text

		Triangle * triangle = new Triangle(m_canvas); // Creates a triangle marker
			triangle->position(266, 145);    // Sets position for the triangle
			triangle->radius(20);            // Sets size of the triangle's bounding radius
			triangle->thickness(0);          // Sets thickness of the border to 0

		triangle = new Triangle(*triangle);  // Creates a triangle marker
			triangle->thickness(2);          // Sets thickness of the border
			triangle->position(309, 144);    // Sets new position for the triangle
			triangle->radius(19);            // Adjusts radius of the triangle

		text = new Text(m_canvas);           // Creates a text label on the canvas
			text->text("Triangle");          // Sets label text
			text->position(350, 130);        // Positions the label on the canvas
			text->font_size(20);             // Sets the font size for the label text

		Square * square = new Square(m_canvas); // Creates a square marker
			square->position(266, 200);      // Sets position for the square
			square->radius(20);              // Sets side length of the square
			square->thickness(0);            // Sets thickness of the border to 0

		square = new Square(*square);        // Creates a square marker
			square->thickness(2);            // Sets thickness of the border
			square->position(309, 200);      // Sets new position for the square
			square->radius(19);              // Adjusts radius of the square

		text = new Text(m_canvas);           // Creates a text label on the canvas
			text->text("Square");            // Sets label text
			text->position(350, 190);        // Positions the label on the canvas
			text->font_size(20);             // Sets the font size for the label text

		// Creates a cross marker
		Cross * cross = new Cross(m_canvas); 
			cross->position(266, 260);       // Sets position for the cross
			cross->radius(20);               // Sets size of the cross arms
			cross->thickness(2);             // Sets thickness of the cross arms

		text = new Text(m_canvas);           // Creates a text label on the canvas
			text->text("Cross");             // Sets label text
			text->position(350, 250);        // Positions the label on the canvas
			text->font_size(20);             // Sets the font size for the label text

		text = new Text(m_canvas);           // Creates a text label on the canvas
			text->text("Pie, Circle, Triangle\n"
						"Square, Cross have their\n"
						"origin point in the\n"
						"center, all others are\n"
						"in the upper left corner.");
			text->font_size(16);             // Sets the font size for the label text
			text->position(240,310);         // Positions the label on the canvas
	}

protected:
	// Member variables representing UI elements and state
	Canvas * m_canvas = 0;  // Main clickable canvas
	Column * m_column = 0; // Content of dialog
};

// Sample function to show Canvas widget
void sample_canvas(Widget * widget, const ClickEvent & evt)
{
	(void)(widget);
	(void)(evt);
	SampleCanvas sample;  // Creates a SampleCanvas 
	sample.title("Canvas sample"); // Add dialog title
	sample.add_back_button("Back");      // Add back button
	sample.exec();                 // Starts the interface and its main loop
}
