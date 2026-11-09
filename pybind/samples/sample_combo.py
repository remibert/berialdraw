"""Python port of samples/sample_combo.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleCombo(Dialog):
    """Date-selection example built with Combo widgets"""

    def __init__(self):
        super().__init__()
        content = Column(self.content)                            # Stack the fields and preview vertically
        content.margin = 16                                        # Add space around the sample content

        fields = Row(content)                                     # Place the three date fields on one line
        fields.margin = 8                                          # Add spacing around the row

        day_field = Column(fields)                                # Group the day caption and selector
        label = Label(day_field)                                  # Create the day caption
        label.text = "Day"                                        # Label the day selector
        self.day = Combo(day_field)                               # Create the day selector
        self.day.size = (90, 40)                                  # Set its display size
        self.day.editable = False                                  # Restrict the day to the provided choices
        self.day.on_key_down = self._on_combo_key                  # Refresh after keyboard selection

        month_field = Column(fields)                              # Group the month caption and selector
        label = Label(month_field)                                # Create the month caption
        label.text = "Month"                                      # Label the month selector
        self.month = Combo(month_field)                           # Create the month selector
        self.month.size = (150, 40)                               # Allow room for full month names
        self.month.editable = False                                # Restrict the month to the provided choices
        self.month.on_key_down = self._on_combo_key                # Refresh after keyboard selection

        year_field = Column(fields)                               # Group the year caption and selector
        label = Label(year_field)                                 # Create the year caption
        label.text = "Year (editable)"                            # Indicate that custom years are accepted
        self.year = Combo(year_field)                             # Create the year selector
        self.year.size = (130, 40)                                # Set its display size
        self.year.editable = True                                  # Allow years outside the suggested range
        self.year.on_key_down = self._on_combo_key                 # Refresh as the year is typed

        for day in range(1, 32):                                  # Populate the fixed day choices
            item = self.day.append(f"{day:02d}")                  # Add a two-digit day value
            item.on_click = self._on_option_click                  # Refresh when a choice is clicked

        months = (
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December",
        )                                                         # Fixed month choices
        for month in months:
            item = self.month.append(month)                       # Add one month to the list
            item.on_click = self._on_option_click                 # Refresh when a choice is clicked

        for year in range(2000, 2027):                            # Add suggested years; keep the field editable
            item = self.year.append(str(year))                    # Add a suggested year
            item.on_click = self._on_option_click                 # Refresh when a choice is clicked
        self.year.select(26)                                      # Start with 2026 selected

        self.preview = Label(content)                             # Display the composed date below the row
        self.preview.margin = (12, 8)                             # Separate the preview from the selectors
        self._update_preview()                                     # Show the initial date immediately

    def _on_option_click(self, _widget, _event):
        self._update_preview()                                    # Reflect the selection in the preview

    def _on_combo_key(self, _widget, _event):
        self._update_preview()                                    # Reflect keyboard selection or year edits

    def _update_preview(self):
        self.preview.text = f"Selected date: {self.day.input} {self.month.input} {self.year.input}"  # Combine values without date validation


def sample_combo(_widget, _event):
    """Sample function to show Combo widget"""
    sample = SampleCombo()                                        # Create the date-selection dialog
    sample.title("Date selection")                               # Set the dialog title
    sample.add_back_button("Back")                               # Add a button to return to the menu
    sample.exec()                                                 # Run the dialog
