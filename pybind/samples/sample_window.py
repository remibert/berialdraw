"""Python port of samples/sample_window.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleWindow(Dialog):
	"""Simple user interface demonstrating extra top-level windows"""

	def __init__(self):
		super().__init__()
		self.window1 = Window()                                # First sub-window
		self.window2 = Window()                                # Second sub-window

		# Configure the first sub-window
		self.window1.size = (300, 190)                         # Sets the size of the window
		self.window1.position = (40, 100)                      # Positions the window at (40, 100)

		pane = Pane(self.window1)                              # Adds a pane container inside the window
		pane.radius_ = 15                                      # Rounded corners with a radius of 15
		pane.thickness_ = 1                                    # Border thickness of 1
		pane.color = Color.LIGHT_GRAY                          # Sets the pane background color
		pane.border_color = Color.GRAY                         # Sets the border color
		pane.margin = 5                                        # Adds a margin around the pane

		column = Column(pane)                                  # Adds a column layout inside the pane

		label = Label(column)                                  # Adds a label inside the column
		label.text = "Window placed in \nabsolute"             # Sets the label text
		label.margin = 5                                       # Adds a margin around the label

		button = Button(column)                                # Adds a button inside the column
		button.text = "Ok"                                     # Sets the button text
		button.margin = 5                                      # Adds a margin around the button

		# Configure the second sub-window
		self.window2.size = (100, 90)                          # Sets the size of the window
		self.window2.align = Align.ALIGN_BOTTOM                # Aligns the window to the bottom
		self.window2.extend = Extend.EXTEND_WIDTH              # Extends the window's width across the screen

		pane = Pane(self.window2)                              # Adds a pane container inside the window
		pane.radius_ = 15                                      # Rounded corners with a radius of 15
		pane.thickness_ = 1                                    # Border thickness of 1
		pane.color = Color.LIGHT_GRAY                          # Sets the pane background color
		pane.border_color = Color.GRAY                         # Sets the border color
		pane.margin = 5                                        # Adds a margin around the pane

		column = Column(pane)                                  # Adds a column layout inside the pane

		label = Label(column)                                  # Adds a label inside the column
		label.text = "Extended window width at bottom"         # Sets the label text
		label.margin = 5                                       # Adds a margin around the label

		button = Button(column)                                # Adds a button inside the column
		button.text = "Ok"                                     # Sets the button text
		button.margin = 5                                      # Adds a margin around the button

	def _on_menu_quit(self, widget, event):
		if isinstance(widget, Button):
			# These extra top-level windows aren't cleaned up by Dialog.exec()
			UIManager.desktop().remove(self.window1)
			UIManager.desktop().remove(self.window2)
		super()._on_menu_quit(widget, event)


def sample_window(widget, event):
	"""Sample function to show Window widget"""
	sample = SampleWindow()             # Creates a SampleWindow
	sample.title("Window sample")       # Add dialog title
	sample.add_back_button("Back")      # Add back button
	sample.exec()                       # Starts the interface and its main loop
