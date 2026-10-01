"""Python port of samples/samples.cpp"""
from pyberialdraw import *
from samples.icon_menu_dialog import IconMenuDialog
from samples.menu_dialog import MenuDialog

from samples.sample_button import sample_button
from samples.sample_canvas import sample_canvas
from samples.sample_column import sample_column
from samples.sample_edit import sample_edit
from samples.sample_grid import sample_grid
from samples.sample_icon import sample_icon
from samples.sample_keyboard import sample_keyboard
from samples.sample_label import sample_label
from samples.sample_pane import sample_pane
from samples.sample_progress_bar import sample_progress_bar
from samples.sample_row import sample_row
from samples.sample_scrollview import sample_scroll_view
from samples.sample_slider import sample_slider
from samples.sample_switch import sample_switch
from samples.sample_window import sample_window
from samples.sample_speedometer import sample_speedometer
from samples.sample_theme import sample_palette
from samples.sample_tableview import sample_tableview
from samples.sample_picture import sample_picture
from samples.sample_list import sample_list


# Menu items configuration (shared between icon and plain menus)
menu_bindings = [
	("Button",      "$(ui.icons)/view_agenda.icn",        sample_button),
	("Canvas",      "$(ui.icons)/draw_abstract.icn",      sample_canvas),
	("Column",      "$(ui.icons)/table_rows_narrow.icn",  sample_column),
	("Edit",        "$(ui.icons)/text_fields_alt.icn",    sample_edit),
	("Grid",        "$(ui.icons)/grid_on.icn",            sample_grid),
	("Icon",        "$(ui.icons)/image.icn",              sample_icon),
	("Keyboard",    "$(ui.icons)/keyboard.icn",           sample_keyboard),
	("Label",       "$(ui.icons)/format_size.icn",        sample_label),
	("Pane",        "$(ui.icons)/featured_video.icn",     sample_pane),
	("ProgressBar", "$(ui.icons)/sliders.icn",            sample_progress_bar),
	("Row",         "$(ui.icons)/calendar_view_week.icn", sample_row),
	("ScrollView",  "$(ui.icons)/scrollable_header.icn",  sample_scroll_view),
	("Slider",      "$(ui.icons)/tune.icn",               sample_slider),
	("Switch",      "$(ui.icons)/toggle_on.icn",          sample_switch),
	("Window",      "$(ui.icons)/select_window.icn",      sample_window),
	("Speedometer", "$(ui.icons)/speed.icn",              sample_speedometer),
	("Theme",       "$(ui.icons)/filter_vintage.icn",     sample_palette),
	("TableView",   "$(ui.icons)/grid_on.icn",            sample_tableview),
	("Picture",     "$(ui.icons)/image.icn",              sample_picture),
	("List",        "$(ui.icons)/sort.icn",               sample_list),
]


def populate_menu(dialog):
	"""Populate dialog with all menu bindings"""
	for text, icon, callback in menu_bindings:
		dialog.bind(text, icon, callback)


def sample_icon_menu():
	"""Sample of icon menu"""
	# UIManager.notifier().log()                   # Log all user events if it uncommented
	UIManager.style = "pearl"                       # Select the style pearl
	UIManager.appearance = "light"                  # Select the light appearance
	UIManager.palette = Color.PALETTE_LIME          # Select the color palette

	dialog = IconMenuDialog()

	dialog.title("Samples")
	dialog.add_back_button("Back")

	populate_menu(dialog)

	while dialog.exec() != "<quit>":
		pass


def sample_menu():
	"""Sample of menu"""
	UIManager.style = "pearl"                       # Select the style pearl
	UIManager.appearance = "light"                  # Select the light appearance
	UIManager.palette = Color.PALETTE_LIME          # Select the color palette

	dialog = MenuDialog()

	dialog.title("Samples")
	dialog.add_back_button("Back")
	populate_menu(dialog)

	while dialog.exec() != "<quit>":
		pass
