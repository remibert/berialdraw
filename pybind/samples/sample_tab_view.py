"""Python port of samples/sample_tab_view.cpp"""
from pyberialdraw import *
from samples.dialog import Dialog


class SampleTabView(Dialog):
	"""Tab view showing a labeled widget on each page"""

	def __init__(self):
		super().__init__()

		content = Column(self.content)                            # Creates a column for the tab view
		content.margin = 8                                        # Adds spacing around the tab view

		self.tabs = TabView(content)                              # Creates the tab container

		page = self._create_tab_content("Button")                 # Creates the Button tab and its label
		button = Button(page)                                     # Adds a button to the tab page
		button.text = "Click me"                                  # Sets the button caption

		page = self._create_tab_content("Canvas")                 # Creates the Canvas tab and its label
		canvas = Canvas(page)                                     # Creates a drawing area on the tab page

		# Draws a star and a triangle on the canvas.
		star = Star(canvas)                                       # Creates a star marker on the canvas
		star.position = (35, 55)                                  # Positions the star
		star.radius = 32                                          # Sets the star size
		star.thickness = 0                                        # Fills the star without an outline

		triangle = Triangle(canvas)                               # Creates a triangle marker on the canvas
		triangle.position = (185, 65)                             # Positions the triangle beside the star
		triangle.radius = 32                                      # Sets the triangle size
		triangle.thickness = 0                                    # Fills the triangle without an outline

		page = self._create_tab_content("Combo")                  # Creates the Combo tab and its label
		combo = Combo(page)                                       # Creates a selection combo box
		combo.size = (220, 45)                                    # Sets the combo box size
		combo.editable = False                                    # Restricts the combo box to its choices
		combo.append("First option")                              # Adds the first choice
		combo.append("Second option")                             # Adds the second choice
		combo.append("Third option")                              # Adds the third choice
		combo.select(0)                                           # Selects the first choice initially

		page = self._create_tab_content("Edit")                   # Creates the Edit tab and its label
		edit = Edit(page)                                         # Creates a text input field
		edit.text = "Type here"                                   # Sets its initial text
		self.keyboard = Keyboard(page)                            # Creates a virtual keyboard

		page = self._create_tab_content("Icon")                   # Creates the Icon tab and its label
		icon = Icon(page)                                         # Creates an icon widget
		icon.filename = "$(ui.icons)/computer.icn"                # Loads the computer icon
		icon.text = "Computer"                                    # Adds a caption below the icon

		page = self._create_tab_content("List")                   # Creates the List tab and its label
		list_widget = List(page)                                  # Creates a scrollable list
		list_widget.append("First item")                          # Adds the first list item
		list_widget.append("Second item")                         # Adds the second list item
		list_widget.append("Third item")                          # Adds the third list item

		page = self._create_tab_content("Slider")                 # Creates the Slider tab and its label
		slider = Slider(page)                                     # Creates a value slider
		slider.value = 60                                         # Sets the initial value

		page = self._create_tab_content("Switch")                 # Creates the Switch tab and its label
		toggle = Switch(page)                                     # Creates a toggle switch
		toggle.checked = True                                     # Starts the switch in the on state

	def _create_tab_content(self, title):
		"""Create a tab page with a descriptive label and a vertical layout"""
		tab_page = self.tabs.add_tab(title)                       # Creates a tab and its content page
		content = Column(tab_page)                                # Stacks the label and widget vertically
		content.margin = 12                                       # Adds spacing inside the tab page

		label = Label(content)                                    # Creates a descriptive label for the widget
		label.text = f"{title} widget example"                    # Names the widget shown on this tab
		label.margin = 8                                          # Separates the label from the widget

		return content


def sample_tab_view(widget, event):
	"""Sample function to show the TabView widget"""
	sample = SampleTabView()                                      # Creates the TabView sample dialog
	sample.title("TabView sample")                                # Sets the dialog title
	sample.add_back_button("Back")                                # Adds a button to return to the menu
	sample.exec()                                                 # Runs the dialog event loop
