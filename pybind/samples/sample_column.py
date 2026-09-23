"""Python port of samples/sample_column.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleColumn(Dialog):
    """Simple user interface with a column of buttons"""

    def __init__(self):
        super().__init__()
        column = Column(self.content)                        # Creates a content column within the scroll view

        for i in range(26):
            button = Button(column)                          # Creates a button within the column
            button.text = chr(0x41 + i)                      # Sets label text


def sample_column(widget, event):
    """Sample function to show Column widget"""
    sample = SampleColumn()               # Creates a SampleColumn
    sample.title("Column sample")         # Add dialog title
    sample.add_back_button("Back")        # Add back button
    sample.exec()                         # Starts the interface and its main loop
