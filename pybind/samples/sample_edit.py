"""Python port of samples/sample_edit.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleEdit(Dialog):
	"""Simple user interface with buttons and an edit"""

	def __init__(self):
		super().__init__()
		column = Column(self.content)                              # Creates a content column within the scroll view

		# Default edit field
		label = Label(column)                                      # Creates a label for the default edit field
		label.text = "Default edit"                                # Sets the label text
		label.text_align = Align.ALIGN_LEFT                        # Aligns the text to the left

		edit = Edit(column)                                        # Creates a default text edit field
		edit.place_holder = "Enter your text"                      # Sets a placeholder text

		# Time input field
		label = Label(column)                                      # Creates a label for the time input field
		label.text = "\nTime"                                      # Sets the label text
		label.text_align = Align.ALIGN_LEFT                        # Aligns the text to the left

		edit = Edit(column)                                        # Creates an edit field for time input
		edit.text = "00:00:00"                                     # Sets the default text
		edit.mask = "%02[0..23]d:%02[0..59]d:%02[0..59]d"          # Applies a mask for time input (HH:MM:SS)

		# Temperature input field
		label = Label(column)                                      # Creates a label for the temperature input field
		label.text = "\nTemperature"                               # Sets the label text
		label.text_align = Align.ALIGN_LEFT                        # Aligns the text to the left

		edit = Edit(column)                                        # Creates an edit field for temperature input
		edit.text = "-100"                                         # Sets the default text
		edit.mask = "%[-99..99]d.%02[0..99]d"                      # Applies a mask for temperature input

		# IP address input field
		label = Label(column)                                      # Creates a label for the IP address input field
		label.text = "\nIp address"                                # Sets the label text
		label.text_align = Align.ALIGN_LEFT                        # Aligns the text to the left

		edit = Edit(column)                                        # Creates an edit field for IP address input
		edit.text = "0.0.0.0"                                      # Sets the default text
		edit.mask = "%3[0..255]d.%3[0..255]d.%3[0..255]d.%3[0..255]d"  # Applies a mask for IPv4 input


def sample_edit(widget, event):
	"""Sample function to show Edit widget"""
	sample = SampleEdit()                # Creates a SampleEdit
	sample.title("Edit sample")          # Add dialog title
	sample.add_back_button("Back")       # Add back button
	sample.exec()                        # Starts the interface and its main loop
