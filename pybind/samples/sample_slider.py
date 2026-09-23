"""Python port of samples/sample_slider.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleSlider(Dialog):
	"""Simple user interface with sliders and a label"""

	def __init__(self):
		super().__init__()
		main_layout = Column(self.content)                    # Creates a content column within the scroll view
		self._next_id_value = 0

		label = Label(main_layout)                            # Creates a label in the main layout
		label.text = "Horizontal slider"

		horizontal = Column(main_layout)                      # Creates a column layout for horizontal sliders

		slider = Slider(horizontal)                           # Creates a horizontal slider
		slider.id = self._next_id()
		slider.on_slide = self._on_slide_event                # Binds the slider to an event handler
		slider.margin = 10                                    # Adds margin around the slider

		slider = Slider(horizontal)                           # Creates another horizontal slider
		slider.id = self._next_id()
		slider.track_size_ = 20                               # Sets the thickness of the slider track
		slider.handle_size_ = 16                              # Sets the size of the slider handle
		slider.track_color = Color.SUCCESS_COLOR              # Sets the track color to a success color
		slider.radius_ = 0                                    # Sets the slider corners to sharp edges
		slider.on_slide = self._on_slide_event                # Binds the slider to an event handler
		slider.margin = 10                                    # Adds margin around the slider

		label = Label(main_layout)                            # Creates a label in the main layout
		label.text = "Vertical slider"

		vertical = Row(main_layout)                           # Creates a row layout for vertical sliders
		vertical.min_size = (0, 240)                          # Sets the minimum size of the vertical row

		slider = Slider(vertical)                             # Creates a vertical slider
		slider.extend = Extend.EXTEND_HEIGHT                  # Allows the slider to extend vertically
		slider.id = self._next_id()
		slider.on_slide = self._on_slide_event                # Binds the slider to an event handler
		slider.margin = 10                                    # Adds margin around the slider

		slider = Slider(vertical)                             # Creates another vertical slider
		slider.extend = Extend.EXTEND_HEIGHT                  # Allows the slider to extend vertically
		slider.track_size_ = 30                               # Sets the thickness of the slider track
		slider.handle_size_ = 26                              # Sets the size of the slider handle
		slider.track_color = Color.SUCCESS_COLOR              # Sets the track color to a success color
		slider.id = self._next_id()
		slider.margin = 10                                    # Adds margin around the slider
		slider.radius_ = 100                                  # Sets the slider corners to rounded edges
		slider.on_slide = self._on_slide_event                # Binds the slider to an event handler

		self.label = Label(main_layout)                       # Creates a label for displaying the slider value
		self.label.text = "0%"                                # Sets the initial label text to "0%"

	def _next_id(self):
		self._next_id_value += 1
		return self._next_id_value

	def _on_slide_event(self, widget, slide_event):
		for identifier in range(self._next_id_value + 1):
			slider = widget.root.search(identifier)
			if isinstance(slider, Slider):
				slider.value = slide_event.value()
		self.label.text = f"{slide_event.value()}%"


def sample_slider(widget, event):
	"""Sample function to show Slider widget"""
	sample = SampleSlider()              # Creates a SampleSlider
	sample.title("Slider sample")        # Add dialog title
	sample.add_back_button("Back")       # Add quit slider
	sample.exec()                        # Starts the interface and its main loop
