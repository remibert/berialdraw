"""Python port of samples/sample_pane.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog

_PANES = [
    # (radius, position)
    (10, (50, 10)),
    (0, (200, 200)),
    (5, (350, 390)),
]


class SamplePane(Dialog):
    """Simple user interface with a few panes"""

    def __init__(self):
        super().__init__()
        for index, (radius, position) in enumerate(_PANES):
            pane = Pane(self.content)                        # Creates a pane container within the scroll view
            pane.radius_ = radius                            # Sets the border radius of the pane
            pane.thickness_ = 1                              # Sets the border thickness of the pane
            pane.position = position                         # Positions the pane
            pane.extend = Extend.EXTEND_NONE                 # Configures the pane to not extend

            column = Column(pane)                            # Adds a column layout inside the pane
            label = Label(column)                            # Creates a label inside the column
            label.text = f"Pane {index + 1}"                 # Sets the label text
            label.margin = 5                                 # Adds a margin around the label

            button = Button(column)                          # Creates a button inside the column
            button.text = "Ok"                               # Sets the button text
            button.margin = 5                                # Adds a margin around the button


def sample_pane(widget, event):
    """Sample function to show Pane widget"""
    sample = SamplePane()               # Creates a SamplePane
    sample.title("Pane sample")         # Add dialog title
    sample.add_back_button("Back")      # Add back button
    sample.exec()                       # Starts the interface and its main loop
