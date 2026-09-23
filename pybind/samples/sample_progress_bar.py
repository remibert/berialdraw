"""Python port of samples/sample_progress_bar.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleProgressBar(Dialog):
    """Simple user interface with progress bars and a label"""

    def __init__(self):
        super().__init__()
        main_layout = Column(self.content)                    # Creates a content column within the scroll view
        self._next_id_value = 0

        label = Label(main_layout)                            # Creates a label in the main layout
        label.text = "Horizontal progress bars"

        horizontal = Column(main_layout)                      # Creates a column layout for horizontal progress bars

        progress_bar = ProgressBar(horizontal)                # Creates a horizontal progress bar
        progress_bar.id = self._next_id()
        progress_bar.value = 33

        progress_bar = ProgressBar(horizontal)                # Creates another horizontal progress bar
        progress_bar.id = self._next_id()
        progress_bar.align = Align.ALIGN_RIGHT                # Aligns the progress bar to the right
        progress_bar.value = 33

        progress_bar = ProgressBar(horizontal)                # Creates another horizontal progress bar
        progress_bar.id = self._next_id()
        progress_bar.track_size_ = 3                          # Sets the track thickness to 3
        progress_bar.fill_size_ = 20                          # Sets the fill thickness to 20
        progress_bar.fill_color = Color.SUCCESS_COLOR         # Sets the fill color to a success color
        progress_bar.value = 33

        progress_bar = ProgressBar(horizontal)                # Creates another horizontal progress bar
        progress_bar.id = self._next_id()
        progress_bar.align = Align.ALIGN_RIGHT                # Aligns the progress bar to the right
        progress_bar.track_size_ = 3                          # Sets the track thickness to 3
        progress_bar.fill_size_ = 20                          # Sets the fill thickness to 20
        progress_bar.fill_color = Color.ERROR_COLOR           # Sets the fill color to an error color
        progress_bar.value = 33

        label = Label(main_layout)                            # Creates a label in the main layout
        label.text = "Vertical progress bars"

        vertical = Row(main_layout)                           # Creates a row layout for vertical progress bars
        vertical.min_size = (0, 200)                          # Sets the minimum size for the vertical row

        progress_bar = ProgressBar(vertical)                  # Creates a vertical progress bar
        progress_bar.extend = Extend.EXTEND_HEIGHT            # Allows the progress bar to extend vertically
        progress_bar.align = Align.ALIGN_BOTTOM               # Aligns the progress bar to the bottom
        progress_bar.id = self._next_id()
        progress_bar.value = 33

        progress_bar = ProgressBar(vertical)                  # Creates another vertical progress bar
        progress_bar.extend = Extend.EXTEND_HEIGHT            # Allows the progress bar to extend vertically
        progress_bar.align = Align.ALIGN_TOP                  # Aligns the progress bar to the top
        progress_bar.id = self._next_id()
        progress_bar.value = 33

        progress_bar = ProgressBar(vertical)                  # Creates another vertical progress bar
        progress_bar.extend = Extend.EXTEND_HEIGHT            # Allows the progress bar to extend vertically
        progress_bar.track_size_ = 3                          # Sets the track thickness to 3
        progress_bar.fill_size_ = 20                          # Sets the fill thickness to 20
        progress_bar.fill_color = Color.SUCCESS_COLOR         # Sets the fill color to a success color
        progress_bar.align = Align.ALIGN_BOTTOM               # Aligns the progress bar to the bottom
        progress_bar.id = self._next_id()
        progress_bar.value = 33

        progress_bar = ProgressBar(vertical)                  # Creates another vertical progress bar
        progress_bar.extend = Extend.EXTEND_HEIGHT            # Allows the progress bar to extend vertically
        progress_bar.align = Align.ALIGN_TOP                  # Aligns the progress bar to the top
        progress_bar.track_size_ = 3                          # Sets the track thickness to 3
        progress_bar.fill_size_ = 20                          # Sets the fill thickness to 20
        progress_bar.id = self._next_id()
        progress_bar.fill_color = Color.ERROR_COLOR           # Sets the fill color to an error color
        progress_bar.value = 33

        self.label = Label(main_layout)                       # Creates a label to display progress percentage
        self.label.text = "33%"

        slider = Slider(main_layout)                          # Creates a slider in the main layout
        slider.margin = 20                                    # Adds margin around the slider
        slider.on_slide = self._on_slide_event                # Binds the slider to an event handler
        slider.value = 33                                     # Sets the initial slider value to 33

    def _next_id(self):
        self._next_id_value += 1
        return self._next_id_value

    def _on_slide_event(self, widget, slide_event):
        for identifier in range(self._next_id_value + 1):
            progress_bar = widget.root.search(identifier)
            if isinstance(progress_bar, ProgressBar):
                progress_bar.value = slide_event.value()
        self.label.text = f"{slide_event.value()}%"


def sample_progress_bar(widget, event):
    """Sample function to show ProgressBar widget"""
    sample = SampleProgressBar()             # Creates a SampleProgressBar
    sample.title("ProgressBar sample")       # Add dialog title
    sample.add_back_button("Back")           # Add quit progress_bar
    sample.exec()                            # Starts the interface and its main loop
