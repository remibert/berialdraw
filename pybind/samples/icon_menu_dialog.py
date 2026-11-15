"""Python port of samples/icon_menu_dialog.cpp / icon_menu_dialog.hpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class IconMenuDialog(Dialog):
	"""Dialog box to display a menu with a list of icons, a title and a button to cancel"""

	def __init__(self):
		super().__init__()
		self.row = Row(self.content)                        # Creates a row layout within the dialog
		self.row.flow = True                                 # Enables flow layout for automatic positioning of icons

	def _on_menu_click(self, widget, event):
		if isinstance(widget, Icon):
			self.selected = widget.text                     # Updates the selected text with the clicked icon's text
		UIManager.desktop().quit()                           # Closes the dialog after the menu selection

	def create_icon(self, text, icon_filename):
		"""Create icon in menu"""
		icon = Icon(self.row)                                # Adds a new icon to the content row

		# Text settings
		icon.text = text                                     # Sets the icon's text
		icon.font_size = 15                                  # Sets the font size of the text
		icon.text_align = Align.CENTER                       # Aligns the text to the center

		# Icon settings
		icon.filename = icon_filename                        # Sets the filename for the icon image
		icon.icon_color = Color.WHITE                        # Sets the color of icon
		icon.color = self._next_backcolor()                   # Sets the computed color of background icon
		icon.thickness = 0                                   # Sets the thickness of borders
		icon.radius = 200                                    # Sets the radius of rounded borders
		icon.icon_padding = 20                               # Adds paddings between icon and borders
		# NOTE: icon_frame_size is not exposed in pyberialdraw yet

		# Other settings
		# icon.focusable = False                               # Disables focus for the icon
		icon.margin = 5                                      # Sets a margin of 5 around the icon
		return icon

	def add_choice(self, text, icon_filename, callback=None):
		"""Add choice in menu, optionally binding a callback (widget, event) -> None"""
		icon = self.create_icon(text, icon_filename)
		if callback is not None:
			icon.on_click = callback
		else:
			icon.on_click = self._on_menu_click              # Binds the click event to the menu click handler
		return icon

	def bind(self, text, icon_filename, callback):
		"""Bind event on a function/method, creating the icon choice"""
		return self.add_choice(text, icon_filename, callback)
