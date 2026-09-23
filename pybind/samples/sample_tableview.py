"""Python port of samples/sample_tableview.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleTableView(Dialog):
    """Simple user interface with a table view"""

    def __init__(self):
        super().__init__()
        column = Column(self.content)                        # Creates a content column within the scroll view

        self.label = Label(column)                           # Creates a label within the column
        self.label.text = "Select a row"                     # Sets default label text
        self.label.margin = 10                               # Adds margin

        self.table = TableView(column)                       # Creates a table view within the column
        self.table.margin = 10                               # Adds margin around the table

        # Load table data
        table_data = [
            ["Name", "Age", "City"],
            ["Alice", "28", "Paris"],
            ["Bob", "35", "Lyon"],
            ["Charlie", "42", "Marseille"],
            ["Diana", "31", "Toulouse"],
            ["Eve", "55", "Nice"],
        ]
        self.table.load(table_data)                          # Load the data into the table
        self.table.on_click = self._on_row_clicked           # Bind click event

    def _on_row_clicked(self, widget, event):
        self.label.text = "Row selected"                     # Updates label when row is clicked


def sample_tableview(widget, event):
    """Sample function to show TableView widget"""
    sample = SampleTableView()               # Creates a SampleTableView
    sample.title("TableView sample")         # Add dialog title
    sample.add_back_button("Back")           # Add back button
    sample.exec()                            # Starts the interface and its main loop
