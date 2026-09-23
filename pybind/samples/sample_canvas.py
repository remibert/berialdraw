"""Python port of samples/sample_canvas.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleCanvas(Dialog):
    """Simple user interface with a canvas full of drawn shapes"""

    def __init__(self):
        super().__init__()
        self.column = Column(self.content)                   # Creates a content column within the scroll view

        self.canvas = Canvas(self.column)                    # Creates the main canvas area
        self.canvas.extend = Extend.EXTEND_ALL               # Extends the canvas to occupy the full available area

        self._add_shapes()

    def _add_shapes(self):
        self.canvas.margin = 5                               # Adds a 5-pixel margin around the canvas

        frame = Rect(self.canvas)                            # Creates a rectangle frame on the canvas
        frame.position = (2, 2)                              # Sets position of the frame
        frame.size = (228, 427)                              # Sets size of the frame
        frame.thickness_ = 1                                 # Sets frame border thickness
        frame.radius_ = 10                                    # Sets rounded corner radius

        frame = Rect(frame)                                  # Duplicates frame object with the same properties
        frame.position = (240, 2)                            # Sets position for the duplicated frame
        frame.size = (228, 300)                              # Sets size for the duplicated frame

        text = Text(self.canvas)                             # Creates a text label on the canvas
        text.text = "Shape"                                  # Sets label text
        text.position = (74, 10)                             # Positions label on the canvas
        text.font_size = 30                                  # Sets font size for label text

        text = Text(text)                                    # Duplicates text label with the same properties
        text.text = "Marker"                                 # Sets new text for duplicated label
        text.position = (308, 10)                            # Positions duplicated label on canvas

        text = Text(self.canvas)                             # Creates a new text label on the canvas
        text.text = "Hello"                                  # Sets label text
        text.position = (20, 55)                             # Positions label
        text.font_size = 25                                  # Sets font size for label
        text.angle_ = -20                                    # Sets rotation angle for label

        text = Text(self.canvas)                             # Creates another text label
        text.text = "Text"                                   # Sets label text
        text.position = (120, 70)                            # Positions label
        text.font_size = 20                                  # Sets font size for label

        polygon = Polygon(self.canvas)                       # Creates a polygon shape
        polygon.position = (20, 105)                         # Sets position of the polygon
        polygon.add_point(0, 35)                             # Adds a point to the polygon
        polygon.add_point(0, 18)                             # Adds another point
        polygon.add_point(30, 24)                            # Adds another point
        polygon.add_point(46, 6)                             # Adds another point
        polygon.add_cubic(72, 6)                             # Adds a cubic curve point
        polygon.add_cubic(72, 50)                            # Adds another cubic curve point
        polygon.add_point(46, 50)                            # Adds final point for the contour
        polygon.next_contour()                               # Begins a new contour for the polygon

        text = Text(self.canvas)                             # Label for the polygon shape
        text.text = "Polygon"                                # Sets label text
        text.position = (120, 130)                           # Positions label
        text.font_size = 20                                  # Sets font size for label

        polylines = PolyLines(self.canvas)                   # PolyLines shape with multiple points
        polylines.position = (20, 175)                       # Sets position for the polyline
        polylines.append(5, 20)                              # Adds the starting point
        polylines.append(10, 50)                             # Adds more points
        polylines.append(30, 10)
        polylines.append(40, 40)
        polylines.append(50, 0)
        polylines.append(70, 50)
        polylines.thickness_ = 4                             # Sets frame line thickness

        text = Text(self.canvas)                             # Creates a text label on the canvas
        text.text = "PolyLines"                              # Sets label text
        text.position = (120, 190)                           # Positions the label on the canvas
        text.font_size = 20                                  # Sets the font size for the label text

        line_ = Line(self.canvas)                            # Creates a line shape
        line_.position = (20, 235)                           # Sets position for the line
        line_.point1 = (4, 4)                                 # Sets the starting point of the line
        line_.point2 = (70, 45)                               # Sets the ending point of the line
        line_.thickness_ = 4                                  # Sets the thickness of the line

        text = Text(self.canvas)                             # Creates a text label on the canvas
        text.text = "Line"                                   # Sets label text
        text.position = (120, 250)                           # Positions the label on the canvas
        text.font_size = 20                                  # Sets the font size for the label text

        rect = Rect(self.canvas)                             # Creates a rectangle shape
        rect.size = (33, 44)                                  # Sets size of the rectangle
        rect.position = (18, 300)                             # Sets position for the rectangle
        rect.radius_ = 10                                     # Sets corner radius for rounded edges
        rect.thickness_ = 0                                   # No border

        rect = Rect(rect)                                    # Creates a rectangle shape
        rect.thickness_ = 3                                   # Sets the border thickness
        rect.size = (33, 44)                                  # Adjusts the size of the rectangle
        rect.position = (62, 300)                             # Sets new position for the rectangle

        text = Text(self.canvas)                             # Creates a text label on the canvas
        text.text = "Rect"                                   # Sets label text
        text.position = (120, 310)                           # Positions the label on the canvas
        text.font_size = 20                                  # Sets the font size for the label text

        pie = Pie(self.canvas)                                # Creates a pie shape
        pie.position = (36, 384)                              # Sets position for the pie shape
        pie.radius_ = 20                                      # Sets radius of the pie shape
        pie.thickness_ = 0                                    # No border
        pie.rope = True                                       # Enables rope effect for pie segments
        pie.start_angle_ = 0                                  # Sets start angle for the pie segment
        pie.sweep_angle_ = 300                                # Sets end angle for the pie segment

        pie = Pie(pie)                                        # Creates a pie shape
        pie.thickness_ = 3                                    # Sets thickness of the border
        pie.radius_ = 20                                      # Adjusts radius of the pie segment
        pie.position = (80, 384)                              # Sets new position for the pie shape

        text = Text(self.canvas)                             # Creates a text label on the canvas
        text.text = "Pie"                                    # Sets label text
        text.position = (120, 370)                           # Positions the label on the canvas
        text.font_size = 20                                  # Sets the font size for the label text

        circle = Circle(self.canvas)                         # Creates a circle marker
        circle.position = (266, 70)                           # Sets position for the circle
        circle.radius_ = 20                                   # Sets radius of the circle
        circle.thickness_ = 0                                 # No border

        circle = Circle(circle)                              # Creates a circle marker
        circle.position = (309, 70)                           # Sets new position for the circle
        circle.thickness_ = 3                                 # Sets thickness of the border
        circle.radius_ = 20                                   # Adjusts radius of the circle

        text = Text(self.canvas)                             # Creates a text label on the canvas
        text.text = "Circle"                                 # Sets label text
        text.position = (350, 60)                             # Positions the label on the canvas
        text.font_size = 20                                  # Sets the font size for the label text

        triangle = Triangle(self.canvas)                     # Creates a triangle marker
        triangle.position = (266, 125)                        # Sets position for the triangle
        triangle.radius_ = 20                                 # Sets size of the triangle's bounding radius
        triangle.thickness_ = 0                               # No border

        triangle = Triangle(triangle)                        # Creates a triangle marker
        triangle.thickness_ = 3                               # Sets thickness of the border
        triangle.position = (309, 125)                        # Sets new position for the triangle
        triangle.radius_ = 20                                 # Adjusts radius of the triangle

        text = Text(self.canvas)                             # Creates a text label on the canvas
        text.text = "Triangle"                               # Sets label text
        text.position = (350, 110)                            # Positions the label on the canvas
        text.font_size = 20                                  # Sets the font size for the label text

        square = Square(self.canvas)                          # Creates a square marker
        square.position = (266, 170)                          # Sets position for the square
        square.radius_ = 20                                   # Sets side length of the square
        square.thickness_ = 0                                 # No border

        square = Square(square)                              # Creates a square marker
        square.thickness_ = 3                                 # Sets thickness of the border
        square.position = (309, 170)                          # Sets new position for the square
        square.radius_ = 20                                   # Adjusts radius of the square

        text = Text(self.canvas)                             # Creates a text label on the canvas
        text.text = "Square"                                 # Sets label text
        text.position = (350, 160)                            # Positions the label on the canvas
        text.font_size = 20                                  # Sets the font size for the label text

        cross = Cross(self.canvas)                            # Creates a cross marker
        cross.position = (266, 220)                           # Sets position for the cross
        cross.radius_ = 20                                    # Sets size of the cross arms
        cross.thickness_ = 3                                  # Sets thickness of the cross arms

        text = Text(self.canvas)                             # Creates a text label on the canvas
        text.text = "Cross"                                  # Sets label text
        text.position = (350, 210)                            # Positions the label on the canvas
        text.font_size = 20                                  # Sets the font size for the label text

        star = Star(self.canvas)                              # Creates a star marker
        star.position = (266, 270)                            # Sets position for the star
        star.radius_ = 20                                     # Sets side length of the star
        star.thickness_ = 0                                   # No border

        star = Star(star)                                    # Creates a star marker
        star.thickness_ = 3                                   # Sets thickness of the border
        star.position = (309, 270)                            # Sets new position for the star
        star.radius_ = 20                                     # Adjusts radius of the star

        text = Text(self.canvas)                             # Creates a text label on the canvas
        text.text = "Star"                                   # Sets label text
        text.position = (350, 260)                            # Positions the label on the canvas
        text.font_size = 20                                  # Sets the font size for the label text

        text = Text(self.canvas)                             # Creates a text label on the canvas
        text.text = ("Pie, Circle, Triangle\n"
                     "Square, Cross, Star have\n"
                     "their origin point in the\n"
                     "center, all others are\n"
                     "in the upper left corner.")
        text.font_size = 16                                  # Sets the font size for the label text
        text.position = (240, 310)                            # Positions the label on the canvas


def sample_canvas(widget, event):
    """Sample function to show Canvas widget"""
    sample = SampleCanvas()               # Creates a SampleCanvas
    sample.title("Canvas sample")         # Add dialog title
    sample.add_back_button("Back")        # Add back button
    sample.exec()                         # Starts the interface and its main loop
