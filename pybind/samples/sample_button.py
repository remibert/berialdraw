"""Python port of samples/sample_button.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleButton(Dialog):
    """Simple user interface with buttons and a label"""

    def __init__(self):
        super().__init__()
        column = Column(self.content)                        # Creates a content column within the scroll view

        self.label = Label(column)                           # Creates a label within the column
        self.label.text = "Button not clicked"               # Sets default label text
        self.label.id = 1234

        self.clicked = 0                                     # Counter for the number of button clicks

        button = Button(column)                              # Creates a main button that the user can click
        button.text = "Click\non me"                         # Sets button text with line break
        button.font_size = 50                                # Sets font size
        button.margin = 20                                   # Adds margin around the button
        button.radius_ = 100                                 # Rounds corners with radius of 100
        button.thickness_ = 4                                # Sets border thickness
        button.on_click = self._on_button_clicked            # Binds click event to the handler

    def _on_button_clicked(self, widget, event):
        self.clicked += 1                                    # Increments click counter
        self.label.text = f"Button clicked {self.clicked} times"  # Updates label text with click count


def sample_button(widget, event):
    """Sample function to show Button widget"""
    sample = SampleButton()               # Creates a SampleButton
    sample.title("Button sample")         # Add dialog title
    sample.add_back_button("Back")        # Add back button
    sample.exec()                         # Starts the interface and its main loop
