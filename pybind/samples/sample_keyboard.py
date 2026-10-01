"""Python port of samples/sample_keyboard.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleKeyboard(Dialog):
	"""Simple user interface with an edit field and a keyboard"""

	def __init__(self):
		super().__init__()
		column = Column(self.content)                        # Creates a content column within the scroll view

		label = Label(column)                                # Creates label
		label.text = "Enter text with keyboard"              # Set text to label

		edit = Edit(column)                                  # Create edit field
		edit.margin = 10                                     # Add margins around edit field
		edit.max_lines = 7                                   # Set the edit field on many lines

		Keyboard(column)                                     # Creates a text keyboard on the layout


def sample_keyboard(widget, event):
	"""Sample function to show Keyboard widget"""
	sample = SampleKeyboard()           # Creates a SampleKeyboard
	sample.title("Keyboard sample")     # Add dialog title
	sample.add_back_button("Back")      # Add back button
	sample.exec()                       # Starts the interface and its main loop
