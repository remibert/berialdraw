"""Python port of samples/sample_row.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleRow(Dialog):
	"""Simple user interface with a row of buttons"""

	def __init__(self):
		super().__init__()
		self.content.scroll_direction = ScrollDirection.SCROLL_HORIZONTAL  # Set the scroll view in horizontal mode
		row = Row(self.content)                              # Creates a content row within the scroll view
		for i in range(26):
			button = Button(row)                             # Creates a button within the row
			button.text = chr(0x41 + i)                      # Sets label text


def sample_row(widget, event):
	"""Sample function to show Row widget"""
	sample = SampleRow()                # Creates a SampleRow
	sample.title("Row sample")          # Add dialog title
	sample.add_back_button("Back")      # Add back button
	sample.exec()                       # Starts the interface and its main loop
