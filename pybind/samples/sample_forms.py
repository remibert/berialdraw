"""Python port of samples/sample_forms.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleForms(Dialog):
    """Simple login form with username, password, remember me and a keyboard"""

    def __init__(self):
        super().__init__()
        page = Column(self.content)

        pane = Pane(page)
        pane.radius_ = 10                                    # Sets the border radius of the pane
        pane.thickness_ = 2                                  # Sets the border thickness of the pane
        pane.margin = (100, 10)
        pane.extend = Extend.EXTEND_ALL                      # Configures the pane to extend in all directions

        column = Column(pane)                                # Creates a content column within the scroll view
        column.margin = 10

        login = Edit(column)                                 # Creates a default text edit field
        login.place_holder = "Username"                      # Sets a placeholder text

        password = Edit(column)                              # Creates a default text edit field
        password.place_holder = "Password"                   # Sets a placeholder text
        password.password = True                             # Hide the entry

        row = Row(column)
        remember_me = Switch(row)
        remember_me.align = Align.ALIGN_RIGHT
        remember_me.size_policy = SizePolicy.SHRINK_ALL
        label = Label(row)
        label.text = "Remember me"
        label.text_align = Align.ALIGN_LEFT

        sign_in = Button(column)
        sign_in.text = "Sign in"
        sign_in.margin = (30, 10)

        Keyboard(page)


def sample_forms(widget, event):
    """Sample function to show forms"""
    sample = SampleForms()               # Creates a SampleForms
    sample.title("Forms sample")         # Add dialog title
    sample.add_back_button("Back")       # Add back button
    sample.exec()                        # Starts the interface and its main loop
