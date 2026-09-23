"""Python port of samples/sample_grid.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleGrid(Dialog):
	"""Simple user interface with a grid of buttons"""

	def __init__(self):
		super().__init__()
		self.grid = Grid(self.content)                       # Creates a content grid within the scroll view

		for row in range(12):
			for column in range(6):
				button = Button(self.grid)                   # Creates a button within the grid
				button.text = f"R{row} C{column}"            # Sets label text
				button.cell(row, column)                     # Set the position in grid layout


def sample_grid(widget, event):
	"""Sample function to show Grid widget"""
	sample = SampleGrid()               # Creates a SampleGrid
	sample.title("Grid sample")         # Add dialog title
	sample.add_back_button("Back")      # Add back button
	sample.exec()                       # Starts the interface and its main loop
