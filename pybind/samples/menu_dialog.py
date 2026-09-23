"""Python port of samples/menu_dialog.cpp / menu_dialog.hpp"""
from pyberialdraw import *
from samples.dialog import Dialog

# Bit flags from include/vector/border.hpp (Borders enum is not exposed to pybind)
BOTTOM_BORDER = 0x04


class MenuDialog(Dialog):
    """Dialog box to display a menu with a list of options, a title and a button to cancel"""

    def __init__(self):
        super().__init__()
        self.column = Column(self.content)                  # Creates a content column within the scroll view

    def _on_menu_click(self, widget, event):
        if isinstance(widget, Button):
            self.selected = widget.text                     # Stores the text of the selected button
        UIManager.desktop().quit()                           # Exits the main loop, closing the dialog

    def create_button(self, text):
        """Create button in menu"""
        button = Button(self.column)                        # Creates a new button for a menu choice
        button.text = text                                  # Sets button text to the provided item
        button.text_align = Align.ALIGN_LEFT                # Aligns text to the left
        button.border_color = Color.BLACK                   # Sets border color (light black with alpha not exposed)
        button.borders = BOTTOM_BORDER                       # Draws border only on the bottom side
        button.margin = (0, 0, 1, 0)                         # Adds margin around the button
        button.thickness_ = 1                                # Sets border thickness to 1
        button.radius_ = 0                                   # Sets corner radius to 0 (square corners)

        label = Label(button)                                # Adds a label inside the button for a right arrow
        label.text = ">"                                    # Sets label text to right arrow
        label.text_color = Color.BUTTON_TEXT_COLOR          # Sets arrow text color
        label.text_align = Align.ALIGN_RIGHT                # Aligns arrow text to the right
        label.font_size = button.font_size                  # Matches label font size to button font size
        return button                                       # Returns the created button

    def add_choice(self, text, callback=None):
        """Add choice in menu, optionally binding a callback (widget, event) -> None"""
        button = self.create_button(text)
        if callback is not None:
            button.on_click = callback
        else:
            button.on_click = self._on_menu_click            # Binds the click event to the menu click handler
        return button

    def bind(self, text, callback):
        """Bind event on a function/method, creating the menu choice"""
        return self.add_choice(text, callback)
