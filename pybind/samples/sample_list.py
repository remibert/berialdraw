"""Python port of samples/sample_list.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleList(Dialog):
	"""Simple user interface with a list widget"""

	def __init__(self):
		super().__init__()
		column = Column(self.content)                        # Creates a content column within the scroll view

		label = Label(column)                                # Creates a label above the list
		label.text = "List Widget Example"                   # Sets the label text

		self.list = List(column)                             # Creates a list widget within the column
		self.list.size = (400, 150)                          # Sets a fixed size for the list
		self.list.margin = 10                                # Adds margin around the list

		self.list.append("Item 1")                           # Populates the list with a few default items
		self.list.append("Item 2")
		self.list.append("Item 3")

		self.count = 0                                       # Counter for the number of items added
		button = Button(column)                              # Creates a button to add new items dynamically
		button.text = "add"                                  # Sets button text
		button.margin = (20, 10)                             # Adds margin around the button
		button.on_click = self._on_add_button                # Binds click event to on_add_button handler

	def _on_add_button(self, widget, event):
		self.count += 1                                      # Increments item counter
		item = self.list.append(f"image {self.count}")       # Appends a new item to the list
		item.trailing = "$(ui.icons)/computer.icn"           # Sets a trailing icon
		item.text_color = Color.RED                          # Sets the item text color


def sample_list(widget, event):
	"""Sample function to show List widget"""
	sample = SampleList()                # Creates a SampleList
	sample.title("List sample")          # Add dialog title
	sample.add_back_button("Back")       # Add back button
	sample.exec()                        # Starts the interface and its main loop
