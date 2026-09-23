"""Python port of samples/sample_hello_world.cpp"""
from pyberialdraw import *


def _on_click_button(widget, event):
    UIManager.desktop().quit()                          # Exits the main loop, closing the dialog


def sample_hello_world():
    """Example of a window displaying a text "hello world" with an "ok" button.
    The example contains the initialization of the library, as well as the deinitialization"""
    # Initializes the UI manager with a new DeviceScreen named "Sample",
    # setting the screen width 320, height 480, pixel format, and zoom size
    UIManager.init(DeviceScreen("Sample"), 320, 480, Framebuf.ARGB8888, 2, "../resources;resources")

    window = Window()                                    # Creates a new window
    window.size = (150, 100)                             # Sizes the window (centered on the screen)

    column = Column(window)                              # Creates a column layout

    label = Label(column)                                # Creates a label
    label.text = "Hello world"                            # Sets the text of the label

    button = Button(column)                              # Creates a new button within the window
    button.text = "OK"                                   # Sets the text of the button
    button.on_click = _on_click_button                    # Binds the button to the click handler
    button.margin = 10                                   # Adds a margin around the button

    # IMPORTANT:
    # All widgets attached to a window are owned by their parent,
    # they are automatically destroyed when the window is destroyed

    UIManager.desktop().mainloop()                        # Starts the main loop of the UI manager

    UIManager.deinit()                                    # Terminates use of the library
