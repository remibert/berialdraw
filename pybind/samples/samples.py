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
from samples.sample_forms import sample_forms


def sample_icon_menu():
    """Sample of icon menu"""
    # UIManager.notifier().log()                   # Log all user events if it uncommented
    UIManager.style = "pearl"                       # Select the style pearl
    UIManager.appearance = "light"                  # Select the light appearance
    UIManager.palette = Color.PALETTE_LIME          # Select the color palette

    dialog = IconMenuDialog()

    dialog.title("Samples")
    dialog.add_back_button("Back")

    dialog.bind("Button", "$(ui.icons)/view_agenda.icn", sample_button)
    dialog.bind("Canvas", "$(ui.icons)/draw_abstract.icn", sample_canvas)
    dialog.bind("Column", "$(ui.icons)/table_rows_narrow.icn", sample_column)
    dialog.bind("Edit", "$(ui.icons)/text_fields_alt.icn", sample_edit)
    dialog.bind("Grid", "$(ui.icons)/grid_on.icn", sample_grid)
    dialog.bind("Icon", "$(ui.icons)/image.icn", sample_icon)
    dialog.bind("Keyboard", "$(ui.icons)/keyboard.icn", sample_keyboard)
    dialog.bind("Label", "$(ui.icons)/format_size.icn", sample_label)
    dialog.bind("Pane", "$(ui.icons)/featured_video.icn", sample_pane)
    dialog.bind("ProgressBar", "$(ui.icons)/sliders.icn", sample_progress_bar)
    dialog.bind("Row", "$(ui.icons)/calendar_view_week.icn", sample_row)
    dialog.bind("ScrollView", "$(ui.icons)/scrollable_header.icn", sample_scroll_view)
    dialog.bind("Slider", "$(ui.icons)/tune.icn", sample_slider)
    dialog.bind("Switch", "$(ui.icons)/toggle_on.icn", sample_switch)
    dialog.bind("Window", "$(ui.icons)/select_window.icn", sample_window)
    dialog.bind("Speedometer", "$(ui.icons)/speed.icn", sample_speedometer)
    dialog.bind("Theme", "$(ui.icons)/filter_vintage.icn", sample_palette)
    dialog.bind("TableView", "$(ui.icons)/grid_on.icn", sample_tableview)
    dialog.bind("Picture", "$(ui.icons)/image.icn", sample_picture)

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
    dialog.bind("Button", sample_button)
    dialog.bind("Canvas", sample_canvas)
    dialog.bind("Column", sample_column)
    dialog.bind("Edit", sample_edit)
    dialog.bind("Grid", sample_grid)
    dialog.bind("Icon", sample_icon)
    dialog.bind("Keyboard", sample_keyboard)
    dialog.bind("Label", sample_label)
    dialog.bind("Pane", sample_pane)
    dialog.bind("ProgressBar", sample_progress_bar)
    dialog.bind("Row", sample_row)
    dialog.bind("ScrollView", sample_scroll_view)
    dialog.bind("Slider", sample_slider)
    dialog.bind("Switch", sample_switch)
    dialog.bind("Window", sample_window)
    dialog.bind("Speedometer", sample_speedometer)
    dialog.bind("Theme", sample_palette)
    dialog.bind("TableView", sample_tableview)
    dialog.bind("Picture", sample_picture)

    while dialog.exec() != "<quit>":
        pass
