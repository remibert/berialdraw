"""Python port of samples/sample_speedometer.cpp

NOTE: see sample_canvas.py for the current pybind shape-binding limitation
(Shape/CommonStyle not yet exposed on most shapes). Shape.repeat() is also
not exposed in pyberialdraw, so the graduation repetition is done here with
an explicit Python loop instead.
"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleSpeedometer(Dialog):
	"""Speedometer UI component that displays speed with a sliding control"""

	def __init__(self):
		super().__init__()
		main_layout = Column(self.content)

		self.window.color = Color.HEADER_COLOR             # Set the window's background color

		self.dial_center = (240, 200)
		self.needle = None
		self.speed_text = None
		self.canvas = None

		self.counter_radius = 200                           # Radius of the speedometer dial
		self.font_size = self.counter_radius // 6           # Font size for the speed text

		self.speed_min = 0                                  # Minimum speed value
		self.speed_max = 100                                # Maximum speed value
		self.speed_inc = 10                                 # Speed increment for graduations

		self.angle_start = -90                              # Starting angle of the dial (leftmost point)
		self.angle_end = 180                                # Ending angle of the dial (rightmost point)
		self.angle_inc = (abs(self.angle_start) + self.angle_end) // ((self.speed_max - self.speed_min) // self.speed_inc)

		self._create(main_layout)

		speed = Slider(main_layout)                          # Creates a speed slider
		speed.on_slide = self._on_slide_event                # Bind slider events to handler
		speed.margin = (20, 0)                               # Add vertical margin
		speed.focusable = False                              # Disable focus for the slider

	def _on_slide_event(self, widget, slide_event):
		# Updates the needle's angle based on the slider's value
		self.needle.angle_ = 0 - (slide_event.value() * self.angle_inc) // self.speed_inc
		# Updates the speed text to display the current slider value
		self.speed_text.text = str(slide_event.value())

	def _draw_dial(self, column):
		self.canvas = Canvas(column)                         # Create the canvas for the speedometer
		self.canvas.size = (480, 400)                        # Set the canvas size
		self.canvas.color = Color.HEADER_COLOR               # Set the canvas background color
		self.canvas.id = 1234

		border = Circle(self.canvas)                         # Add a circular border to the speedometer dial
		border.color = Color.GRAY                            # Set the border color
		border.position = self.dial_center                   # Position the border at the center
		border.radius_ = self.counter_radius                 # Set the radius of the border
		border.thickness_ = 1                                # Define the thickness of the border

	def _draw_graduations(self):
		pie = Pie(self.canvas)                                # Create the outer graduation circle
		pie.color = Color.BLUE                                # Set the pie color
		pie.position = self.dial_center                       # Position the pie at the dial center
		pie.radius_ = self.counter_radius - 8                 # Set the pie radius
		pie.thickness_ = 5                                    # Define the pie thickness
		pie.rope = False                                      # Disable connecting ropes
		pie.start_angle_ = self.angle_start                   # Sets the starting angle of the pie
		pie.sweep_angle_ = self.angle_end                     # Sets the ending angle of the pie

		# Major graduations, repeated across the dial (Shape.repeat() is not exposed in pyberialdraw)
		# -90 replicates the offset Shape::paints() applies internally for REPEAT_ANGLE
		angle = self.angle_start
		while angle <= self.angle_end:
			major = Line(self.canvas)
			major.color = Color.BLUE
			major.point1 = (0, 8 - self.counter_radius)
			major.point2 = (0, 8 - self.counter_radius + 14)
			major.position = self.dial_center
			major.thickness_ = 3
			major.angle_ = angle - 90
			angle += self.angle_inc

		# Minor graduations, between the major ones
		angle = self.angle_start + (self.angle_inc // 2) + 1
		while angle <= self.angle_end - (self.angle_inc // 2):
			minor = Line(self.canvas)
			minor.color = Color.BLUE
			minor.point1 = (0, 8 - self.counter_radius)
			minor.point2 = (0, 8 - self.counter_radius + 10)
			minor.position = self.dial_center
			minor.thickness_ = 1
			minor.angle_ = angle - 90
			angle += self.angle_inc

	def _draw_text(self):
		compass = Compass()                                   # Create a compass to position the texts
		compass.center = self.dial_center                     # Set the center of the compass
		compass.radius = self.counter_radius - 22 - self.font_size  # Set the radius to position the texts

		speed = 0                                             # Initialize the speed value
		angle = self.angle_end
		while angle >= self.angle_start:
			compass.angle = angle                             # Rotate the compass to the correct angle
			graduation_text = Text(self.canvas)               # Create a new text object
			graduation_text.font_size = self.font_size        # Set the font size of the text
			graduation_text.color = Color.WHITE               # Set the text color
			graduation_text.text = str(speed)                 # Set the text to display the speed value
			graduation_text.center = graduation_text.content_size().middle()  # Center the text on its own middle
			graduation_text.position = compass.position()     # Position the text at the compass angle
			speed += self.speed_inc                           # Increment the speed value
			angle -= self.angle_inc

	def _draw_needle(self):
		self.needle = Polygon(self.canvas)                    # Create the needle as a polygon
		self.needle.color = Color.RED                         # Set the needle color
		self.needle.position = self.dial_center               # Position the needle at the dial center
		self.needle.angle_ = 0                                # Set the initial rotation angle of the needle

		# Define the shape of the needle (elongated trapezoid)
		self.needle.add_point(0 - (self.counter_radius - 20), 3)
		self.needle.add_cubic(0 - (self.counter_radius - 15), 3)
		self.needle.add_cubic(0 - (self.counter_radius - 15), -3)
		self.needle.add_point(0 - (self.counter_radius - 20), -3)
		self.needle.add_point(0, -8)
		self.needle.add_cubic(13, -8)
		self.needle.add_cubic(13, 8)
		self.needle.add_point(0, 8)
		self.needle.next_contour()

		circle = Circle(self.canvas)                          # Add a white circle above the needle
		circle.radius_ = self.counter_radius // 12            # Set the radius of the circle
		circle.position = self.dial_center                    # Position the circle at the dial center
		circle.color = Color.WHITE                            # Set the circle color
		circle.thickness_ = 0                                 # Fill the circle

	def _draw_speed_text(self):
		self.speed_text = Text(self.canvas)                   # Create a text element for the current speed
		self.speed_text.position = (
			self.dial_center[0] - (self.counter_radius // 2) - self.font_size,
			self.dial_center[1] + (self.counter_radius // 2) - (self.font_size * 2),
		)
		self.speed_text.font_size = self.font_size * 2        # Set the font size for the speed text
		self.speed_text.text = "0"                            # Initialize the speed text to "0"
		self.speed_text.color = Color.WHITE                   # Set the text color to white

	def _create(self, column):
		"""Creates a speedometer ranging from 0 to 100"""
		self._draw_dial(column)                               # Draw the dial background
		self._draw_graduations()                              # Draw the graduations on the dial
		self._draw_text()                                     # Add graduation labels
		self._draw_needle()                                   # Draw the speed indicator needle
		self._draw_speed_text()                               # Add the dynamic speed text


def sample_speedometer(widget, event):
	"""Sample function to show speedometer on canvas widget"""
	sample = SampleSpeedometer()             # Creates a SampleSpeedometer
	sample.title("Speedometer sample")       # Add dialog title
	sample.add_back_button("Back")           # Add quit speedometer
	sample.exec()                            # Starts the interface and its main loop
