"""Python port of samples/sample_switch.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleSwitch(Dialog):
    """Simple user interface with switches, radios, checkbox and a label"""

    def __init__(self):
        super().__init__()
        main_layout = Column(self.content)                    # Creates a content column within the scroll view

        label = Label(main_layout)                            # Creates a label in the main layout
        label.text = "Normal switch"

        switch_ = Switch(main_layout)                         # Creates a switch
        switch_.on_check = self._on_check_event               # Binds the switch to an event handler
        switch_.focusable = False                             # Remove the focus border

        label = Label(main_layout)                            # Creates a label in the main layout
        label.text = "Customized switch"

        switch_ = Switch(main_layout)                         # Creates a switch
        switch_.on_check = self._on_check_event               # Binds the switch to an event handler
        switch_.radius_ = 40                                  # Rounded border
        switch_.switch_size = (200, 80)                       # Enlarge size of switch
        switch_.thumb_padding_ = 5                             # Set the thumb padding in track
        switch_.on_track_color = Color.LIGHT_GREEN            # Change the track color when checked
        switch_.off_track_color = Color.RED                   # Change the track color when not checked
        switch_.focusable = False                             # Remove the focus border

        label = Label(main_layout)                            # Creates a label in the main layout
        label.text = "Radio buttons (group 1)"

        radio = Radio(main_layout)                            # Creates a radio button
        radio.on_click = self._on_radio_event                 # Binds the radio to an event handler
        radio.text = "Option 1"                               # Sets the radio text
        radio.group = "group1"                                # Assigns to group 1
        radio.focusable = False                               # Remove the focus border

        radio = Radio(main_layout)                            # Creates a radio button
        radio.on_click = self._on_radio_event                 # Binds the radio to an event handler
        radio.text = "Option 2"                               # Sets the radio text
        radio.group = "group1"                                # Assigns to group 1
        radio.focusable = False                               # Remove the focus border

        label = Label(main_layout)                            # Creates a label in the main layout
        label.text = "Checkbox"

        checkbox = Checkbox(main_layout)                      # Creates a checkbox
        checkbox.on_check = self._on_check_event              # Binds the checkbox to an event handler
        checkbox.text = "Accept terms"                        # Sets the checkbox text
        checkbox.focusable = False                            # Remove the focus border

        self.label = Label(main_layout)                       # Creates a label for displaying the widget value
        self.label.text = ""                                  # Sets the initial label text to empty string

    def _on_radio_event(self, widget, click_event):
        self.label.text = "Radio selected in group 1"

    def _on_check_event(self, widget, check_event):
        self.label.text = "Checked" if check_event.checked() else "Unchecked"


def sample_switch(widget, event):
    """Sample function to show Switch widget"""
    sample = SampleSwitch()              # Creates a SampleSwitch
    sample.title("Switch sample")        # Add dialog title
    sample.add_back_button("Back")       # Add quit switch
    sample.exec()                        # Starts the interface and its main loop
