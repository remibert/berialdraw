"""Python port of samples/sample_popup.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SamplePopup(Dialog):
	"""Open a popup at the click position to change a counter"""

	def __init__(self):
		super().__init__()                                        # Initializes the shared dialog layout
		column = Column(self.content)                             # Creates a content column within the scroll view

		instruction = Label(column)                               # Creates an instruction label
		instruction.text = "Click anywhere to change the counter" # Explains how to open the popup
		instruction.color = Color.GRAY                            # Styles the instruction label
		instruction.margin = 2                                    # Adds space around the instruction

		self.counter = 0                                          # Stores the current counter value
		self.counter_label = Label(column)                        # Creates a label to display the counter
		self.counter_label.text = f"Count: {self.counter}"        # Sets the initial counter text

		self.popup = Popup(self.window)                           # Creates a popup owned by the dialog window

		self.list = List(self.popup)                              # Creates a list inside the popup
		self.list.extend = Extend.EXTEND_ALL                      # Extends the list to the popup bounds
		self.list.scroll_direction = ScrollDirection.SCROLL_VERTICAL # Keeps the action list vertically scrollable
		self.list.margin = 2                                      # Adds space around the list items

		decrease_item = self.list.append("Decrease")              # Adds the decrease action
		decrease_item.on_click = self._on_decrease                # Binds the decrease handler
		increase_item = self.list.append("Increase")              # Adds the increase action
		increase_item.on_click = self._on_increase                # Binds the increase handler

		self.window.on_click = self._on_dialog_click              # Binds clicks on the dialog window

	# Open the popup when the user clicks anywhere except the Back button or popup.
	def _on_dialog_click(self, widget, event):
		is_back_button = widget.classname == "button" and widget.text == "< Back" # Identifies the dialog's Back button
		current = widget                                          # Starts at the widget that received the click
		in_popup = False                                          # Tracks whether the click belongs to the popup
		while current is not None and not in_popup:
			if current.classname == "popup":
				in_popup = True
			else:
				current = current.parent
		if not is_back_button and not in_popup:
			self.popup.open_at(event.position)                    # Opens the popup at the click position

	# Close the popup and increment the displayed counter.
	def _on_increase(self, widget, event):
		self.popup.close()                                        # Closes the popup after selecting an action
		self.counter += 1                                         # Increments the counter
		self.counter_label.text = f"Count: {self.counter}"        # Refreshes the displayed counter

	# Close the popup and decrement the displayed counter.
	def _on_decrease(self, widget, event):
		self.popup.close()                                        # Closes the popup after selecting an action
		self.counter -= 1                                         # Decrements the counter
		self.counter_label.text = f"Count: {self.counter}"        # Refreshes the displayed counter


def sample_popup(widget, event):
	"""Sample function to show the Popup widget"""
	sample = SamplePopup()                                        # Creates a SamplePopup
	sample.title("Popup sample")                                  # Adds the dialog title
	sample.add_back_button("Back")                                # Adds a back button
	sample.exec()                                                 # Starts the interface and its main loop