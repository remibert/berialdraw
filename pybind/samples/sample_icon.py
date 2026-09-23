"""Python port of samples/sample_icon.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleIcon(Dialog):
    """Simple user interface with icons and a label"""

    def __init__(self):
        super().__init__()
        self.row = Row(self.content)                          # Creates a row layout within the scroll view
        self.row.flow = True                                  # Enables flow layout for automatic positioning

        self.clicked = 0                                      # Counter for the number of icon clicks

        icon = Icon(self.row)                                 # Adds a new icon to the content row
        icon.filename = "$(ui.icons)/computer.icn"            # Sets the filename for the icon image
        icon.text = "Default"                                 # Sets the icon's text
        icon.on_click = self._on_icon_clicked                 # Binds the click event to the handler

        icon = Icon(self.row)                                 # Adds another icon to the content row
        icon.filename = "$(ui.icons)/computer.icn"            # Sets the filename for the icon image
        icon.text = "Without\nfocus"                          # Sets the icon's text
        icon.on_click = self._on_icon_clicked                 # Binds the click event to the handler
        icon.focusable = False                                # Disables focus for this icon

        icon = Icon(self.row)                                 # Adds a rounded icon to the content row
        icon.filename = "$(ui.icons)/computer.icn"            # Sets the filename for the icon image
        icon.text = "Rounded"                                 # Sets the icon's text
        icon.radius_ = 1000                                   # Sets a large radius for a fully rounded shape
        icon.on_click = self._on_icon_clicked                 # Binds the click event to the handler

        icon = Icon(self.row)                                 # Adds a square icon to the content row
        icon.filename = "$(ui.icons)/computer.icn"            # Sets the filename for the icon image
        icon.text = "Square"                                  # Sets the icon's text
        icon.radius_ = 0                                      # Sets the radius to 0 for sharp edges
        icon.on_click = self._on_icon_clicked                 # Binds the click event to the handler

        icon = Icon(self.row)                                 # Adds another icon to the content row
        icon.filename = "$(ui.icons)/computer.icn"            # Sets the filename for the icon image
        icon.text = "Full color"                              # Sets the icon's text
        icon.on_click = self._on_icon_clicked                 # Binds the click event to the handler

        self.label = Label(self.row)                          # Adds a new label to the content row
        self.label.text = "Icon not\nclicked"                 # Sets the default text of the label
        self.label.font_size = 16                             # Sets the font size of the label text

    def _on_icon_clicked(self, widget, event):
        self.clicked += 1                                     # Increments click counter
        self.label.text = f"Icon\nclicked\n{self.clicked} times"  # Updates label text with click count


def sample_icon(widget, event):
    """Sample function to show Icon widget"""
    sample = SampleIcon()                # Creates a SampleIcon
    sample.title("Icon sample")          # Add dialog title
    sample.add_back_button("Back")       # Add quit icon
    sample.exec()                        # Starts the interface and its main loop
