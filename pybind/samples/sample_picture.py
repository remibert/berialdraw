"""Python port of samples/sample_picture.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SamplePicture(Dialog):
	"""Simple user interface with picture display"""

	def __init__(self):
		super().__init__()
		row = Row(self.content)                              # Creates a row layout within the scroll view
		row.margin = 20                                      # Adds margin around the row

		# Display PNG image
		png_image = Picture(row)                             # Creates a picture for PNG image
		png_image.filename = "$(ui.images)/house.png"        # Sets the PNG filename
		png_image.fit_mode = ImageFitMode.FIT                # Keep aspect ratio
		png_image.margin = 10                                # Adds margin

		# Display JPG image
		jpg_image = Picture(row)                             # Creates a picture for JPG image
		jpg_image.filename = "$(ui.images)/filleperle.jpg"   # Sets the JPG filename
		jpg_image.fit_mode = ImageFitMode.FIT                # Keep aspect ratio
		jpg_image.margin = 10                                # Adds margin


def sample_picture(widget, event):
	"""Sample function to show Picture widget"""
	sample = SamplePicture()             # Creates a SamplePicture
	sample.title("Picture sample")       # Add dialog title
	sample.add_back_button("Back")       # Add back button
	sample.exec()                        # Starts the interface and its main loop
