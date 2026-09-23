"""Python port of samples/sample_scrollview.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleScrollView(Dialog):
	"""Simple user interface with horizontal and vertical scroll views"""

	def __init__(self):
		super().__init__()
		self.content.destroy()                                        # Remove scroll view created in Dialog class
		self.content = Column(self.main_layout)                       # Creates a content column within the scroll view

		label = Label(self.content)                                   # Creates a label
		label.text = "Horizontal scroll view"                         # Set text to label

		horizontal_scroll_view = ScrollView(self.content)             # Create scroll view
		horizontal_scroll_view.scroll_direction = ScrollDirection.SCROLL_HORIZONTAL
		horizontal_scroll_view.size_policy = SizePolicy.ENLARGE_ALL
		horizontal_layout = Row(horizontal_scroll_view)               # Creates a content row within the scroll view
		for i in range(26):
			button = Button(horizontal_layout)                        # Creates a button within the row
			button.text = chr(0x41 + i)                               # Sets label text

		label = Label(self.content)                                   # Creates a label
		label.text = "Vertical scroll view"                           # Set text to label

		vertical_scroll_view = ScrollView(self.content)               # Create scroll view
		vertical_scroll_view.scroll_direction = ScrollDirection.SCROLL_VERTICAL
		vertical_scroll_view.size_policy = SizePolicy.ENLARGE_ALL
		vertical_scroll_layout = Column(vertical_scroll_view)         # Creates a content column within the scroll view
		for i in range(26):
			button = Button(vertical_scroll_layout)                   # Creates a button within the column
			button.text = chr(0x41 + i)                               # Sets label text


def sample_scroll_view(widget, event):
	"""Sample function to show ScrollView widget"""
	sample = SampleScrollView()          # Creates a SampleScrollView
	sample.title("ScrollView sample")    # Add dialog title
	sample.add_back_button("Back")       # Add back button
	sample.exec()                        # Starts the interface and its main loop
