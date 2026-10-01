"""Python port of samples/menu_dialog.cpp / menu_dialog.hpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class MenuDialog(Dialog):
	"""Dialog box to display a menu with a list of options, a title and a button to cancel"""

	def __init__(self):
		super().__init__()
		self.list = List(self.content)                       # Creates a list view
		self.list.size_policy = SizePolicy.ENLARGE_ALL
		self.list.focusable = True
		self.list.margin = 0
		self.list.thickness = 0

	def _on_menu_click(self, widget, event):
		if isinstance(widget, ListItem):
			self.selected = widget.text                     # Stores the text of the selected item
		UIManager.desktop().quit()                           # Exits the main loop, closing the dialog

	def create_menu(self, text, icon_filename=""):
		"""Create item in menu"""
		def configure(item):
			item.text = " " + text
			item.padding = 9
			item.leading = icon_filename
			item.trailing = ">"
		return self.list.append(configure)

	def add_choice(self, text, icon_filename="", callback=None):
		"""Add choice in menu, optionally binding a callback (widget, event) -> None"""
		item = self.create_menu(text, icon_filename)
		if callback is not None:
			item.on_click = callback
		else:
			item.on_click = self._on_menu_click              # Binds the click event to the menu click handler
		return item

	def bind(self, text, icon_filename, callback):
		"""Bind event on a function/method, creating the menu choice"""
		return self.add_choice(text, icon_filename, callback)
