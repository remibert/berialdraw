"""Python port of samples/dialog.cpp / dialog.hpp"""
from pyberialdraw import *


class Dialog:
	"""Dialog box to display a menu with a list of icons, a title and a button to cancel"""

	def __init__(self):
		self.window = Window()

		self.main_layout = Column(self.window)             # Sets up a main layout as a vertical column

		self.header = Pane(self.main_layout)               # Creates a header pane within the main layout
		self.header.color = Color.HEADER_COLOR             # Sets header background color
		self.header.size_policy = SizePolicy.SHRINK_HEIGHT # Shrinks header height based on content

		scroll = ScrollView(self.main_layout)              # Adds a scrollable view for menu items
		scroll.scroll_direction = ScrollDirection.SCROLL_VERTICAL
		self.content = scroll
		self.content.align = Align.ALIGN_TOP               # Aligns scroll view to the top

		self.selected = ""

	def _on_menu_quit(self, widget, event):
		if isinstance(widget, Button):
			self.selected = "<quit>"                       # Stores the text of the selected button
			UIManager.desktop().remove(self.window)        # Destroys the window (mirrors C++ Dialog's RAII ~Window)
		UIManager.desktop().quit()                         # Exits the main loop, closing the dialog

	def title(self, text):
		"""Add title menu"""
		label = Label(self.header)                         # Creates a label inside the header pane
		label.text = text                                  # Sets the title text
		label.text_color = Color.HEADER_TEXT_COLOR         # Sets text color to white

	def add_back_button(self, text):
		"""Add close button"""
		button = Button(self.header)                       # Creates a button inside the header pane
		button.text = f"< {text}"                          # Sets button text with a back arrow
		button.text_color = Color.HEADER_TEXT_COLOR        # Sets text color to white
		button.text_align = Align.ALIGN_LEFT               # Aligns text to the left
		button.border_color = Color.TRANSPARENT            # Sets border color to transparent
		button.color = Color.TRANSPARENT                   # Sets background color to transparent
		button.thickness_ = 0                              # Removes border thickness
		button.align = Align.ALIGN_LEFT                    # Aligns button to the left
		button.extend = Extend.EXTEND_HEIGHT               # Extends button height to fill available space
		button.on_click = self._on_menu_quit               # Binds the click event to the quit handler
		button.focusable = False                           # Remove button focus border

	def exec(self):
		"""Execute dialog menu and return the string selected"""
		self.selected = ""                                 # Initializes selected item to an empty string
		UIManager.desktop().mainloop()                     # Runs the main event loop for the desktop
		return self.selected                               # Returns the selected item after loop ends
