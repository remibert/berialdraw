"""Python port of samples/sample_theme.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog

PROGRESS_BAR_ID = 123
PALETTE_NAME_ID = 124

# Keyed by plain int (the value stored in a widget's `id`), not the Color enum itself
_PALETTE_NAMES = {
	int(Color.PALETTE_RED): "RED",
	int(Color.PALETTE_ORANGE): "ORANGE",
	int(Color.PALETTE_YELLOW): "YELLOW",
	int(Color.PALETTE_LIME): "LIME",
	int(Color.PALETTE_GREEN): "GREEN",
	int(Color.PALETTE_JADE): "JADE",
	int(Color.PALETTE_CYAN): "CYAN",
	int(Color.PALETTE_AZURE): "AZURE",
	int(Color.PALETTE_BLUE): "BLUE",
	int(Color.PALETTE_INDIGO): "INDIGO",
	int(Color.PALETTE_PURPLE): "PURPLE",
	int(Color.PALETTE_VIOLET): "VIOLET",
	int(Color.PALETTE_FUSHIA): "FUSHIA",
	int(Color.PALETTE_PINK): "PINK",
	int(Color.PALETTE_ROSEWOOD): "ROSEWOOD",
	int(Color.PALETTE_SAND): "SAND",
	int(Color.PALETTE_ASH): "ASH",
	int(Color.PALETTE_GRAY): "GRAY",
	int(Color.PALETTE_ZINC): "ZINC",
	int(Color.PALETTE_SLATE): "SLATE",
}


class SampleTheme(Dialog):
	"""Palette selection and rendering widgets preview"""

	def __init__(self):
		super().__init__()
		column = Column(self.content)                        # Creates a content column within the scroll view

		head = Row(column)                                   # Creates a new row within the column
		label = Label(head)                                  # Creates a new label within the row
		label.text = "Theme color : "                        # Sets the text of the label
		label.text_align = Align.ALIGN_LEFT                  # Aligns the text to the left
		label.size_policy = SizePolicy.SHRINK_WIDTH          # Sets the size policy to shrink width
		label.padding = 0

		label = Label(head)                                  # Creates another label within the row
		label.text = ""                                      # Sets the text of the label to an empty string
		label.id = PALETTE_NAME_ID                           # Sets the ID of the label
		label.text_align = Align.ALIGN_LEFT                  # Aligns the text to the left
		label.padding = 0

		dark_light = Switch(head)                             # Creates a new switch within the row
		dark_light.size_policy = SizePolicy.SHRINK_WIDTH      # Sets the size policy to shrink width
		dark_light.focusable = False                          # Sets the switch to be non-focusable
		dark_light.checked = True                             # Sets the switch to be checked
		dark_light.on_check = self._on_dark_light_event       # Binds the switch to an event handler
		dark_light.padding = 0

		label = Label(head)                                   # Creates another label within the row
		label.text = "Dark/Light"                             # Sets the text of the label
		label.size_policy = SizePolicy.SHRINK_WIDTH           # Sets the size policy to shrink width
		label.padding = 0

		# Theme color picker
		color_picker = Row(column)                            # Creates a content row within the scroll view
		for palette_color, _ in _PALETTE_NAMES.items():
			color = Button(color_picker)                      # Creates a button within the row
			color.color = palette_color                       # Sets the palette color
			color.text = " "                                  # Sets label text
			color.focusable = False                           # Sets the button to be non-focusable
			color.radius_ = 0                                 # Sets the radius of the button to 0
			color.margin = 1                                  # Sets the margin of the button to 1
			color.id = int(palette_color)                     # Sets the ID of the button
			color.on_click = self._on_color_event             # Binds the button to an event handler
			color.thickness_ = 0

		label = Label(column)
		label.text = "Rendering Widgets"

		# Show top widgets
		row = Row(column)                                     # Creates a new row within the column
		col = Column(row)                                     # Creates a new column within the row
		button = Button(col)                                  # Creates a new button within the column
		button.text = "Button"                                # Sets the text of the button
		button.padding = 0

		edit = Edit(col)                                      # Creates a new edit box within the column
		edit.text = "Hello World"                             # Sets the text of the edit box
		edit.select_all()                                     # Selects all text in the edit box
		edit.place_holder = "Edit"                            # Sets the placeholder text of the edit box
		edit.max_columns = 15                                 # Sets the maximum number of columns for the edit box
		edit.focused = True                                   # Sets the edit box to be focused

		col = Column(row)                                     # Creates another column within the row
		switch_ = Switch(col)                                 # Creates a new switch within the column
		switch_.checked = True                                # Sets the switch to be checked
		checkbox = Checkbox(col)                              # Creates a new checkbox within the column
		checkbox.checked = True                               # Sets the checkbox to be checked
		slider = Slider(col)                                  # Creates a new slider within the column
		slider.on_slide = self._on_slide_event                # Binds the slider to an event handler
		slider.value = 50                                     # Sets the value of the slider
		progress_bar = ProgressBar(col)                       # Creates a new progress bar within the column
		progress_bar.id = PROGRESS_BAR_ID                     # Sets a unique ID for the progress bar
		progress_bar.value = 50                               # Sets the value of the progress bar

		col = Column(row)                                     # Creates another column within the row
		col.size_policy = SizePolicy.SHRINK_WIDTH             # Sets the size policy to shrink width
		icon = Icon(col)                                      # Creates a new icon within the column
		icon.text = "Icon"                                    # Sets the text of the icon
		icon.filename = "$(ui.icons)/sentiment_excited.icn"   # Sets the filename of the icon

		Keyboard(column)                                      # Create keyboard

		canvas = Canvas(column)                               # Create canvas
		canvas.size = (200, 40)                               # Set size of canvas
		circle = Circle(canvas)                               # Creates a circle marker
		circle.position = (70 + 18, 20)                       # Sets new position for the circle
		circle.thickness_ = 2                                 # Sets thickness of the border
		circle.radius_ = 14                                   # Adjusts radius of the circle

		triangle = Triangle(canvas)                           # Creates a triangle marker
		triangle.thickness_ = 2                               # Sets thickness of the border
		triangle.position = (70 + 18 + 32, 24)                # Sets new position for the triangle
		triangle.radius_ = 16                                 # Adjusts radius of the triangle

		square = Square(canvas)                               # Creates a square marker
		square.thickness_ = 2                                 # Sets thickness of the border
		square.position = (70 + 18 + 64, 20)                  # Sets new position for the square
		square.radius_ = 18                                   # Adjusts radius of the square

		text = Text(canvas)                                   # Create a text
		text.position = (0, 10)                               # Sets position of text
		text.text = "Canvas"                                  # Sets text
		text.font_size = 20                                   # Adjusts fonts size

	def _on_slide_event(self, widget, slide_event):
		progress_bar = widget.root.search(PROGRESS_BAR_ID)
		if isinstance(progress_bar, ProgressBar):
			progress_bar.value = slide_event.value()

	def _on_dark_light_event(self, widget, check_event):
		if check_event.checked():
			UIManager.appearance = "light"
		else:
			UIManager.appearance = "dark"

	def _on_color_event(self, widget, click_event):
		palette_name = widget.root.search(PALETTE_NAME_ID)   # Search the label for the palette name
		UIManager.palette = widget.id                        # Change the color palette
		if palette_name is not None:
			palette_name.text = _PALETTE_NAMES.get(widget.id, "")


def sample_palette(widget, event):
	"""Sample function to show all colors palettes"""
	sample = SampleTheme()               # Creates a SampleTheme
	sample.title("Theme sample")         # Add dialog title
	sample.add_back_button("Back")       # Add back button
	sample.exec()                        # Starts the interface and its main loop
