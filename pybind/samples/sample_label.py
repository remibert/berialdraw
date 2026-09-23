"""Python port of samples/sample_label.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleLabel(Dialog):
    """Simple user interface with a variety of labels"""

    def __init__(self):
        super().__init__()
        column = Column(self.content)                        # Creates a content column within the scroll view

        label = Label(column)                                # Creates a text label on the layout
        label.text = "Default label"                         # Sets the default text for the label

        label = Label(column)                                # Creates a text label on the layout
        label.text = "With value : 1234"                     # Sets the label text with a formatted integer value

        label = Label(column)                                # Creates a text label on the layout
        label.text = "Utf8 encoded : rémi"                   # Sets UTF-8 encoded text with special characters

        label = Label(column)                                # Creates a text label on the layout
        label.text = "Large"                                 # Sets the label text
        label.font_size = (80, 30)                           # Sets font size: large width, smaller height

        label = Label(column)                                # Creates a text label on the layout
        label.text = "Narrow"                                # Sets the label text
        label.font_size = (30, 80)                           # Sets font size: narrow width, taller height

        label = Label(column)                                # Creates a text label on the layout
        label.text_align = Align.ALIGN_LEFT                  # Aligns text to the left
        label.text = "Align left"                            # Sets the label text

        label = Label(column)                                # Creates a text label on the layout
        label.text_align = Align.ALIGN_RIGHT                 # Aligns text to the right
        label.text = "Align right"                           # Sets the label text

        label = Label(column)                                # Creates a text label on the layout
        label.text = "Many\nlines\nlabel"                    # Sets multiline text for the label


def sample_label(widget, event):
    """Sample function to show Label widget"""
    sample = SampleLabel()               # Creates a SampleLabel
    sample.title("Label sample")         # Add dialog title
    sample.add_back_button("Back")       # Add back button
    sample.exec()                        # Starts the interface and its main loop
